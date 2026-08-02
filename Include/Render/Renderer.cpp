/*
 *	Copyright 2026-2027 static - hiro
 *
 *	Use of this source code without explicit permission from owner is strictly prohibited.
 */

#include "Render/Renderer.h"
#include "Utils/Logging.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <array>
#include <format>
#include <string>

namespace Render
{
	namespace
	{
		constexpr std::string_view kVertexSource = R"(#version 150 core
in vec2 aPos;
in vec2 aUV;
in vec4 aColor;
out vec2 vUV;
out vec4 vColor;
uniform mat4 uProjection;
void main()
{
	vUV = aUV;
	vColor = aColor;
	gl_Position = uProjection * vec4(aPos, 0.0, 1.0);
}
)";

		constexpr std::string_view kFragmentSource = R"(#version 150 core
in vec2 vUV;
in vec4 vColor;
out vec4 FragColor;
uniform sampler2D uTexture;
void main()
{
	FragColor = texture(uTexture, vUV) * vColor;
}
)";

		void MakeOrtho(float left, float right, float bottom, float top, float* out)
		{
			for (int i = 0; i < 16; ++i)
			{
				out[i] = 0.0f;
			}

			out[0] = 2.0f / (right - left);
			out[5] = 2.0f / (top - bottom);
			out[10] = -1.0f;
			out[12] = -(right + left) / (right - left);
			out[13] = -(top + bottom) / (top - bottom);
			out[15] = 1.0f;
		}
	}

	Renderer::~Renderer()
	{
		Shutdown();
	}

	void Renderer::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		auto* self = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
		self->m_Width = width;
		self->m_Height = height;
		glViewport(0, 0, width, height);
	}

	bool Renderer::Init(int width, int height, std::string_view title)
	{
		if (glfwInit() == GLFW_FALSE)
		{
			LOG_ERROR("Renderer: glfwInit failed");
			return false;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

		m_Window = glfwCreateWindow(width, height, std::string(title).c_str(), nullptr, nullptr);
		if (m_Window == nullptr)
		{
			LOG_ERROR("Renderer: glfwCreateWindow failed");
			glfwTerminate();
			return false;
		}

		m_Width = width;
		m_Height = height;

		glfwSetWindowUserPointer(m_Window, this);
		glfwSetFramebufferSizeCallback(m_Window, FramebufferSizeCallback);
		glfwMakeContextCurrent(m_Window);
		glfwSwapInterval(1);

		if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
		{
			LOG_ERROR("Renderer: gladLoadGLLoader failed");
			Shutdown();
			return false;
		}

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (!m_Shader.Compile(kVertexSource, kFragmentSource))
		{
			Shutdown();
			return false;
		}

		std::array<uint8_t, 4> whitePixel = { 255, 255, 255, 255 };
		m_WhiteTexture.LoadFromMemory(1, 1, whitePixel.data());

		m_Vertices.resize(static_cast<size_t>(kMaxQuadsPerBatch) * 4);

		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);

		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, static_cast<long>(m_Vertices.size() * sizeof(QuadVertex)), nullptr, GL_DYNAMIC_DRAW);

		// set attribute pointers
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), reinterpret_cast<void*>(offsetof(QuadVertex, x)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), reinterpret_cast<void*>(offsetof(QuadVertex, u)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), reinterpret_cast<void*>(offsetof(QuadVertex, r)));

		std::vector<uint32_t> indices(static_cast<size_t>(kMaxQuadsPerBatch) * 6);
		for (uint32_t i = 0; i < kMaxQuadsPerBatch; ++i)
		{
			uint32_t base = i * 4;
			indices[i * 6 + 0] = base + 0;
			indices[i * 6 + 1] = base + 1;
			indices[i * 6 + 2] = base + 2;
			indices[i * 6 + 3] = base + 2;
			indices[i * 6 + 4] = base + 3;
			indices[i * 6 + 5] = base + 0;
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<long>(indices.size() * sizeof(uint32_t)), indices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);

		return true;
	}

	void Renderer::Shutdown()
	{
		if (m_VBO != 0)
		{
			glDeleteBuffers(1, &m_VBO);
			m_VBO = 0;
		}

		if (m_EBO != 0)
		{
			glDeleteBuffers(1, &m_EBO);
			m_EBO = 0;
		}

		if (m_VAO != 0)
		{
			glDeleteVertexArrays(1, &m_VAO);
			m_VAO = 0;
		}

		m_Shader.Destroy();
		m_WhiteTexture.Destroy();

		if (m_Window != nullptr)
		{
			glfwDestroyWindow(m_Window);
			m_Window = nullptr;
			glfwTerminate();
		}
	}

	bool Renderer::ShouldClose() const
	{
		return glfwWindowShouldClose(m_Window) != 0;
	}

	void Renderer::PollEvents() const
	{
		glfwPollEvents();
	}

	void Renderer::BeginFrame(const Color& clearColor, float cameraX, float cameraY, float cameraZoom)
	{
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		glClear(GL_COLOR_BUFFER_BIT);

		m_QuadCount = 0;
		m_BatchTextureID = 0;

		m_CameraX = cameraX;
		m_CameraY = cameraY;
		m_CameraZoom = cameraZoom;
	}

	void Renderer::EndFrame()
	{
		Flush();
		glfwSwapBuffers(m_Window);
	}

	void Renderer::PushQuad(float x, float y, float width, float height, float u0, float v0, float u1, float v1, const Color& color, uint32_t textureID)
	{
		PushQuad(x, y, width, height, u0, v0, u1, v1, color, color, textureID);
	}

	void Renderer::PushQuad(float x, float y, float width, float height, float u0, float v0, float u1, float v1, const Color& topColor, const Color& bottomColor, uint32_t textureID)
	{
		if (m_QuadCount > 0 && (m_QuadCount >= kMaxQuadsPerBatch || textureID != m_BatchTextureID))
		{
			Flush();
		}

		m_BatchTextureID = textureID;

		QuadVertex* vertex = &m_Vertices[static_cast<size_t>(m_QuadCount) * 4];
		vertex[0] = { x, y, u0, v0, topColor.r, topColor.g, topColor.b, topColor.a };
		vertex[1] = { x + width, y, u1, v0, topColor.r, topColor.g, topColor.b, topColor.a };
		vertex[2] = { x + width, y + height, u1, v1, bottomColor.r, bottomColor.g, bottomColor.b, bottomColor.a };
		vertex[3] = { x, y + height, u0, v1, bottomColor.r, bottomColor.g, bottomColor.b, bottomColor.a };

		++m_QuadCount;
	}

	void Renderer::DrawQuad(float x, float y, float width, float height, const Color& color)
	{
		PushQuad(x, y, width, height, 0.0f, 0.0f, 1.0f, 1.0f, color, m_WhiteTexture.GetID());
	}

	void Renderer::DrawQuad(float x, float y, float width, float height, const Color& topColor, const Color& bottomColor)
	{
		PushQuad(x, y, width, height, 0.0f, 0.0f, 1.0f, 1.0f, topColor, bottomColor, m_WhiteTexture.GetID());
	}

	void Renderer::DrawQuad(float x, float y, float width, float height, const GLTexture& texture, const Color& color)
	{
		PushQuad(x, y, width, height, 0.0f, 0.0f, 1.0f, 1.0f, color, texture.GetID());
	}

	void Renderer::DrawQuad(float x, float y, float width, float height, const GLTexture& texture, float u0, float v0, float u1, float v1, const Color& color)
	{
		PushQuad(x, y, width, height, u0, v0, u1, v1, color, texture.GetID());
	}

	void Renderer::Flush()
	{
		if (m_QuadCount == 0)
		{
			return;
		}

		glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<long>(static_cast<size_t>(m_QuadCount) * 4 * sizeof(QuadVertex)), m_Vertices.data());

		float left = m_CameraX;
		float right = m_CameraX + static_cast<float>(m_Width) / m_CameraZoom;
		float top = m_CameraY;
		float bottom = m_CameraY + static_cast<float>(m_Height) / m_CameraZoom;

		float projection[16];
		MakeOrtho(left, right, bottom, top, projection);

		m_Shader.Bind();
		m_Shader.SetMat4("uProjection", projection);
		m_Shader.SetInt("uTexture", 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_BatchTextureID);

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glDrawElements(GL_TRIANGLES, static_cast<int>(m_QuadCount * 6), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

		m_QuadCount = 0;
	}
}

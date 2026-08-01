/*
 *	Copyright 2026-2027 static - hiro
 *
 *	Use of this source code without explicit permission from owner is strictly prohibited.
 */

#pragma once

#include "Render/OpenGL/GLShader.h"
#include "Render/OpenGL/GLTexture.h"

#include <cstdint>
#include <string_view>
#include <vector>

struct GLFWwindow;

namespace Render
{
	struct Color
	{
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
		float a = 1.0f;
	};

	class Renderer
	{
	public:
		Renderer() = default;
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		bool Init(int width, int height, std::string_view title);
		void Shutdown();

		bool ShouldClose() const;
		void PollEvents() const;

		void BeginFrame(const Color& clearColor);
		void EndFrame();

		void DrawQuad(float x, float y, float width, float height, const Color& color);
		void DrawQuad(float x, float y, float width, float height, const GLTexture& texture, const Color& color = Color{ 1.0f, 1.0f, 1.0f, 1.0f });

		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
		GLFWwindow* GetWindow() const { return m_Window; }

	private:
		struct QuadVertex
		{
			float x, y;
			float u, v;
			float r, g, b, a;
		};

		static constexpr uint32_t kMaxQuadsPerBatch = 10000;

		static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

		void Flush();
		void PushQuad(float x, float y, float width, float height, float u0, float v0, float u1, float v1, const Color& color, uint32_t textureID);

		GLFWwindow* m_Window = nullptr;
		int m_Width = 0;
		int m_Height = 0;

		GLShader m_Shader;
		GLTexture m_WhiteTexture;

		uint32_t m_VAO = 0;
		uint32_t m_VBO = 0;
		uint32_t m_EBO = 0;

		std::vector<QuadVertex> m_Vertices;
		uint32_t m_QuadCount = 0;
		uint32_t m_BatchTextureID = 0;
	};
}

/*
 *	Copyright 2026-2027 static - hiro
 *
 *	Use of this source code without explicit permission from owner is strictly prohibited.
 */

#include "Render/OpenGL/GLShader.h"
#include "Utils/Logging.h"

#include <glad/glad.h>

#include <format>
#include <string>
#include <vector>

namespace Render
{
	namespace
	{
		uint32_t CompileStage(GLenum stage, std::string_view source)
		{
			uint32_t handle = glCreateShader(stage);
			const char* sourcePtr = source.data();
			int sourceLength = static_cast<int>(source.size());
			glShaderSource(handle, 1, &sourcePtr, &sourceLength);
			glCompileShader(handle);

			int success = 0;
			glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				int logLength = 0;
				glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logLength);
				std::vector<char> log(static_cast<size_t>(logLength) + 1);
				glGetShaderInfoLog(handle, logLength, nullptr, log.data());
				LOG_ERROR(std::format("GLShader: stage compile failed: {}", log.data()));
				glDeleteShader(handle);
				return 0;
			}

			return handle;
		}
	}

	GLShader::~GLShader()
	{
		Destroy();
	}

	GLShader::GLShader(GLShader&& other) noexcept
		: m_ID(other.m_ID)
	{
		other.m_ID = 0;
	}

	GLShader& GLShader::operator=(GLShader&& other) noexcept
	{
		if (this != &other)
		{
			Destroy();
			m_ID = other.m_ID;
			other.m_ID = 0;
		}

		return *this;
	}

	bool GLShader::Compile(std::string_view vertexSource, std::string_view fragmentSource)
	{
		// compiles the shader
		uint32_t vertex = CompileStage(GL_VERTEX_SHADER, vertexSource);
		if (vertex == 0)
		{
			return false;
		}

		uint32_t fragment = CompileStage(GL_FRAGMENT_SHADER, fragmentSource);
		if (fragment == 0)
		{
			glDeleteShader(vertex);
			return false;
		}

		uint32_t program = glCreateProgram();
		glAttachShader(program, vertex);
		glAttachShader(program, fragment);
		glBindAttribLocation(program, 0, "aPos");
		glBindAttribLocation(program, 1, "aUV");
		glBindAttribLocation(program, 2, "aColor");
		glLinkProgram(program);

		glDeleteShader(vertex);
		glDeleteShader(fragment);

		int success = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			int logLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
			std::vector<char> log(static_cast<size_t>(logLength) + 1);
			glGetProgramInfoLog(program, logLength, nullptr, log.data());
			LOG_ERROR(std::format("GLShader: link failed: {}", log.data()));
			glDeleteProgram(program);
			return false;
		}

		Destroy();
		m_ID = program;
		return true;
	}

	void GLShader::Destroy()
	{
		// kills the shader
		if (m_ID != 0)
		{
			glDeleteProgram(m_ID);
			m_ID = 0;
		}
	}

	void GLShader::Bind() const
	{
		// binds shader
		glUseProgram(m_ID);
	}

	void GLShader::Unbind() const
	{
		// unbinds shader
		glUseProgram(0);
	}

	int GLShader::GetUniformLocation(std::string_view name) const
	{
		// grabs the loc
		return glGetUniformLocation(m_ID, std::string(name).c_str());
	}

	void GLShader::SetInt(std::string_view name, int value) const
	{
		// allows u to set the value of an int in the shader
		glUniform1i(GetUniformLocation(name), value);
	}

	void GLShader::SetFloat4(std::string_view name, float x, float y, float z, float w) const
	{
		// allows u to set the value of a float in the shader
		glUniform4f(GetUniformLocation(name), x, y, z, w);
	}

	void GLShader::SetMat4(std::string_view name, const float* matrix) const
	{
		// allwos u to set the shader
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, matrix);
	}
}

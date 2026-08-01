/*
 *	Copyright 2026-2027 static - hiro
 *
 *	Use of this source code without explicit permission from owner is strictly prohibited.
 */

#pragma once

#include <cstdint>
#include <string_view>

namespace Render
{
	class GLShader
	{
	public:
		GLShader() = default;
		~GLShader();

		GLShader(const GLShader&) = delete;
		GLShader& operator=(const GLShader&) = delete;

		GLShader(GLShader&& other) noexcept;
		GLShader& operator=(GLShader&& other) noexcept;

		bool Compile(std::string_view vertexSource, std::string_view fragmentSource);
		void Destroy();

		void Bind() const;
		void Unbind() const;

		void SetInt(std::string_view name, int value) const;
		void SetFloat4(std::string_view name, float x, float y, float z, float w) const;
		void SetMat4(std::string_view name, const float* matrix) const;

		uint32_t GetID() const { return m_ID; }

	private:
		int GetUniformLocation(std::string_view name) const;

		uint32_t m_ID = 0;
	};
}

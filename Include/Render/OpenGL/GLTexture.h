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
	class GLTexture
	{
	public:
		GLTexture() = default;
		~GLTexture();

		GLTexture(const GLTexture&) = delete;
		GLTexture& operator=(const GLTexture&) = delete;

		GLTexture(GLTexture&& other) noexcept;
		GLTexture& operator=(GLTexture&& other) noexcept;

		bool LoadFromFile(std::string_view path);
		bool LoadFromMemory(int width, int height, const uint8_t* pixelsRGBA);
		void Destroy();

		void Bind(uint32_t slot = 0) const;
		void Unbind() const;

		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
		uint32_t GetID() const { return m_ID; }

	private:
		uint32_t m_ID = 0;
		int m_Width = 0;
		int m_Height = 0;
	};
}

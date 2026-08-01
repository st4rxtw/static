/*
 *	Copyright 2026-2027 static - hiro
 *
 *	Use of this source code without explicit permission from owner is strictly prohibited.
 */

#include "Render/OpenGL/GLTexture.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <cstdio>
#include <string>

namespace Render
{
	GLTexture::~GLTexture()
	{
		Destroy();
	}

	GLTexture::GLTexture(GLTexture&& other) noexcept
		: m_ID(other.m_ID)
		, m_Width(other.m_Width)
		, m_Height(other.m_Height)
	{
		other.m_ID = 0;
		other.m_Width = 0;
		other.m_Height = 0;
	}

	GLTexture& GLTexture::operator=(GLTexture&& other) noexcept
	{
		if (this != &other)
		{
			Destroy();
			m_ID = other.m_ID;
			m_Width = other.m_Width;
			m_Height = other.m_Height;
			other.m_ID = 0;
			other.m_Width = 0;
			other.m_Height = 0;
		}

		return *this;
	}

	bool GLTexture::LoadFromFile(std::string_view path)
	{
		// load texture from a file
		stbi_set_flip_vertically_on_load(1);

		int width = 0;
		int height = 0;
		int channels = 0;
		stbi_uc* pixels = stbi_load(std::string(path).c_str(), &width, &height, &channels, 4);
		if (pixels == nullptr)
		{
			std::fprintf(stderr, "GLTexture: failed to load \"%s\": %s\n", std::string(path).c_str(), stbi_failure_reason());
			return false;
		}

		bool loaded = LoadFromMemory(width, height, pixels);
		stbi_image_free(pixels);
		return loaded;
	}

	bool GLTexture::LoadFromMemory(int width, int height, const uint8_t* pixelsRGBA)
	{
		// load a texture from mem
		Destroy();

		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelsRGBA);

		glBindTexture(GL_TEXTURE_2D, 0);

		m_Width = width;
		m_Height = height;
		return true;
	}

	void GLTexture::Destroy()
	{
		// kill the texture
		if (m_ID != 0)
		{
			glDeleteTextures(1, &m_ID);
			m_ID = 0;
		}

		m_Width = 0;
		m_Height = 0;
	}

	void GLTexture::Bind(uint32_t slot) const
	{
		// bind the teexture
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}

	void GLTexture::Unbind() const
	{
		// unbind tex
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

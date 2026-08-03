/*
 *	Copyright 2026-2027 static - hiro
 *
 *	Use of this source code without explicit permission from owner is strictly prohibited.
 */

#include "Base/Text.h"
#include "Utils/Logging.h"
#include "Utils/Paths.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#include <format>
#include <fstream>
#include <vector>

namespace Base
{
	bool Font::LoadFromFile(std::string_view path, float pixelHeight)
	{
		// loads the ttf font
		Destroy();

		std::string resolvedPath = Utils::Paths::Resolve(path);

		std::ifstream file(resolvedPath, std::ios::binary);
		if (!file)
		{
			LOG_ERROR(std::format("Font: failed to open \"{}\"", resolvedPath));
			return false;
		}

		std::vector<unsigned char> fileData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

		constexpr int kAtlasSize = 512;
		std::vector<unsigned char> atlasBitmap(static_cast<size_t>(kAtlasSize) * kAtlasSize);

		std::array<stbtt_bakedchar, kCharCount> bakedChars{};
		int result = stbtt_BakeFontBitmap(fileData.data(), 0, pixelHeight, atlasBitmap.data(), kAtlasSize, kAtlasSize, kFirstChar, kCharCount, bakedChars.data());
		if (result <= 0)
		{
			LOG_ERROR(std::format("Font: failed to bake \"{}\"", resolvedPath));
			return false;
		}

		std::vector<uint8_t> rgba(static_cast<size_t>(kAtlasSize) * kAtlasSize * 4);
		for (size_t i = 0; i < atlasBitmap.size(); ++i)
		{
			rgba[i * 4 + 0] = 255;
			rgba[i * 4 + 1] = 255;
			rgba[i * 4 + 2] = 255;
			rgba[i * 4 + 3] = atlasBitmap[i] >= 128 ? 255 : 0;
		}

		m_Atlas.LoadFromMemory(kAtlasSize, kAtlasSize, rgba.data());

		stbtt_fontinfo info;
		stbtt_InitFont(&info, fileData.data(), 0);

		int ascent = 0;
		stbtt_GetFontVMetrics(&info, &ascent, nullptr, nullptr);
		m_Ascent = static_cast<float>(ascent) * stbtt_ScaleForPixelHeight(&info, pixelHeight);

		for (int i = 0; i < kCharCount; ++i)
		{
			const stbtt_bakedchar& baked = bakedChars[static_cast<size_t>(i)];
			Glyph& glyph = m_Glyphs[static_cast<size_t>(i)];
			glyph.u0 = static_cast<float>(baked.x0) / kAtlasSize;
			glyph.v0 = static_cast<float>(baked.y0) / kAtlasSize;
			glyph.u1 = static_cast<float>(baked.x1) / kAtlasSize;
			glyph.v1 = static_cast<float>(baked.y1) / kAtlasSize;
			glyph.width = static_cast<float>(baked.x1 - baked.x0);
			glyph.height = static_cast<float>(baked.y1 - baked.y0);
			glyph.xoff = baked.xoff;
			glyph.yoff = baked.yoff;
			glyph.xadvance = baked.xadvance;
		}

		return true;
	}

	void Font::Destroy()
	{
		// kills the font
		m_Atlas.Destroy();
		m_Glyphs.fill(Glyph{});
	}

	float Text::GetWidth() const
	{
		if (m_Font == nullptr)
		{
			return 0.0f;
		}

		float width = 0.0f;
		for (char c : m_Text)
		{
			auto uc = static_cast<unsigned char>(c);
			if (!Font::HasGlyph(uc))
			{
				continue;
			}

			width += m_Font->GetGlyph(uc).xadvance;
		}

		return width * m_ScaleX;
	}

	void Text::Draw(Render::Renderer& renderer)
	{
		// renders the text
		if (m_Font == nullptr)
		{
			return;
		}

		float penX = m_X;
		float penY = m_Y + m_Font->GetAscent() * m_ScaleY;

		for (char c : m_Text)
		{
			auto uc = static_cast<unsigned char>(c);
			if (!Font::HasGlyph(uc))
			{
				continue;
			}

			const Font::Glyph& glyph = m_Font->GetGlyph(uc);
			renderer.DrawQuad(penX + glyph.xoff * m_ScaleX, penY + glyph.yoff * m_ScaleY, glyph.width * m_ScaleX, glyph.height * m_ScaleY, m_Font->GetAtlas(), glyph.u0, glyph.v0, glyph.u1, glyph.v1, m_Color);

			penX += glyph.xadvance * m_ScaleX;
		}
	}
}

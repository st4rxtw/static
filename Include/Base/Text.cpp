/*
 *	Copyright 2026-2027 static - hiro
 *
 *	Use of this source code without explicit permission from owner is strictly prohibited.
 */

#include "Base/Text.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#include <cstdio>
#include <fstream>
#include <vector>

namespace Base
{
	bool Font::LoadFromFile(std::string_view path, float pixelHeight)
	{
		// loads the ttf font
		Destroy();

		std::ifstream file(std::string(path), std::ios::binary);
		if (!file)
		{
			std::fprintf(stderr, "Font: failed to open \"%s\"\n", std::string(path).c_str());
			return false;
		}

		std::vector<unsigned char> fileData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

		constexpr int kAtlasSize = 512;
		std::vector<unsigned char> atlasBitmap(static_cast<size_t>(kAtlasSize) * kAtlasSize);

		std::array<stbtt_bakedchar, kCharCount> bakedChars{};
		int result = stbtt_BakeFontBitmap(fileData.data(), 0, pixelHeight, atlasBitmap.data(), kAtlasSize, kAtlasSize, kFirstChar, kCharCount, bakedChars.data());
		if (result <= 0)
		{
			std::fprintf(stderr, "Font: failed to bake \"%s\"\n", std::string(path).c_str());
			return false;
		}

		std::vector<uint8_t> rgba(static_cast<size_t>(kAtlasSize) * kAtlasSize * 4);
		for (size_t i = 0; i < atlasBitmap.size(); ++i)
		{
			rgba[i * 4 + 0] = 255;
			rgba[i * 4 + 1] = 255;
			rgba[i * 4 + 2] = 255;
			rgba[i * 4 + 3] = atlasBitmap[i];
		}

		m_Atlas.LoadFromMemory(kAtlasSize, kAtlasSize, rgba.data());

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

	void Text::Draw(Render::Renderer& renderer)
	{
		// renders the text
		if (m_Font == nullptr)
		{
			return;
		}

		float penX = m_X;
		float penY = m_Y;

		for (char c : m_Text)
		{
			auto uc = static_cast<unsigned char>(c);
			if (!Font::HasGlyph(uc))
			{
				continue;
			}

			const Font::Glyph& glyph = m_Font->GetGlyph(uc);
			renderer.DrawQuad(penX + glyph.xoff, penY + glyph.yoff, glyph.width, glyph.height, m_Font->GetAtlas(), glyph.u0, glyph.v0, glyph.u1, glyph.v1, m_Color);

			penX += glyph.xadvance;
		}
	}
}

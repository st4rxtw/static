/*
 *	Copyright 2026-2027 static - hiro
 *
 *	Use of this source code without explicit permission from owner is strictly prohibited.
 */

#pragma once

#include "Base/GameObject.h"
#include "Render/OpenGL/GLTexture.h"
#include "Render/Renderer.h"

#include <array>
#include <string>
#include <string_view>

namespace Base
{
	class Font
	{
	public:
		static constexpr int kFirstChar = 32;
		static constexpr int kCharCount = 96;

		struct Glyph
		{
			float u0, v0, u1, v1;
			float width, height;
			float xoff, yoff;
			float xadvance;
		};

		Font() = default;

		Font(const Font&) = delete;
		Font& operator=(const Font&) = delete;

		bool LoadFromFile(std::string_view path, float pixelHeight);
		void Destroy();

		const Render::GLTexture& GetAtlas() const { return m_Atlas; }
		const Glyph& GetGlyph(unsigned char c) const { return m_Glyphs[c - kFirstChar]; }
		static bool HasGlyph(unsigned char c) { return c >= kFirstChar && c < kFirstChar + kCharCount; }

	private:
		Render::GLTexture m_Atlas;
		std::array<Glyph, kCharCount> m_Glyphs{};
	};

	class Text : public GameObject
	{
	public:
		Text() = default;

		void SetFont(const Font* font) { m_Font = font; }
		void SetString(std::string text) { m_Text = std::move(text); }
		void SetColor(const Render::Color& color) { m_Color = color; }
		void SetScale(float x, float y) { m_ScaleX = x; m_ScaleY = y; }

		const std::string& GetString() const { return m_Text; }
		float GetScaleX() const { return m_ScaleX; }
		float GetScaleY() const { return m_ScaleY; }
		float GetWidth() const;

		float* GetScaleXPtr() { return &m_ScaleX; }
		float* GetScaleYPtr() { return &m_ScaleY; }
		Render::Color& GetColorRef() { return m_Color; }

		void Draw(Render::Renderer& renderer) override;

	private:
		const Font* m_Font = nullptr;
		std::string m_Text;
		Render::Color m_Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float m_ScaleX = 1.0f;
		float m_ScaleY = 1.0f;
	};
}


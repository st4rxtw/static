/*
 *	Copyright 2026-2027 static - hiro
 *
 *	Use of this source code without explicit permission from owner is strictly prohibited.
 */

#pragma once

#include "Base/GameObject.h"
#include "Render/Renderer.h"

namespace Base
{
	class Sprite : public GameObject
	{
	public:
		Sprite() = default;

		// sets the sprite texture
		void SetTexture(const Render::GLTexture* texture) { m_Texture = texture; }
		// sets the sprite size
		void SetSize(float width, float height) { m_Width = width; m_Height = height; }
		// sets the sprites color
		void SetColor(const Render::Color& color) { m_Color = color; }
		void SetScale(float x, float y) { m_ScaleX = x; m_ScaleY = y; }

		float GetWidth() const { return m_Width; }
		float GetHeight() const { return m_Height; }
		float GetScaleX() const { return m_ScaleX; }
		float GetScaleY() const { return m_ScaleY; }

		float* GetWidthPtr() { return &m_Width; }
		float* GetHeightPtr() { return &m_Height; }
		float* GetScaleXPtr() { return &m_ScaleX; }
		float* GetScaleYPtr() { return &m_ScaleY; }
		Render::Color& GetColorRef() { return m_Color; }

		void Draw(Render::Renderer& renderer) override
		{
			// renders the sprite
			float width = m_Width * m_ScaleX;
			float height = m_Height * m_ScaleY;

			if (m_Texture != nullptr)
			{
				renderer.DrawQuad(m_X, m_Y, width, height, *m_Texture, m_Color);
			}
			else
			{
				renderer.DrawQuad(m_X, m_Y, width, height, m_Color);
			}
		}

	private:
		const Render::GLTexture* m_Texture = nullptr;
		float m_Width = 32.0f;
		float m_Height = 32.0f;
		float m_ScaleX = 1.0f;
		float m_ScaleY = 1.0f;
		Render::Color m_Color{ 1.0f, 1.0f, 1.0f, 1.0f };
	};
}


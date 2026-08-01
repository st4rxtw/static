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

		void Draw(Render::Renderer& renderer) override
		{
			// renders the sprite
			if (m_Texture != nullptr)
			{
				renderer.DrawQuad(m_X, m_Y, m_Width, m_Height, *m_Texture, m_Color);
			}
			else
			{
				renderer.DrawQuad(m_X, m_Y, m_Width, m_Height, m_Color);
			}
		}

	private:
		const Render::GLTexture* m_Texture = nullptr;
		float m_Width = 32.0f;
		float m_Height = 32.0f;
		Render::Color m_Color{ 1.0f, 1.0f, 1.0f, 1.0f };
	};
}


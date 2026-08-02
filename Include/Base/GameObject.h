/*
 *	Copyright 2026-2027 static - hiro
 *
 *	Use of this source code without explicit permission from owner is strictly prohibited.
 */

#pragma once

namespace Render
{
	class Renderer;
}

namespace Base
{
	class GameObject
	{
	public:
		GameObject() = default;
		virtual ~GameObject();

		virtual void Update(float deltaTime);
		virtual void Draw(Render::Renderer& renderer);

		void SetPosition(float x, float y) { m_X = x; m_Y = y; }
		void Move(float dx, float dy) { m_X += dx; m_Y += dy; }

		float GetX() const { return m_X; }
		float GetY() const { return m_Y; }

		float* GetXPtr() { return &m_X; }
		float* GetYPtr() { return &m_Y; }

	protected:
		float m_X = 0.0f;
		float m_Y = 0.0f;
	};
}


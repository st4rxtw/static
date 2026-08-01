/*
 *	Copyright 2026-2027 static - hiro
 *
 *	Use of this source code without explicit permission from owner is strictly prohibited.
 */

#pragma once

namespace Base
{
	class Camera
	{
	public:
		Camera() = default;

		void SetPosition(float x, float y) { m_X = x; m_Y = y; }
		void Move(float dx, float dy) { m_X += dx; m_Y += dy; }
		void SetZoom(float zoom) { m_Zoom = zoom; }

		float GetX() const { return m_X; }
		float GetY() const { return m_Y; }
		float GetZoom() const { return m_Zoom; }

	private:
		float m_X = 0.0f;
		float m_Y = 0.0f;
		float m_Zoom = 1.0f;
	};
}


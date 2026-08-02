/*
 *	Copyright 2026-2027 static - hiro
 *
 *	Use of this source code without explicit permission from owner is strictly prohibited.
 */

#pragma once

#include <array>

struct GLFWwindow;

namespace Base
{
	class EventHandler
	{
	public:
		EventHandler() = default;

		void Init(GLFWwindow* window);
		void Update();

		bool IsKeyDown(int key) const;
		bool IsKeyPressed(int key) const;
		bool IsKeyReleased(int key) const;

		bool IsMouseButtonDown(int button) const;
		bool IsMouseButtonPressed(int button) const;

		float GetMouseX() const { return m_MouseX; }
		float GetMouseY() const { return m_MouseY; }

	private:
		static constexpr int kKeyCount = 349;
		static constexpr int kMouseButtonCount = 8;

		GLFWwindow* m_Window = nullptr;

		std::array<bool, kKeyCount> m_CurrentKeys{};
		std::array<bool, kKeyCount> m_PreviousKeys{};

		std::array<bool, kMouseButtonCount> m_CurrentMouseButtons{};
		std::array<bool, kMouseButtonCount> m_PreviousMouseButtons{};

		float m_MouseX = 0.0f;
		float m_MouseY = 0.0f;
	};
}


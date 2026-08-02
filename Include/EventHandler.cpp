/*
 *	Copyright 2026-2027 static - hiro
 *
 *	Use of this source code without explicit permission from owner is strictly prohibited.
 */

#include "EventHandler.h"

#include <GLFW/glfw3.h>

namespace Base
{
	void EventHandler::Init(GLFWwindow* window)
	{
		m_Window = window;
	}

	void EventHandler::Update()
	{
		m_PreviousKeys = m_CurrentKeys;

		for (int key = 0; key < kKeyCount; ++key)
		{
			m_CurrentKeys[static_cast<size_t>(key)] = glfwGetKey(m_Window, key) == GLFW_PRESS;
		}

		m_PreviousMouseButtons = m_CurrentMouseButtons;

		for (int button = 0; button < kMouseButtonCount; ++button)
		{
			m_CurrentMouseButtons[static_cast<size_t>(button)] = glfwGetMouseButton(m_Window, button) == GLFW_PRESS;
		}

		double x = 0.0;
		double y = 0.0;
		glfwGetCursorPos(m_Window, &x, &y);
		m_MouseX = static_cast<float>(x);
		m_MouseY = static_cast<float>(y);
	}

	bool EventHandler::IsKeyDown(int key) const
	{
		return key >= 0 && key < kKeyCount && m_CurrentKeys[static_cast<size_t>(key)];
	}

	bool EventHandler::IsKeyPressed(int key) const
	{
		return key >= 0 && key < kKeyCount && m_CurrentKeys[static_cast<size_t>(key)] && !m_PreviousKeys[static_cast<size_t>(key)];
	}

	bool EventHandler::IsKeyReleased(int key) const
	{
		return key >= 0 && key < kKeyCount && !m_CurrentKeys[static_cast<size_t>(key)] && m_PreviousKeys[static_cast<size_t>(key)];
	}

	bool EventHandler::IsMouseButtonDown(int button) const
	{
		return glfwGetMouseButton(m_Window, button) == GLFW_PRESS;
	}

	bool EventHandler::IsMouseButtonPressed(int button) const
	{
		return button >= 0 && button < kMouseButtonCount && m_CurrentMouseButtons[static_cast<size_t>(button)] && !m_PreviousMouseButtons[static_cast<size_t>(button)];
	}
}

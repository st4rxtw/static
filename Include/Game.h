/*
 *	Copyright 2026-2027 static - hiro
 *
 *	Use of this source code without explicit permission from owner is strictly prohibited.
 */

#pragma once

#include "Base/Camera.h"
#include "EventHandler.h"
#include "Render/Renderer.h"

#include <string>
#include <string_view>

namespace Base
{
	class Game
	{
	public:
		Game(std::string_view title, int width = 1280, int height = 720);
		virtual ~Game() = default;

		static Game* Instance;

		std::string title;

		float fps = 0.0f;
		float fpsCap = 240.0f;

		Render::Renderer renderer;
		EventHandler events;
		Camera camera;

		virtual void update(float deltaTime);
	};
}

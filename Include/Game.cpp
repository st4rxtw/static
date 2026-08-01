/*
 *	Copyright 2026-2027 static - hiro
 *
 *	Use of this source code without explicit permission from owner is strictly prohibited.
 */

#include "Game.h"
#include "Utils/Logging.h"

#include <format>

namespace Base
{
	Game* Game::Instance = nullptr;

	Game::Game(std::string_view title, int width, int height)
		: title(title)
	{
		if (!renderer.Init(width, height, title))
		{
			LOG_ERROR("Failed to create window");
			return;
		}

		events.Init(renderer.GetWindow());

		Instance = this;

		LOG_INFO(std::format("Game created", this->title));
	}

	void Game::update(float deltaTime)
	{
		(void)deltaTime;
	}
}

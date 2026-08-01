/*
 *	Copyright 2026-2027 static - hiro
 *
 *	Use of this source code without explicit permission from owner is strictly prohibited.
 */

#pragma once

#include "Utils/Easing.h"

#include <functional>
#include <vector>

namespace Utils
{
	class TweenManager
	{
	public:
		using CompletionCallback = std::function<void()>;

		void Tween(float* target, float from, float to, float duration, EasingType easing = EasingType::Linear, CompletionCallback onComplete = nullptr);
		void Update(float deltaTime);
		void Clear();

	private:
		struct Entry
		{
			float* target;
			float from;
			float to;
			float duration;
			float elapsed;
			EasingType easing;
			CompletionCallback onComplete;
		};

		std::vector<Entry> m_Tweens;
	};
}


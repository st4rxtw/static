/*
 *	Copyright 2026-2027 static - hiro
 *
 *	Use of this source code without explicit permission from owner is strictly prohibited.
 */

#include "Utils/TweenManager.h"

namespace Utils
{
	void TweenManager::Tween(float* target, float from, float to, float duration, EasingType easing, CompletionCallback onComplete)
	{
		Entry entry;
		entry.target = target;
		entry.from = from;
		entry.to = to;
		entry.duration = duration;
		entry.elapsed = 0.0f;
		entry.easing = easing;
		entry.onComplete = std::move(onComplete);

		*target = from;

		m_Tweens.push_back(std::move(entry));
	}

	void TweenManager::Update(float deltaTime)
	{
		for (size_t i = 0; i < m_Tweens.size();)
		{
			Entry& entry = m_Tweens[i];
			entry.elapsed += deltaTime;

			float t = entry.duration > 0.0f ? entry.elapsed / entry.duration : 1.0f;
			if (t > 1.0f)
			{
				t = 1.0f;
			}

			*entry.target = entry.from + (entry.to - entry.from) * Easing::Evaluate(entry.easing, t);

			if (t >= 1.0f)
			{
				CompletionCallback onComplete = std::move(entry.onComplete);

				m_Tweens[i] = std::move(m_Tweens.back());
				m_Tweens.pop_back();

				if (onComplete)
				{
					onComplete();
				}

				continue;
			}

			++i;
		}
	}

	void TweenManager::Clear()
	{
		m_Tweens.clear();
	}
}

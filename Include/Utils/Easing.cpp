/*
 *	Copyright 2026-2027 static - hiro
 *
 *	Use of this source code without explicit permission from owner is strictly prohibited.
 */

#include "Utils/Easing.h"

#include <cmath>
#include <numbers>

namespace Utils
{
	namespace
	{
		constexpr float kPi = std::numbers::pi_v<float>;

		float BounceOutImpl(float t)
		{
			constexpr float n1 = 7.5625f;
			constexpr float d1 = 2.75f;

			if (t < 1.0f / d1)
			{
				return n1 * t * t;
			}
			else if (t < 2.0f / d1)
			{
				t -= 1.5f / d1;
				return n1 * t * t + 0.75f;
			}
			else if (t < 2.5f / d1)
			{
				t -= 2.25f / d1;
				return n1 * t * t + 0.9375f;
			}
			else
			{
				t -= 2.625f / d1;
				return n1 * t * t + 0.984375f;
			}
		}
	}

	float Easing::Linear(float t)
	{
		return t;
	}

	float Easing::QuadIn(float t)
	{
		return t * t;
	}

	float Easing::QuadOut(float t)
	{
		return 1.0f - (1.0f - t) * (1.0f - t);
	}

	float Easing::QuadInOut(float t)
	{
		return t < 0.5f ? 2.0f * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
	}

	float Easing::CubicIn(float t)
	{
		return t * t * t;
	}

	float Easing::CubicOut(float t)
	{
		return 1.0f - std::pow(1.0f - t, 3.0f);
	}

	float Easing::CubicInOut(float t)
	{
		return t < 0.5f ? 4.0f * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 3.0f) / 2.0f;
	}

	float Easing::QuarticIn(float t)
	{
		return t * t * t * t;
	}

	float Easing::QuarticOut(float t)
	{
		return 1.0f - std::pow(1.0f - t, 4.0f);
	}

	float Easing::QuarticInOut(float t)
	{
		return t < 0.5f ? 8.0f * t * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 4.0f) / 2.0f;
	}

	float Easing::SineIn(float t)
	{
		return 1.0f - std::cos(t * kPi / 2.0f);
	}

	float Easing::SineOut(float t)
	{
		return std::sin(t * kPi / 2.0f);
	}

	float Easing::SineInOut(float t)
	{
		return -(std::cos(kPi * t) - 1.0f) / 2.0f;
	}

	float Easing::ExpoIn(float t)
	{
		return t <= 0.0f ? 0.0f : std::pow(2.0f, 10.0f * t - 10.0f);
	}

	float Easing::ExpoOut(float t)
	{
		return t >= 1.0f ? 1.0f : 1.0f - std::pow(2.0f, -10.0f * t);
	}

	float Easing::ExpoInOut(float t)
	{
		if (t <= 0.0f)
		{
			return 0.0f;
		}

		if (t >= 1.0f)
		{
			return 1.0f;
		}

		return t < 0.5f ? std::pow(2.0f, 20.0f * t - 10.0f) / 2.0f : (2.0f - std::pow(2.0f, -20.0f * t + 10.0f)) / 2.0f;
	}

	float Easing::BackIn(float t)
	{
		constexpr float c1 = 1.70158f;
		constexpr float c3 = c1 + 1.0f;

		return c3 * t * t * t - c1 * t * t;
	}

	float Easing::BackOut(float t)
	{
		constexpr float c1 = 1.70158f;
		constexpr float c3 = c1 + 1.0f;

		return 1.0f + c3 * std::pow(t - 1.0f, 3.0f) + c1 * std::pow(t - 1.0f, 2.0f);
	}

	float Easing::BackInOut(float t)
	{
		constexpr float c1 = 1.70158f;
		constexpr float c2 = c1 * 1.525f;

		return t < 0.5f
			? (std::pow(2.0f * t, 2.0f) * ((c2 + 1.0f) * 2.0f * t - c2)) / 2.0f
			: (std::pow(2.0f * t - 2.0f, 2.0f) * ((c2 + 1.0f) * (t * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
	}

	float Easing::ElasticIn(float t)
	{
		constexpr float c4 = (2.0f * kPi) / 3.0f;

		if (t <= 0.0f)
		{
			return 0.0f;
		}

		if (t >= 1.0f)
		{
			return 1.0f;
		}

		return -std::pow(2.0f, 10.0f * t - 10.0f) * std::sin((t * 10.0f - 10.75f) * c4);
	}

	float Easing::ElasticOut(float t)
	{
		constexpr float c4 = (2.0f * kPi) / 3.0f;

		if (t <= 0.0f)
		{
			return 0.0f;
		}

		if (t >= 1.0f)
		{
			return 1.0f;
		}

		return std::pow(2.0f, -10.0f * t) * std::sin((t * 10.0f - 0.75f) * c4) + 1.0f;
	}

	float Easing::ElasticInOut(float t)
	{
		constexpr float c5 = (2.0f * kPi) / 4.5f;

		if (t <= 0.0f)
		{
			return 0.0f;
		}

		if (t >= 1.0f)
		{
			return 1.0f;
		}

		return t < 0.5f
			? -(std::pow(2.0f, 20.0f * t - 10.0f) * std::sin((20.0f * t - 11.125f) * c5)) / 2.0f
			: (std::pow(2.0f, -20.0f * t + 10.0f) * std::sin((20.0f * t - 11.125f) * c5)) / 2.0f + 1.0f;
	}

	float Easing::BounceIn(float t)
	{
		return 1.0f - BounceOutImpl(1.0f - t);
	}

	float Easing::BounceOut(float t)
	{
		return BounceOutImpl(t);
	}

	float Easing::BounceInOut(float t)
	{
		return t < 0.5f
			? (1.0f - BounceOutImpl(1.0f - 2.0f * t)) / 2.0f
			: (1.0f + BounceOutImpl(2.0f * t - 1.0f)) / 2.0f;
	}

	float Easing::Evaluate(EasingType type, float t)
	{
		switch (type)
		{
			case EasingType::Linear: return Linear(t);

			case EasingType::QuadIn: return QuadIn(t);
			case EasingType::QuadOut: return QuadOut(t);
			case EasingType::QuadInOut: return QuadInOut(t);

			case EasingType::CubicIn: return CubicIn(t);
			case EasingType::CubicOut: return CubicOut(t);
			case EasingType::CubicInOut: return CubicInOut(t);

			case EasingType::QuarticIn: return QuarticIn(t);
			case EasingType::QuarticOut: return QuarticOut(t);
			case EasingType::QuarticInOut: return QuarticInOut(t);

			case EasingType::SineIn: return SineIn(t);
			case EasingType::SineOut: return SineOut(t);
			case EasingType::SineInOut: return SineInOut(t);

			case EasingType::ExpoIn: return ExpoIn(t);
			case EasingType::ExpoOut: return ExpoOut(t);
			case EasingType::ExpoInOut: return ExpoInOut(t);

			case EasingType::BackIn: return BackIn(t);
			case EasingType::BackOut: return BackOut(t);
			case EasingType::BackInOut: return BackInOut(t);

			case EasingType::ElasticIn: return ElasticIn(t);
			case EasingType::ElasticOut: return ElasticOut(t);
			case EasingType::ElasticInOut: return ElasticInOut(t);

			case EasingType::BounceIn: return BounceIn(t);
			case EasingType::BounceOut: return BounceOut(t);
			case EasingType::BounceInOut: return BounceInOut(t);
		}

		return t;
	}
}


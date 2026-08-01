/*
 *	Copyright 2026-2027 static - hiro
 *
 *	Use of this source code without explicit permission from owner is strictly prohibited.
 */

#pragma once

namespace Utils
{
	enum class EasingType
	{
		Linear,
		QuadIn, QuadOut, QuadInOut,
		CubicIn, CubicOut, CubicInOut,
		QuarticIn, QuarticOut, QuarticInOut,
		SineIn, SineOut, SineInOut,
		ExpoIn, ExpoOut, ExpoInOut,
		BackIn, BackOut, BackInOut,
		ElasticIn, ElasticOut, ElasticInOut,
		BounceIn, BounceOut, BounceInOut
	};

	class Easing
	{
	public:
		static float Linear(float t);

		static float QuadIn(float t);
		static float QuadOut(float t);
		static float QuadInOut(float t);

		static float CubicIn(float t);
		static float CubicOut(float t);
		static float CubicInOut(float t);

		static float QuarticIn(float t);
		static float QuarticOut(float t);
		static float QuarticInOut(float t);

		static float SineIn(float t);
		static float SineOut(float t);
		static float SineInOut(float t);

		static float ExpoIn(float t);
		static float ExpoOut(float t);
		static float ExpoInOut(float t);

		static float BackIn(float t);
		static float BackOut(float t);
		static float BackInOut(float t);

		static float ElasticIn(float t);
		static float ElasticOut(float t);
		static float ElasticInOut(float t);

		static float BounceIn(float t);
		static float BounceOut(float t);
		static float BounceInOut(float t);

		static float Evaluate(EasingType type, float t);
	};
}


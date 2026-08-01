/*
 *	Copyright 2026-2027 static - hiro
 *
 *	Use of this source code without explicit permission from owner is strictly prohibited.
 */

#pragma once

#include "Base/Sprite.h"

namespace Utils
{
	class Collision
	{
	public:
		static bool AABB(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2)
		{
			return x1 < x2 + w2 &&
				x1 + w1 > x2 &&
				y1 < y2 + h2 &&
				y1 + h1 > y2;
		}

		static bool AABB(
			float x1, float y1,
			float x2, float y2,
			float w2, float h2, float dW, float dH, bool ratio)
		{
			float _x2 = x2;
			float _y2 = y2;
			if (ratio)
			{
				_x2 = _x2 * dW;
				_y2 = _y2 * dH;
			}

			return x1 >= _x2 &&
				y1 >= _y2 &&
				x1 <= _x2 + w2 &&
				y1 <= _y2 + h2;
		}

		static bool SAT(float x1, float y1,
			float x2, float y2,
			float w2, float h2, float dW, float dH)
		{
			float tR = x1 + dW;
			float tL = x1;
			float tT = y1;
			float tB = y1 + dH;

			float oR = x2 + w2;
			float oL = x2;
			float oT = y2;
			float oB = y2 + h2;

			return tR < oL || tL > oR || tT > oB || tB < oT;
		}

		static bool AABB(const Base::Sprite& a, const Base::Sprite& b)
		{
			return AABB(a.GetX(), a.GetY(), a.GetWidth(), a.GetHeight(), b.GetX(), b.GetY(), b.GetWidth(), b.GetHeight());
		}

		static bool AABB(Base::Sprite* obj1, Base::Sprite* obj2)
		{
			return AABB(
				obj1->GetX(), obj1->GetY(),
				obj2->GetX(), obj2->GetY(),
				obj2->GetWidth(), obj2->GetHeight(),
				obj1->GetWidth(), obj1->GetHeight());
		}

		static bool AABB(Base::Sprite* obj, float x, float y, float w, float h)
		{
			return AABB(
				x, y,
				obj->GetX(), obj->GetY(),
				obj->GetWidth(), obj->GetHeight(),
				w, h);
		}

		static bool SAT(Base::Sprite* obj, float x, float y, float w, float h)
		{
			return SAT(
				x, y,
				obj->GetX(), obj->GetY(),
				obj->GetWidth(), obj->GetHeight(),
				w, h);
		}

		static bool AABB(float x, float y, float w, float h, Base::Sprite* obj)
		{
			return AABB(
				obj->GetX(), obj->GetY(),
				x, y,
				w, h,
				obj->GetWidth(), obj->GetHeight());
		}

		static bool SAT(float x, float y, float w, float h, Base::Sprite* obj)
		{
			return SAT(
				obj->GetX(), obj->GetY(),
				x, y,
				w, h,
				obj->GetWidth(), obj->GetHeight());
		}
	};
}


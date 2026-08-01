/*
 *	Copyright 2026-2027 static - hiro
 *
 *	Use of this source code without explicit permission from owner is strictly prohibited.
 */

#include "Base/GameObject.h"

namespace Base
{
	GameObject::~GameObject() = default;

	void GameObject::Update(float deltaTime)
	{
		// updates the obj
		(void)deltaTime;
	}

	void GameObject::Draw(Render::Renderer& renderer)
	{
		// renders the obj
		(void)renderer;
	}
}


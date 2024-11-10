#pragma once
#include "vxpch.h"

#include "Component.h"
#include "Core/Sprite.h"
#include "Core/Color.h"

namespace VoltexEngine {

	class SpriteComponent : public Component
	{

	public:

		std::shared_ptr<Sprite> Sprite;

		/* Tint of the sprite */
		Color Tint;

	public:

		SpriteComponent();

	};

}
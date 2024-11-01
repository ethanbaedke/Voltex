#pragma once
#include "vxpch.h"

#include "Component.h"
#include "Core/Sprite.h"

namespace VoltexEngine {

	class SpriteComponent : public Component
	{

	public:

		std::shared_ptr<Sprite> Sprite;

	public:

		SpriteComponent();

	};

}
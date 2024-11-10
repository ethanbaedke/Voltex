#include "vxpch.h"
#include "SpriteComponent.h"

namespace VoltexEngine {

	SpriteComponent::SpriteComponent()
		: Component(), Tint(Color(255, 255, 255, 255)), Sprite(nullptr)
	{
	}

}
#pragma once
#include "vxpch.h"

#include "Component.h"
#include "Core/Vector.h"
#include "Core/Event.h"

namespace VoltexEngine {

	class CollisionComponent : public Component
	{

	public:

		Vector Size;

		Event_OneParam<std::shared_ptr<CollisionComponent>> OnCollision;

	public:

		CollisionComponent();

	};

}
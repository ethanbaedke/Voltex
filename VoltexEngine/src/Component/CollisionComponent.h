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

		/* If this component is dynamic, it means that the object its attached to will be moving around the screen, this should be false whenever possible */
		bool IsDynamic;

		Event_OneParam<std::shared_ptr<CollisionComponent>> OnCollision;

	public:

		CollisionComponent();

	};

}
#include "vxpch.h"
#include "CollisionComponent.h"

namespace VoltexEngine {

	CollisionComponent::CollisionComponent()
		: Component(), Size(Vector(1, 1)), IsDynamic(false)
	{
	}

}
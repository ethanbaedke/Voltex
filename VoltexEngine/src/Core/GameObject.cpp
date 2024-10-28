#include "vxpch.h"
#include "GameObject.h"

#include "Console.h"

namespace VoltexEngine {

	GameObject::GameObject()
		: Position(Vector()), Angle(0.0f), Depth(0)
	{
	}

	std::string GameObject::GetPrintable() const
	{
		return Position.GetPrintable();
	}

}
#include "vxpch.h"
#include "GameObject.h"

#include "Console.h"

namespace VoltexEngine {

	GameObject::GameObject()
		: m_Position(Vector()), m_Angle(0.0f), m_Depth(0)
	{
	}

	std::string GameObject::GetPrintable() const
	{
		return m_Position.GetPrintable();
	}

}
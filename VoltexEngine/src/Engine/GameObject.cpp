#include "vxpch.h"
#include "GameObject.h"

#include "Console.h"

namespace VoltexEngine {

	// Declare static variables
	Event_OneParam<std::weak_ptr<GameObject>> GameObject::s_OnGameObjectCreated;

	std::string GameObject::GetPrintable() const
	{
		return m_Position.GetPrintable();
	}

	GameObject::GameObject()
		: m_Position(Vector()), m_Scale(Vector(1.0f)), m_Angle(0.0f)
	{
	}

}
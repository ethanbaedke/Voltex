#include "vxpch.h"
#include "GameObject.h"

#include "Console.h"

namespace VoltexEngine {

	// Initialize static variables
	Event_OneParam<GameObject> GameObject::s_OnGameObjectCreated;

}
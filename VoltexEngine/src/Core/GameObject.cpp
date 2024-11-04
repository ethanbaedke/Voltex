#include "vxpch.h"
#include "GameObject.h"

#include "Console.h"

namespace VoltexEngine {

	Event_OneParam<std::shared_ptr<GameObject>> GameObject::OnCreated;

	GameObject::GameObject()
		: Position(Vector()), Angle(0.0f), Depth(0), m_Components(std::vector<std::shared_ptr<Component>>()), m_UninitializedComponents(std::vector<std::shared_ptr<Component>>())
	{
	}

	void GameObject::Initialize()
	{
		InitializeNewComponents();
	}

	void GameObject::Update(float deltaTime)
	{
		InitializeNewComponents();
	}

	std::string GameObject::GetPrintable() const
	{
		return Position.GetPrintable();
	}

	void GameObject::InitializeNewComponents()
	{
		// We use a while loop so if components are created in other components init functions they are also initialized
		while (m_UninitializedComponents.size() > 0)
		{
			// Get and remove the back object before initializing it so if it adds more objects to the back of the list they are not removed instead
			std::shared_ptr<Component> comp = m_UninitializedComponents.back();
			m_UninitializedComponents.pop_back();

			// Initialize the object and add it to the list of initialized game objects
			comp->Initialize();
			m_Components.push_back(comp);
		}
	}

}
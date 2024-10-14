#include "vxpch.h"
#include "Application.h"

#include "Renderer.h"
#include "Console.h"
#include "Vector.h"

namespace VoltexEngine {

	Application::Application()
		: m_Window(nullptr), m_GameObjects(std::vector<std::weak_ptr<GameObject>>()), m_UninitializedGameObjects(std::vector<std::weak_ptr<GameObject>>())
	{
		if (!Renderer::Init())
			return;

		// Subscribe to game objects being created
		GameObject::s_OnGameObjectCreated.AddCallback([&](std::weak_ptr<GameObject> gameObject) { HandleGameObjectCreated(gameObject); });

		m_Window = Window::Create(std::string("Voltex Window"), 1280, 720);

		VX_LOG("Application Initialized");
	}

	void Application::Run()
	{
		float deltaTime = 1.0f / 60.0f;

		while (true)
		{
			// Initialize any uninitialized game objects
			// We use a while loop so if objects are created in other objects init functions they are also initialized
			while (m_UninitializedGameObjects.size() > 0)
			{
				// Get and remove the back object before initializing it so if it adds more objects to the back of the list they are not removed instead
				std::weak_ptr<GameObject> weakObj = m_UninitializedGameObjects.back();
				m_UninitializedGameObjects.pop_back();

				// Make sure the object still exists
				if (std::shared_ptr<GameObject> sharedObj = weakObj.lock())
				{
					// Initialize the object and add it to the game objects list
					sharedObj->Initialize();
					m_GameObjects.push_back(weakObj);
				}
				else
				{
					VX_ERROR("Trying to access object but it does not exist");
				}
			}

			// Update game objects
			for (std::weak_ptr<GameObject> weakObj : m_GameObjects)
			{
				// Make sure the object still exists
				if (std::shared_ptr<GameObject> sharedObj = weakObj.lock())
				{
					// Update the object
					sharedObj->Update(deltaTime);
				}
			}

			// Render
			Renderer::Tick(deltaTime);
		}
	}

	void Application::HandleGameObjectCreated(std::weak_ptr<GameObject> gameObject)
	{
		m_UninitializedGameObjects.push_back(gameObject);
	}

}
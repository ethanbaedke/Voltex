#include "vxpch.h"
#include "Application.h"

#include "Renderer.h"
#include "Console.h"
#include "Vector.h"

#include <chrono>

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
		auto prevFrameTime = std::chrono::high_resolution_clock::now();

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
			}

			// Update game objects
			auto currentFrameTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> deltaTime = currentFrameTime - prevFrameTime;
			for (int i = 0; i < m_GameObjects.size(); i++)
			{
				if (std::shared_ptr<GameObject> obj = m_GameObjects[i].lock())
				{
					// Update object
					obj->Update(deltaTime.count());
				}
				else
				{
					// Remove expired object
					m_GameObjects.erase(m_GameObjects.begin() + i);
				}
			}
			prevFrameTime = currentFrameTime;

			// Render, by the time we do this any expired game objects have been removed already
			Renderer::Tick(m_GameObjects);
		}
	}

	void Application::HandleGameObjectCreated(std::weak_ptr<GameObject> gameObject)
	{
		m_UninitializedGameObjects.push_back(gameObject);
	}

}
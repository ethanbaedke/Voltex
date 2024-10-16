#include "vxpch.h"
#include "Application.h"

#include "Renderer.h"
#include "Console.h"
#include "Vector.h"

#include <chrono>

namespace VoltexEngine {

	Application::Application()
		: m_GameObjects(std::vector<std::shared_ptr<GameObject>>()), m_UninitializedGameObjects(std::vector<std::shared_ptr<GameObject>>())
	{
		if (!Renderer::Init(1280, 720))
			return;

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
				std::shared_ptr<GameObject> obj = m_UninitializedGameObjects.back();
				m_UninitializedGameObjects.pop_back();
				
				// Initialize the object and add it to the list of initialized game objects
				obj->Initialize();
				m_GameObjects.push_back(obj);
			}

			// Update game objects
			auto currentFrameTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> deltaTime = currentFrameTime - prevFrameTime;
			for (std::shared_ptr<GameObject> obj : m_GameObjects)
			{
				// Update game object
				obj->Update(deltaTime.count());
			}
			prevFrameTime = currentFrameTime;

			// Render, by the time we do this any expired game objects have been removed already
			Renderer::Tick(m_GameObjects);
		}
	}

	std::shared_ptr<Sprite> Application::CreateSprite(const std::string& texturePath)
	{
		int width, height;
		unsigned int textureID = Renderer::GenerateTexture(texturePath, &width, &height);

		if (textureID == 0)
			return std::shared_ptr<Sprite>(nullptr);

		std::shared_ptr<Sprite> spr = std::make_shared<Sprite>(width, height, textureID);
		return spr;
	}

}
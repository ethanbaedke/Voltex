#include "vxpch.h"
#include "Application.h"

#include "Renderer.h"
#include "Console.h"
#include "Vector.h"
#include "Input.h"
#include "tinyfiledialogs.h"

#include <chrono>

namespace VoltexEngine {

	Application* Application::Current;

	Application::Application()
		: m_GameObjects(std::vector<std::shared_ptr<GameObject>>()), m_UninitializedGameObjects(std::vector<std::shared_ptr<GameObject>>())
	{
		if (!Input::Init())
			return;

		if (!Renderer::Init(1280, 720))
			return;

		Current = this;

		VX_LOG("Application Initialized");
	}

	void Application::Run()
	{
		auto prevFrameTime = std::chrono::high_resolution_clock::now();

		while (true)
		{
			int i;

			// Process input for this frame (input queued last frame is now available for use)
			Input::Tick();

			// Remove gizmos that are no longer root gizmos
			i = 0;
			while (i < m_RootGizmos.size())
			{
				if (!m_RootGizmos[i]->IsRoot)
					m_RootGizmos.erase(m_RootGizmos.begin() + i);
				else
					i++;
			}

			// Handle cursor logic for UI
			if (Renderer::GetCursorEnabled())
			{
				std::shared_ptr<Gizmo> hit;

				float curX, curY;
				Input::GetCursorPosition(&curX, &curY);

				// Depth first check all gizmos for collision
				std::vector<std::shared_ptr<Gizmo>> collisionStack(m_RootGizmos);

				while (collisionStack.size() > 0)
				{
					// Pop the next gizmo off the top of the stack
					std::shared_ptr<Gizmo> giz = collisionStack.back();
					collisionStack.pop_back();

					// If the current gizmo is a layout gizmo, get its children and add them to the render stack
					if (std::shared_ptr<LayoutGizmo> layGiz = std::dynamic_pointer_cast<LayoutGizmo>(giz))
					{
						std::vector<std::shared_ptr<Gizmo>> children = layGiz->GetChildren();
						for (std::shared_ptr<Gizmo> childGiz : children)
							collisionStack.push_back(childGiz);
					}

					// If our current gizmo is at a lower depth than our hit gizmo, ignore it
					if (hit && giz->Depth < hit->Depth)
						continue;

					// Check if the cursor overlaps the gizmo
					float x, y, w, h;
					giz->GetDimensions(&x, &y, &w, &h);
					if (x < curX && curX < (x + w) && y < curY && curY < (y + h))
						hit = giz;
				}

				Gizmo::CursorOverlap(hit);

				if (Input::MouseDown(ClickCode::LeftMouse))
					Gizmo::CursorSelect();
			}

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
				obj->Update(deltaTime.count());
			prevFrameTime = currentFrameTime;

			// Update UI, removing gizmos that are no longer root gizmos from the root gizmos list
			for (std::shared_ptr<Gizmo> giz : m_RootGizmos)
				giz->Tick();

			// Render, by the time we do this any expired game objects have been removed already
			Renderer::Tick(m_GameObjects, m_RootGizmos);
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

	const char* Application::LoadFileDialog() const
	{
		const char* filter[] = { "*.bke" };
		return tinyfd_openFileDialog("Select a Room File", "../VoltexGame/rooms", 1, filter, "BKE File", 0);
	}

	const char* Application::SaveFileDialog() const
	{
		const char* filter[] = {"*.bke"};
		return tinyfd_saveFileDialog("Save File As", "../VoltexGame/rooms/NewRoom.bke", 1, filter, "BKE File");
	}

}
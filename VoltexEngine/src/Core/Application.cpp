#include "vxpch.h"
#include "Application.h"

#include "Renderer.h"
#include "Console.h"
#include "Vector.h"
#include "Input.h"
#include "UI/LayoutGizmo.h"
#include "UI/TextGizmo.h"
#include "Component/CollisionComponent.h"
#include "tinyfiledialogs.h"

#include <chrono>

namespace VoltexEngine {

	Application::Application()
		: m_GameObjects(std::vector<std::shared_ptr<GameObject>>()), m_UninitializedGameObjects(std::vector<std::shared_ptr<GameObject>>())
	{
		if (!Input::Init())
			return;

		if (!Renderer::Init(1280, 720))
			return;

		GameObject::OnCreated.AddCallback([&](std::shared_ptr<GameObject> obj) { HandleGameObjectCreated(obj); });
		Gizmo::OnCreated.AddCallback([&](std::shared_ptr<Gizmo> giz) { HandleGizmoCreated(giz); });

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
					// Otherwise, if the current gizmo is a text gizmo, skip checking for overlap with it
					else if (std::shared_ptr<TextGizmo> textGiz = std::dynamic_pointer_cast<TextGizmo>(giz))
					{
						continue;
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

			std::vector<std::shared_ptr<GameObject>> staticObjects;
			std::vector<std::shared_ptr<GameObject>> dynamicObjects;

			// Update game objects and get colliders (for collision step)
			auto currentFrameTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> deltaTime = currentFrameTime - prevFrameTime;

			for (std::shared_ptr<GameObject> obj : m_GameObjects)
			{
				obj->Update(deltaTime.count());
				if (std::shared_ptr<CollisionComponent> colComp = obj->GetComponent<CollisionComponent>())
				{
					if (!colComp->IsDynamic)
						staticObjects.push_back(obj);
					else
						dynamicObjects.push_back(obj);
				}
			}
			prevFrameTime = currentFrameTime;

			// Handle collision
			for (int i = 0; i < dynamicObjects.size(); i++)
			{
				// Get the dynamic object and its collision component
				std::shared_ptr<GameObject> objA = dynamicObjects[i];
				std::shared_ptr<CollisionComponent> colA = objA->GetComponent<CollisionComponent>();

				// Get the bottom left and top right coordinates of its collision component
				Vector blA = objA->Position - (colA->Size / 2);
				Vector trA = objA->Position + (colA->Size / 2);

				// If this isnt the last object in the list, check it against all other dynamic objects
				if (i < (int)dynamicObjects.size() - 1)
				{
					for (int f = i + 1; f < dynamicObjects.size(); f++)
					{
						// Get the other dynamic object and its collision component
						std::shared_ptr<GameObject> objB = dynamicObjects[f];
						std::shared_ptr<CollisionComponent> colB = objB->GetComponent<CollisionComponent>();

						// Get the bottom left and top right coordinates of its collision component
						Vector blB = objB->Position - (colB->Size / 2);
						Vector trB = objB->Position + (colB->Size / 2);

						// Check if the two are colliding
						if (blA.X >= trB.X || trA.X <= blB.X || blA.Y >= trB.Y || trA.Y <= blB.Y)
							continue;
						else
						{
							colA->OnCollision.Invoke(colB);
							colB->OnCollision.Invoke(colA);
						}
					}
				}

				// Check this object against all static objects
				for (int f = 0; f < staticObjects.size(); f++)
				{
					// Get the static object and its collision component
					std::shared_ptr<GameObject> objB = staticObjects[f];
					std::shared_ptr<CollisionComponent> colB = objB->GetComponent<CollisionComponent>();

					// Get the bottom left and top right coordinates of its collision component
					Vector blB = objB->Position - (colB->Size / 2);
					Vector trB = objB->Position + (colB->Size / 2);

					// Check if the two are colliding
					if (blA.X >= trB.X || trA.X <= blB.X || blA.Y >= trB.Y || trA.Y <= blB.Y)
						continue;
					else
					{
						colA->OnCollision.Invoke(colB);
						colB->OnCollision.Invoke(colA);
					}
				}
			}

			// Update UI
			for (int i = 0; i < m_RootGizmos.size(); i++)
				m_RootGizmos[i]->Tick();

			// Render, by the time we do this any expired game objects have been removed already
			Renderer::Tick(m_GameObjects, m_RootGizmos);
		}
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

	void Application::HandleGameObjectCreated(std::shared_ptr<GameObject> obj)
	{
		m_UninitializedGameObjects.push_back(obj);
	}

	void Application::HandleGizmoCreated(std::shared_ptr<Gizmo> giz)
	{
		m_RootGizmos.push_back(giz);
	}

}
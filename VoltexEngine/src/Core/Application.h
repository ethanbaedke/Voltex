#pragma once
#include "vxpch.h"

#include "GameObject.h"
#include "UI/Gizmo.h"

namespace VoltexEngine {

	/*
	* The "Voltex Engine" is a set of classes meant to abstract complex logic so applications can be developed at a higher level
	* The Application class is meant to be derived from by the application using the engine
	*/
	class Application
	{

	private:

		/* All initialized game objects tracked by this application */
		std::vector<std::shared_ptr<GameObject>> m_GameObjects;

		/* Uninitialized game objects that need to be initialized next frame */
		std::vector<std::shared_ptr<GameObject>> m_UninitializedGameObjects;

		/* The root gizmo for UI rendering */
		std::shared_ptr<Gizmo> m_RootGizmo;

	public:

		Application();

		void Run();

		/* Creates an returns a GameObject or any subclass of a GameObject */
		template <typename T>
		std::shared_ptr<T> CreateObject()
		{
			std::shared_ptr<T> obj = std::make_shared<T>();
			m_UninitializedGameObjects.push_back(obj);
			return obj;
		}

		/* Creates and returns a Sprite
		   If there are any errors reading the texture, returns nullptr */
		std::shared_ptr<Sprite> CreateSprite(const std::string& texturePath);

		/* Creates and returns a Gizmo */
		std::shared_ptr<Gizmo> CreateGizmo();
		std::shared_ptr<Gizmo> CreateGizmo(UILayout layout);
		std::shared_ptr<Gizmo> CreateGizmo(std::shared_ptr<Gizmo> parent);
		std::shared_ptr<Gizmo> CreateGizmo(std::shared_ptr<Gizmo> parent, UILayout layout);
		std::shared_ptr<Gizmo> CreateGizmo(std::shared_ptr<Gizmo> parent, unsigned int weight);
		std::shared_ptr<Gizmo> CreateGizmo(std::shared_ptr<Gizmo> parent, UILayout layout, unsigned int weight);

		inline void SetRootGizmo(std::shared_ptr<Gizmo> gizmo) { m_RootGizmo = gizmo; }

	};

}

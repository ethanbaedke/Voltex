#pragma once
#include "vxpch.h"

#include "Window.h"
#include "GameObject.h"

namespace VoltexEngine {

	/*
	* The "Voltex Engine" is a set of classes meant to abstract complex logic so applications can be developed at a higher level
	* The Application class is meant to be derived from by the application using the engine
	*/
	class Application
	{

	private:

		std::shared_ptr<Window> m_Window;

		/* All initialized game objects tracked by this application */
		std::vector<std::weak_ptr<GameObject>> m_GameObjects;

		/* Uninitialized game objects that need to be initialized next frame */
		std::vector<std::weak_ptr<GameObject>> m_UninitializedGameObjects;

	public:

		Application();

		void Run();

	private:

		void HandleGameObjectCreated(std::weak_ptr<GameObject> gameObject);

	};

}

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

		static Application* s_Current;

		/* All initialized game objects tracked by this application */
		std::vector<std::shared_ptr<GameObject>> m_GameObjects;

		/* Uninitialized game objects that need to be initialized next frame */
		std::vector<std::shared_ptr<GameObject>> m_UninitializedGameObjects;

		/* Gizmos for UI rendering */
		std::vector<std::shared_ptr<Gizmo>> m_RootGizmos;

	public:

		static std::shared_ptr<GameObject> OverlapPoint(const Vector& point);

		Application();

		void Run();

		/* Opens dialogues where the user can select a file or folder and returns the selected path */
		const char* LoadFileDialog() const;
		const char* SaveFileDialog() const;

	private:

		/* Event handlers */
		void HandleGameObjectCreated(std::shared_ptr<GameObject> obj);
		void HandleGizmoCreated(std::shared_ptr<Gizmo> giz);

	};

}

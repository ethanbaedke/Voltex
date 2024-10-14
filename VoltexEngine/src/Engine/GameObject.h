#pragma once
#include "vxpch.h"

#include "Vector.h"
#include "Event.h"

namespace VoltexEngine {

	class GameObject : public std::enable_shared_from_this<GameObject>
	{

	public:

		/* Broadcasts a game object whenever one is created */
		static Event_OneParam<GameObject> s_OnGameObjectCreated;

	public:

		/* Creates and returns a game object */
		template <typename T>
		static std::shared_ptr<T> Create()
		{
			std::shared_ptr<T> obj(new T());
			s_OnGameObjectCreated.Invoke(obj);
			return obj;
		}

	public:

		/* Called once at the begining of the closest frame after this object is constructed */
		virtual void Initialize() {};

		/* Called once every frame */
		virtual void Update(float deltaTime) {};

	protected:

		/* Protected constructor forces the use of the create function */
		GameObject() {};

	};

}


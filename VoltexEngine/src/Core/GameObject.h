#pragma once
#include "vxpch.h"

#include "Vector.h"
#include "Event.h"
#include "Printable.h"
#include "Sprite.h"
#include "Component/Component.h"

namespace VoltexEngine {

	class GameObject : public IPrintable
	{

	public:

		/* This objects position in the world */
		Vector Position;

		/* The angle of rotation of the object in the world, in degrees */
		float Angle;

		/* Objects with a lower depth will be rendered behind objects with a higher depth */
		int Depth;

	private:

		/* All initialized game objects tracked by this application */
		std::vector<std::shared_ptr<Component>> m_Components;

		/* Uninitialized game objects that need to be initialized next frame */
		std::vector<std::shared_ptr<Component>> m_UninitializedComponents;

	public:

		GameObject();

		/* Create an add a component to this game object */
		template <typename T>
		std::shared_ptr<T> AddComponent()
		{
			std::shared_ptr<T> comp = std::make_shared<T>();
			m_UninitializedComponents.push_back(comp);
			return comp;
		}

		/* Gets and returns the first found component of the given type off this object if it exists, returns nullptr otherwise */
		template <typename T>
		std::shared_ptr<T> GetComponent()
		{
			// There is a chance this will cause issues if a user attaches a component to an object and tries to retrieve it the same frame
			// In this case, the component will still be uninitialized and will not be returned
			int i = 0;
			while (i < m_Components.size())
				if (std::shared_ptr<T> comp = std::dynamic_pointer_cast<T>(m_Components[i]))
					return comp;
			return nullptr;
		}

		/* Called once at the begining of the closest frame after this object is constructed */
		virtual void Initialize();

		/* Called once every frame */
		virtual void Update(float deltaTime);

		virtual std::string GetPrintable() const;

	private:

		// Initialize any uninitialized components
		void InitializeNewComponents();

	};

}


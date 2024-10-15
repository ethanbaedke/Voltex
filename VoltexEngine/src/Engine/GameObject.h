#pragma once
#include "vxpch.h"

#include "Vector.h"
#include "Event.h"
#include "Printable.h"
#include "Sprite.h"

namespace VoltexEngine {

	class GameObject : public IPrintable
	{

	public:

		/* Broadcasts a game object whenever one is created */
		static Event_OneParam<std::weak_ptr<GameObject>> s_OnGameObjectCreated;

	public:

		/* Creates and returns a game object */
		template <typename T>
		static std::shared_ptr<T> Create()
		{
			std::shared_ptr<T> obj(new T());
			s_OnGameObjectCreated.Invoke(obj);
			return obj;
		}

	private:

		/* This objects position in the world */
		Vector m_Position;

		/* The objects scale in the world */
		Vector m_Scale;

		/* The angle of rotation of the object in the world, in degrees */
		float m_Angle;

		/* Objects with a lower depth will be rendered behind objects with a higher depth */
		int m_Depth;

		std::shared_ptr<Sprite> m_Sprite;

	public:

		/* Called once at the begining of the closest frame after this object is constructed */
		virtual void Initialize() {};

		/* Called once every frame */
		virtual void Update(float deltaTime) {};

		inline void SetPosition(const Vector& position) { m_Position = position; }
		inline const Vector& GetPosition() { return m_Position; }

		inline void SetScale(const Vector& scale) { m_Scale = scale; }
		inline const Vector& GetScale() { return m_Scale; }

		inline void SetRotation(float angle) { m_Angle = angle; }
		inline float GetRotation() { return m_Angle; }

		inline void SetDepth(int depth) { m_Depth = depth; }
		inline int GetDepth() { return m_Depth; }

		inline void SetSprite(std::shared_ptr<Sprite> sprite) { m_Sprite = sprite; }
		inline std::shared_ptr<Sprite> GetSprite() { return m_Sprite; }

		virtual std::string GetPrintable() const;

	protected:

		/* Protected constructor forces the use of the create function */
		GameObject();

	};

}


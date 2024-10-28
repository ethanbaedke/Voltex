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

		GameObject();

	public:

		/* This objects position in the world */
		Vector Position;

		/* The angle of rotation of the object in the world, in degrees */
		float Angle;

		/* Objects with a lower depth will be rendered behind objects with a higher depth */
		int Depth;

		std::shared_ptr<Sprite> ObjectSprite;

	public:

		/* Called once at the begining of the closest frame after this object is constructed */
		virtual void Initialize() {};

		/* Called once every frame */
		virtual void Update(float deltaTime) {};

		virtual std::string GetPrintable() const;

	};

}


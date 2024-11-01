#pragma once
#include "vxpch.h"

namespace VoltexEngine {

	class Component
	{

	public:

		/* Called once at the begining of the closest frame after this component is constructed */
		virtual void Initialize() {};

		/* Called once every frame */
		virtual void Update(float deltaTime) {};

	};

}
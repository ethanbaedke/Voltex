#pragma once
#include "vxpch.h"

#include "Event.h"

namespace VoltexEngine {

	class Sprite
	{

	public:

		static Event_TwoParam<std::shared_ptr<Sprite>, const std::string&> OnCreated;

		/* Creates and returns a Sprite */
		static std::shared_ptr<Sprite> Create(const std::string& texturePath);

	public:

		/* The pixel width and height of the texture this sprite is holding */
		int PixelWidth;
		int PixelHeight;

	public:

		Sprite();

	};

}


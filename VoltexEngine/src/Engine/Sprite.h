#pragma once
#include "vxpch.h"

#include "Event.h"

namespace VoltexEngine {

	class Sprite
	{

	private:

		/* This is used exclusively by the renderer */
		unsigned int m_TextureID;

	public:

		Sprite(unsigned int textureID);

		/* These should only be called by the renderer */
		inline unsigned int GetTextureID() const { return m_TextureID; }

	};

}


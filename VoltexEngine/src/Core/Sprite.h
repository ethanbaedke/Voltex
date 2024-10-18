#pragma once
#include "vxpch.h"

#include "Event.h"

namespace VoltexEngine {

	class Sprite
	{

	private:

		/* This is used exclusively by the renderer */
		unsigned int m_TextureID;

		/* The pixel width and height of the texture this sprite is holding */
		int m_Width;
		int m_Height;

	public:

		Sprite(int width, int height, unsigned int textureID);

		/* These should only be called by the renderer */
		inline unsigned int GetTextureID() const { return m_TextureID; }

		inline int GetWidth() const { return m_Width; }
		inline int GetHeight() const { return m_Height; }

	};

}


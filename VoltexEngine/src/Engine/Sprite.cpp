#include "vxpch.h"
#include "Sprite.h"

namespace VoltexEngine {

	Sprite::Sprite(int width, int height, unsigned int textureID)
		: m_TextureID(textureID), m_Width(width), m_Height(height)
	{
	}

}
#include "vxpch.h"
#include "Sprite.h"

namespace VoltexEngine {

	// Declare static variables
	Event_TwoParam<std::weak_ptr<Sprite>, const std::string&> Sprite::s_OnSpriteCreated;

	std::shared_ptr<Sprite> Sprite::Create(const std::string& texturePath)
	{
		std::shared_ptr<Sprite> spr(new Sprite(texturePath));
		s_OnSpriteCreated.Invoke(spr, texturePath);
		return spr;
	}

	Sprite::Sprite(const std::string& texturePath)
		: m_TextureID(0)
	{
	}

}
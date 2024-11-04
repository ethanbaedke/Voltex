#include "vxpch.h"
#include "Sprite.h"

namespace VoltexEngine {

	Event_TwoParam<std::shared_ptr<Sprite>, const std::string&> Sprite::OnCreated;

	std::shared_ptr<Sprite> Sprite::Create(const std::string& texturePath)
	{
		std::shared_ptr<Sprite> spr = std::make_shared<Sprite>();
		OnCreated.Invoke(spr, texturePath);
		return spr;
	}

	Sprite::Sprite()
		: PixelWidth(0), PixelHeight(0)
	{
	}

}
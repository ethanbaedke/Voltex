#pragma once

#include "VECore.h"

using namespace VoltexEngine;

class Level
{

private:

	static const int ROOM_WIDTH = 20;
	static const int ROOM_HEIGHT = 10;

	static std::vector<std::shared_ptr<Sprite>> s_TileSprites;

public:

	Level(const Vector& position, const Vector& size);

};


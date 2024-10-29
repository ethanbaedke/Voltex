#pragma once

#include "VECore.h"

#include <filesystem>
#include <random>

using namespace VoltexEngine;

class Level
{

private:

	static bool s_LevelDataInitialized;

	static const int ROOM_WIDTH = 20;
	static const int ROOM_HEIGHT = 10;

	static std::vector<std::shared_ptr<Sprite>> s_TileSprites;

	/* List of file paths in each category of room */
	static std::vector<std::filesystem::path> s_StandardPaths;
	static std::vector<std::filesystem::path> s_DropPaths;
	static std::vector<std::filesystem::path> s_CatchPaths;

	static std::mt19937 randomGenerator;

public:

	Level(const Vector& position, const Vector& size);

};


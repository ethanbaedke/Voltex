#pragma once

#include "VECore.h"

#include <filesystem>
#include <random>

using namespace VoltexEngine;

class Level
{

private:

	static const int ROOM_WIDTH = 20;
	static const int ROOM_HEIGHT = 10;

	static bool s_LevelDataInitialized;

	static std::vector<std::shared_ptr<Sprite>> s_TileSprites;

	/* List of file paths in each category of room */
	static std::vector<std::filesystem::path> s_StandardPaths;
	static std::vector<std::filesystem::path> s_DropPaths;
	static std::vector<std::filesystem::path> s_CatchPaths;
	static std::vector<std::filesystem::path> s_StartPaths;
	static std::vector<std::filesystem::path> s_EndPaths;

	static std::mt19937 s_RandomGenerator;

public:

	Level(const Vector& position, Vector& size);

private:

	/* 
	* Returns a list of integers representing a level
	* 0 - a standard room
	* 1 - a drop room
	* 2 - a catch room
	* 3 - the start room
	* 4 - the end room
	*/
	std::vector<int> GenerateLevel(Vector size);

};


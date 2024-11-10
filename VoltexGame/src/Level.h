#pragma once

#include <VXCore.h>

#include <filesystem>
#include <random>

using namespace VoltexEngine;

class Level
{

private:

	static const int ROOM_WIDTH = 20;
	static const int ROOM_HEIGHT = 10;

	static bool s_LevelDataInitialized;

	static std::shared_ptr<Sprite> s_DirtBlockSprite;
	static std::shared_ptr<Sprite> s_GrassBlockSprite;
	static std::shared_ptr<Sprite> s_StoneBlockSprite;

	/* List of file paths in each category of room */
	static std::vector<std::filesystem::path> s_StandardPaths;
	static std::vector<std::filesystem::path> s_DropPaths;
	static std::vector<std::filesystem::path> s_CatchPaths;
	static std::vector<std::filesystem::path> s_StartPaths;
	static std::vector<std::filesystem::path> s_EndPaths;

	static std::mt19937 s_RandomGenerator;

private:

	Vector m_Postion;
	Vector m_Size;

public:

	Level(const Vector& position, Vector& size);

	void Generate(Vector* outStartPos, Vector* outEndPos);

private:

	/* 
	* Returns a list of integers representing a level
	* 0 - a standard room
	* 1 - a drop room
	* 2 - a catch room
	* 3 - the start room
	* 4 - the end room
	*/
	std::vector<int> GenerateMap(Vector size);

};


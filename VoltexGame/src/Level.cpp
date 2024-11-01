#include "Level.h"

#include <fstream>

bool Level::s_LevelDataInitialized = false;
std::vector<std::shared_ptr<Sprite>> Level::s_TileSprites;
std::vector<std::filesystem::path> Level::s_StandardPaths;
std::vector<std::filesystem::path> Level::s_DropPaths;
std::vector<std::filesystem::path> Level::s_CatchPaths;
std::vector<std::filesystem::path> Level::s_StartPaths;
std::vector<std::filesystem::path> Level::s_EndPaths;
std::mt19937 Level::s_RandomGenerator;

Level::Level(const Vector& position, Vector& size)
{
	// Make sure our level is at least 2x2
	if (size.X < 2)
		size.X = 2;
	if (size.Y < 2)
		size.Y = 2;

	// The first time we create a level, initialize data levels rely on like sprites and room file paths
	if (!s_LevelDataInitialized)
	{
		s_TileSprites.push_back(nullptr);
		s_TileSprites.push_back(Application::Current->CreateSprite("../VoltexGame/textures/tiles/DirtBlock.png"));

		for (const auto& entry : std::filesystem::directory_iterator("rooms/standard"))
			s_StandardPaths.push_back(entry.path());
		for (const auto& entry : std::filesystem::directory_iterator("rooms/drop"))
			s_DropPaths.push_back(entry.path());
		for (const auto& entry : std::filesystem::directory_iterator("rooms/catch"))
			s_CatchPaths.push_back(entry.path());
		for (const auto& entry : std::filesystem::directory_iterator("rooms/start"))
			s_StartPaths.push_back(entry.path());
		for (const auto& entry : std::filesystem::directory_iterator("rooms/end"))
			s_EndPaths.push_back(entry.path());

		std::random_device rd;
		s_RandomGenerator = std::mt19937(rd());

		s_LevelDataInitialized = true;
	}

	int xOffset = position.X;
	int yOffset = position.Y;

	// Build the border
	for (int borderX = position.X - 1; borderX < ((position.X + (size.X * ROOM_WIDTH)) + 1); borderX++)
	{
		// Create an object for the tile, set its sprite, and position it in the world
		// Do this for the top and bottom of the level
		std::shared_ptr<GameObject> topObj = Application::Current->CreateObject<GameObject>();
		std::shared_ptr<SpriteComponent> topSprComp = topObj->AddComponent<SpriteComponent>();
		topSprComp->Sprite = s_TileSprites[1];
		topObj->Position.X = borderX;
		topObj->Position.Y = position.Y + 1;
		std::shared_ptr<GameObject> bottomObj = Application::Current->CreateObject<GameObject>();
		std::shared_ptr<SpriteComponent> bottomSprComp = bottomObj->AddComponent<SpriteComponent>();
		bottomSprComp->Sprite = s_TileSprites[1];
		bottomObj->Position.X = borderX;
		bottomObj->Position.Y = position.Y - (size.Y * ROOM_HEIGHT);
	}
	for (int borderY = position.Y; borderY > position.Y - (size.Y * ROOM_HEIGHT); borderY--)
	{
		// Create an object for the tile, set its sprite, and position it in the world
		// Do this for the left and right sides of the level
		std::shared_ptr<GameObject> leftObj = Application::Current->CreateObject<GameObject>();
		std::shared_ptr<SpriteComponent> leftSprComp = leftObj->AddComponent<SpriteComponent>();
		leftSprComp->Sprite = s_TileSprites[1];
		leftObj->Position.X = position.X - 1;
		leftObj->Position.Y = borderY;
		std::shared_ptr<GameObject> rightObj = Application::Current->CreateObject<GameObject>();
		std::shared_ptr<SpriteComponent> rightSprComp = rightObj->AddComponent<SpriteComponent>();
		rightSprComp->Sprite = s_TileSprites[1];
		rightObj->Position.X = position.X + (size.Y * ROOM_WIDTH);
		rightObj->Position.Y = borderY;
	}

	// Get the room codes
	std::vector<int> roomCodes = GenerateLevel(Vector(size.X, size.Y));

	// Build the rooms
	for (int levelY = 0; levelY < size.Y; levelY++)
	{
		for (int levelX = 0; levelX < size.X; levelX++)
		{
			std::vector<std::filesystem::path>* filePathList;
			switch (roomCodes[levelX + (size.X * levelY)])
			{
			case 0:
				filePathList = &s_StandardPaths;
				break;
			case 1:
				filePathList = &s_DropPaths;
				break;
			case 2:
				filePathList = &s_CatchPaths;
				break;
			case 3:
				filePathList = &s_StartPaths;
				break;
			case 4:
				filePathList = &s_EndPaths;
				break;
			default:
				filePathList = &s_StandardPaths;
				break;
			}

			std::uniform_int_distribution<> distr(0, filePathList->size() - 1);
			std::string filePath = (*filePathList)[distr(s_RandomGenerator)].string();
			std::ifstream file(filePath, std::ios::binary);

			if (!file)
			{
				VX_ERROR("Could not read file at location: " + filePath);
				return;
			}

			// Build the room from the room file
			for (int y = 0; y < ROOM_HEIGHT; y++)
			{
				for (int x = 0; x < ROOM_WIDTH; x++)
				{
					// Read a byte from the file
					unsigned char byte;
					file.read(reinterpret_cast<char*>(&byte), sizeof(byte));

					// Skip the tile if its empty
					if (byte == 0x00)
						continue;

					// Create an object for the tile, set its sprite, and position it in the world
					std::shared_ptr<GameObject> obj = Application::Current->CreateObject<GameObject>();
					std::shared_ptr<SpriteComponent> sprComp = obj->AddComponent<SpriteComponent>();
					sprComp->Sprite = s_TileSprites[byte];
					obj->Position.X = xOffset + x;
					obj->Position.Y = yOffset - y;
				}
			}

			file.close();

			xOffset += ROOM_WIDTH;
		}
		xOffset = position.X;
		yOffset -= ROOM_HEIGHT;
	}
}

std::vector<int> Level::GenerateLevel(Vector size)
{
	std::vector<int> codes = std::vector<int>(size.X * size.Y);
	std::uniform_int_distribution<> distr(0, size.X - 1);

	// Add our start room
	int codeIndex = distr(s_RandomGenerator);
	codes[codeIndex] = 3;

	// Add our drop and catch rooms
	for (int y = 0; y < size.Y - 1; y++)
	{
		// Find an available room
		while (codes[codeIndex] != 0)
			codeIndex = distr(s_RandomGenerator) + (size.X * y);

		codes[codeIndex] = 1;
		codes[codeIndex + size.X] = 2;
	}

	// Set the end room
	while (codes[codeIndex] != 0)
		codeIndex = distr(s_RandomGenerator) + (size.X * (size.Y - 1));
	codes[codeIndex] = 4;

	return codes;
}

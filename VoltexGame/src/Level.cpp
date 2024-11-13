#include "Level.h"

#include <fstream>

bool Level::s_LevelDataInitialized = false;
std::shared_ptr<Sprite> Level::s_DirtBlockSprite;
std::shared_ptr<Sprite> Level::s_GrassBlockSprite;
std::shared_ptr<Sprite> Level::s_LogDecorSprite;
std::shared_ptr<Sprite> Level::s_VineDecorSprite;
std::shared_ptr<Sprite> Level::s_SlimeEnemySprite;
std::shared_ptr<Sprite> Level::s_BeeEnemySprite;
std::vector<std::filesystem::path> Level::s_StandardPaths;
std::vector<std::filesystem::path> Level::s_DropPaths;
std::vector<std::filesystem::path> Level::s_CatchPaths;
std::vector<std::filesystem::path> Level::s_StartPaths;
std::vector<std::filesystem::path> Level::s_EndPaths;
std::mt19937 Level::s_RandomGenerator;

Level::Level(const Vector& position, Vector& size)
	: m_Postion(position), m_Size(size)
{
	// Make sure our level is at least 2x2
	if (m_Size.X < 2)
		m_Size.X = 2;
	if (m_Size.Y < 2)
		m_Size.Y = 2;

	// The first time we create a level, initialize data levels rely on like sprites and room file paths
	if (!s_LevelDataInitialized)
	{
		s_DirtBlockSprite = Sprite::Create("textures/tiles/DirtBlock.png");
		s_GrassBlockSprite = Sprite::Create("textures/tiles/GrassBlock.png");
		s_LogDecorSprite = Sprite::Create("textures/tiles/LogDecor.png");
		s_VineDecorSprite = Sprite::Create("textures/tiles/VineDecor.png");
		s_SlimeEnemySprite = Sprite::Create("textures/tiles/SlimeEnemy.png");
		s_BeeEnemySprite = Sprite::Create("textures/tiles/BeeEnemy.png");

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
}

void Level::Generate(Vector* outStartPos, Vector* outEndPos)
{
	// Build the border
	for (int borderX = m_Postion.X - 1; borderX < ((m_Postion.X + (m_Size.X * ROOM_WIDTH)) + 1); borderX++)
	{
		// Create an object for the tile, set its sprite, and position it in the world
		// Do this for the top and bottom of the level
		std::shared_ptr<GameObject> topObj = GameObject::Create<GameObject>();
		std::shared_ptr<SpriteComponent> topSprComp = topObj->AddComponent<SpriteComponent>();
		topSprComp->Sprite = s_DirtBlockSprite;
		topObj->AddComponent<CollisionComponent>();
		topObj->Position.X = borderX;
		topObj->Position.Y = m_Postion.Y + 1;
		std::shared_ptr<GameObject> bottomObj = GameObject::Create<GameObject>();
		std::shared_ptr<SpriteComponent> bottomSprComp = bottomObj->AddComponent<SpriteComponent>();
		bottomSprComp->Sprite = s_DirtBlockSprite;
		bottomObj->AddComponent<CollisionComponent>();
		bottomObj->Position.X = borderX;
		bottomObj->Position.Y = m_Postion.Y - (m_Size.Y * ROOM_HEIGHT);
	}
	for (int borderY = m_Postion.Y; borderY > m_Postion.Y - (m_Size.Y * ROOM_HEIGHT); borderY--)
	{
		// Create an object for the tile, set its sprite, and position it in the world
		// Do this for the left and right sides of the level
		std::shared_ptr<GameObject> leftObj = GameObject::Create<GameObject>();
		std::shared_ptr<SpriteComponent> leftSprComp = leftObj->AddComponent<SpriteComponent>();
		leftSprComp->Sprite = s_DirtBlockSprite;
		leftObj->AddComponent<CollisionComponent>();
		leftObj->Position.X = m_Postion.X - 1;
		leftObj->Position.Y = borderY;
		std::shared_ptr<GameObject> rightObj = GameObject::Create<GameObject>();
		std::shared_ptr<SpriteComponent> rightSprComp = rightObj->AddComponent<SpriteComponent>();
		rightSprComp->Sprite = s_DirtBlockSprite;
		rightObj->AddComponent<CollisionComponent>();
		rightObj->Position.X = m_Postion.X + (m_Size.Y * ROOM_WIDTH);
		rightObj->Position.Y = borderY;
	}

	// Get the room codes
	std::vector<int> roomCodes = GenerateMap(Vector(m_Size.X, m_Size.Y));

	int xOffset = m_Postion.X;
	int yOffset = m_Postion.Y;
	Vector spawnPoint;

	// Build the rooms
	for (int levelY = 0; levelY < m_Size.Y; levelY++)
	{
		for (int levelX = 0; levelX < m_Size.X; levelX++)
		{
			std::vector<std::filesystem::path>* filePathList;
			switch (roomCodes[levelX + (m_Size.X * levelY)])
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
				*outStartPos = Vector(xOffset + (ROOM_WIDTH / 2) - 0.5f, yOffset - ROOM_HEIGHT + 2.5f);
				break;
			case 4:
				filePathList = &s_EndPaths;
				*outEndPos = Vector(xOffset + (ROOM_WIDTH / 2) - 0.5f, yOffset - ROOM_HEIGHT + 2.5f);
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

					if (byte == 0x00)
						continue;

					// 50% chance to skip object creation on a chance tile
					if (byte > 0x02)
					{
						std::uniform_int_distribution<> distr(0, 1);
						if (distr(s_RandomGenerator) == 0)
							continue;
					}

					// Create an object for the tile
					std::shared_ptr<GameObject> obj = GameObject::Create<GameObject>();
					std::shared_ptr<SpriteComponent> sprComp = obj->AddComponent<SpriteComponent>();
					obj->AddComponent<CollisionComponent>();

					// Set its sprite
					switch (byte)
					{
					case 0x01:
						sprComp->Sprite = s_DirtBlockSprite;
						break;
					case 0x02:
						sprComp->Sprite = s_GrassBlockSprite;
						break;
					case 0x03:
						sprComp->Sprite = s_DirtBlockSprite;
						break;
					case 0x04:
						sprComp->Sprite = s_GrassBlockSprite;
						break;
					case 0x05:
						sprComp->Sprite = s_LogDecorSprite;
						break;
					case 0x06:
						sprComp->Sprite = s_VineDecorSprite;
						break;
					case 0x07:
						sprComp->Sprite = s_SlimeEnemySprite;
						break;
					case 0x08:
						sprComp->Sprite = s_BeeEnemySprite;
						break;
					}

					// Set its position in the world
					obj->Position.X = xOffset + x;
					obj->Position.Y = yOffset - y;
				}
			}

			file.close();

			xOffset += ROOM_WIDTH;
		}
		xOffset = m_Postion.X;
		yOffset -= ROOM_HEIGHT;
	}
}

std::vector<int> Level::GenerateMap(Vector size)
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

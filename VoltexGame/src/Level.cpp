#include "Level.h"

#include <fstream>
#include <random>

std::vector<std::shared_ptr<Sprite>> Level::s_TileSprites;

Level::Level(const Vector& position, const Vector& size)
{
	// If we haven't already, load our sprites
	if (s_TileSprites.size() == 0)
	{
		s_TileSprites.push_back(nullptr);
		s_TileSprites.push_back(Application::Current->CreateSprite("../VoltexGame/textures/tiles/DirtBlock.png"));
	}

	int xOffset = position.X;
	int yOffset = position.Y;

	// Build the border
	for (int borderX = position.X - 1; borderX < ((position.X + (size.X * ROOM_WIDTH)) + 1); borderX++)
	{
		// Create an object for the tile, set its sprite, and position it in the world
		// Do this for the top and bottom of the level
		std::shared_ptr<GameObject> topObj = Application::Current->CreateObject<GameObject>();
		topObj->ObjectSprite = s_TileSprites[1];
		topObj->Position.X = borderX;
		topObj->Position.Y = position.Y + 1;
		std::shared_ptr<GameObject> bottomObj = Application::Current->CreateObject<GameObject>();
		bottomObj->ObjectSprite = s_TileSprites[1];
		bottomObj->Position.X = borderX;
		bottomObj->Position.Y = position.Y - (size.Y * ROOM_HEIGHT);
	}
	for (int borderY = position.Y; borderY > position.Y - (size.Y * ROOM_HEIGHT); borderY--)
	{
		// Create an object for the tile, set its sprite, and position it in the world
		// Do this for the left and right sides of the level
		std::shared_ptr<GameObject> leftObj = Application::Current->CreateObject<GameObject>();
		leftObj->ObjectSprite = s_TileSprites[1];
		leftObj->Position.X = position.X - 1;
		leftObj->Position.Y = borderY;
		std::shared_ptr<GameObject> rightObj = Application::Current->CreateObject<GameObject>();
		rightObj->ObjectSprite = s_TileSprites[1];
		rightObj->Position.X = position.X + (size.Y * ROOM_WIDTH);
		rightObj->Position.Y = borderY;
	}

	// Build the rooms
	for (int levelY = 0; levelY < size.Y; levelY++)
	{
		for (int levelX = 0; levelX < size.X; levelX++)
		{
			std::string filePath = "rooms/BasicAcross.bke";
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
					obj->ObjectSprite = s_TileSprites[byte];
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

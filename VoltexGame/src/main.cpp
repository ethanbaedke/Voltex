#include <VECore.h>

#include <fstream>

using namespace VoltexEngine;

static std::vector<std::shared_ptr<Sprite>> TileSprites;

/* Lets create a class from the Application class, which handels creating and updating a window */
class Game : public Application
{

public:

	Game()
	{
		TileSprites.push_back(nullptr);
		TileSprites.push_back(CreateSprite("../VoltexGame/textures/tiles/DirtBlock.png"));
		TileSprites.push_back(CreateSprite("../VoltexGame/textures/tiles/StoneBlock.png"));

		std::string filePath = "rooms/StoneBasicAcross.bke";
		std::ifstream file(filePath, std::ios::binary);

		if (!file)
		{
			VX_ERROR("Could not read file at location: " + filePath);
			return;
		}

		// Read the first two integers of the file which tell us the width and height of the room
		int width, height;
		file.read(reinterpret_cast<char*>(&width), sizeof(width));
		file.read(reinterpret_cast<char*>(&height), sizeof(height));

		// Read the file byte by byte and fill in tiles at the filled locations
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				unsigned char byte;
				file.read(reinterpret_cast<char*>(&byte), sizeof(byte));

				if (byte == 0x00)
					continue;

				std::shared_ptr<GameObject> obj = CreateObject<GameObject>();
				obj->ObjectSprite = TileSprites[byte];
				obj->Position.X = x;
				obj->Position.Y = -y;
			}
		}

		file.close();
		VX_LOG("Read file from location: " + filePath);
	}

};

int main()
{
	// In our main function, we create an instance of our game application, and we run it
	std::unique_ptr<Game> game = std::make_unique<Game>();
	game->Run();

	return 0;
}
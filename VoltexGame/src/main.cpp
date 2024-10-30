#include <VECore.h>

#include "Level.h"

using namespace VoltexEngine;

/* Lets create a class from the Application class, which handels creating and updating a window */
class Game : public Application
{

public:

	Game()
	{
		// Create a level
		Level level(Vector(0, 0), Vector(4, 4));
	}

};

int main()
{
	// In our main function, we create an instance of our game application, and we run it
	std::unique_ptr<Game> game = std::make_unique<Game>();
	game->Run();

	return 0;
}
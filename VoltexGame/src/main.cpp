#include <VoltexEngine.h>

class Game : public VoltexEngine::Application
{
};

int main()
{
	// Make the engine
	std::unique_ptr<Game> game = std::make_unique<Game>();
	game->Run();

	return 0;
}
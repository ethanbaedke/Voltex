#include "Engine.h"
#include <memory>

int main()
{
	// Make the engine
	std::unique_ptr<VoltexEngine::Engine> engine = std::make_unique<VoltexEngine::Engine>();
	engine->Run();

	return 0;
}
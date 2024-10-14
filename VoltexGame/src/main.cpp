#include <VoltexEngine.h>

class ExampleObject : public VoltexEngine::GameObject
{

public:

	virtual void Initialize() override
	{
		VX_LOG("Init");
	}

	virtual void Update(float deltaTime) override
	{
		VX_LOG("Update");
	}

};

class Game : public VoltexEngine::Application
{

private:

	std::shared_ptr<VoltexEngine::GameObject> testObject;

public:

	Game()
	{
		testObject = ExampleObject::Create<ExampleObject>();
	}

};

int main()
{
	// Make and run the engine
	std::unique_ptr<Game> game = std::make_unique<Game>();
	game->Run();

	return 0;
}
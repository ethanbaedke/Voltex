#include <VoltexEngine.h>

class ExampleObject : public VoltexEngine::GameObject
{
};

class Game : public VoltexEngine::Application
{

private:

	std::shared_ptr<VoltexEngine::Sprite> testSprite1;
	std::shared_ptr<VoltexEngine::GameObject> testObject1;
	std::shared_ptr<VoltexEngine::GameObject> testObject2;

public:

	Game()
	{
		testSprite1 = VoltexEngine::Sprite::Create("textures/Sixteen.png");
		testObject1 = ExampleObject::Create<ExampleObject>();
		testObject1->SetSprite(testSprite1);

		testObject2 = ExampleObject::Create<ExampleObject>();
		testObject2->SetPosition(VoltexEngine::Vector(2.0f, 0.0f));
		testObject2->SetSprite(testSprite1);
	}

};

int main()
{
	// Make and run the engine
	std::unique_ptr<Game> game = std::make_unique<Game>();
	game->Run();

	return 0;
}
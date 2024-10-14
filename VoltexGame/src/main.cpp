#include <VoltexEngine.h>

class ExampleObject : public VoltexEngine::GameObject
{
};

class Game : public VoltexEngine::Application
{

private:

	std::shared_ptr<VoltexEngine::Sprite> testSprite1;
	std::shared_ptr<VoltexEngine::GameObject> testObject1;
	std::shared_ptr<VoltexEngine::Sprite> testSprite2;
	std::shared_ptr<VoltexEngine::GameObject> testObject2;

public:

	Game()
	{
		testSprite1 = VoltexEngine::Sprite::Create("textures/Checkerboard.png");
		testSprite2 = VoltexEngine::Sprite::Create("textures/MyBuds.png");

		testObject1 = ExampleObject::Create<ExampleObject>();
		testObject1->SetPosition(VoltexEngine::Vector(-0.25, 0));
		testObject1->SetScale(VoltexEngine::Vector(0.5));
		testObject1->SetRotation(45.0f);
		testObject1->SetSprite(testSprite1);

		testObject2 = ExampleObject::Create<ExampleObject>();
		testObject2->SetPosition(VoltexEngine::Vector(0.25, 0));
		testObject2->SetScale(VoltexEngine::Vector(0.75));
		testObject2->SetRotation(-30.0f);
		testObject2->SetSprite(testSprite2);
	}

};

int main()
{
	// Make and run the engine
	std::unique_ptr<Game> game = std::make_unique<Game>();
	game->Run();

	return 0;
}
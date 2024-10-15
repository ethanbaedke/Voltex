#include <VoltexEngine.h>
#include <random>

namespace VE = VoltexEngine;

/* This is an example of subclassing a GameObject */
class ExampleObject : public VE::GameObject
{

private:

	// These are some variables we will use in our example object below
	VE::Vector m_InitialPosition;
	float m_TimeElapsed = 0.0f;
	float m_BounceSpeed = 2.0f;

public:

	/* This function is called once at the beginning of the frame following the creation of this object */
	virtual void Initialize() override
	{
		// Lets position our object at a random point a few units from the origin
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(-2.0, 2.0);
		float randomX = dis(gen);
		float randomY = dis(gen);
		
		// Here we create a vector holding the random x and y values we generated
		VE::Vector randomPosition(randomX, randomY);

		// We call the SetPosition function on our game object, passing in the vector we just made
		SetPosition(randomPosition);

		// Lets store our initial position so we can do some fun things with it in the Update function
		m_InitialPosition = randomPosition;
	}

	/* This function is called once every frame, and takes in the amount of time since the last frame begun */
	virtual void Update(float deltaTime) override
	{
		// Lets have our object bounce back and fourth on the x-axis
		m_TimeElapsed += deltaTime;
		VE::Vector newPosition(m_InitialPosition.X() + sin(m_TimeElapsed * m_BounceSpeed), m_InitialPosition.Y());
		SetPosition(newPosition);
	}

};

/* This is and example of a game application */
class Game : public VE::Application
{

private:

	std::vector<std::shared_ptr<VE::Sprite>> m_Sprites;
	std::vector<std::shared_ptr<VE::GameObject>> m_GameObjects;

public:

	Game()
	{
		// Create some sprites
		std::shared_ptr<VE::Sprite> barrelSprite = VE::Sprite::Create("textures/Barrel.png");
		m_Sprites.push_back(barrelSprite);

		std::shared_ptr<VE::Sprite> presentSprite = VE::Sprite::Create("textures/Present.png");
		m_Sprites.push_back(presentSprite);

		// Create some instances of our custom game object
		std::shared_ptr<ExampleObject> barrel = ExampleObject::Create<ExampleObject>();
		barrel->SetSprite(barrelSprite);
		m_GameObjects.push_back(barrel);

		std::shared_ptr<ExampleObject> present = ExampleObject::Create<ExampleObject>();
		present->SetSprite(presentSprite);
		m_GameObjects.push_back(present);
	}

};

int main()
{
	// Make and run the engine
	std::unique_ptr<Game> game = std::make_unique<Game>();
	game->Run();

	return 0;
}
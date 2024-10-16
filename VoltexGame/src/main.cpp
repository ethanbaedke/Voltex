#include <VoltexEngine.h>
#include <random>

using namespace VoltexEngine;

/* This is an example of subclassing a GameObject */
class ExampleObject : public GameObject
{

private:

	// These are some variables we will use in our example object below
	Vector m_InitialPosition;
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
		float randomX = (float)dis(gen);
		float randomY = (float)dis(gen);
		
		// Here we create a vector holding the random x and y values we generated
		Vector randomPosition(randomX, randomY);

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
		Vector newPosition(m_InitialPosition.X() + sin(m_TimeElapsed * m_BounceSpeed), m_InitialPosition.Y());
		SetPosition(newPosition);
	}

};

/* This is and example of a game application */
class Game : public Application
{

public:

	Game()
	{
		// Create some sprites
		std::shared_ptr<Sprite> barrelSprite = CreateSprite("textures/Barrel.png");
		std::shared_ptr<Sprite> presentSprite = CreateSprite("textures/Present.png");
		std::shared_ptr<Sprite> crateSprite = CreateSprite("textures/Crate.png");

		// Create some instances of our custom game object
		std::shared_ptr<ExampleObject> barrel = CreateObject<ExampleObject>();
		barrel->SetSprite(barrelSprite);

		std::shared_ptr<ExampleObject> present = CreateObject<ExampleObject>();
		present->SetSprite(presentSprite);

		std::shared_ptr<ExampleObject> crate = CreateObject<ExampleObject>();
		crate->SetSprite(crateSprite);

		// Here we tell our present to render in front, our barrel behind it, and our crate behind that
		present->SetDepth(1);
		barrel->SetDepth(0);
		crate->SetDepth(-1);
	}

};

int main()
{
	// Make and run the engine
	std::unique_ptr<Game> game = std::make_unique<Game>();
	game->Run();

	return 0;
}
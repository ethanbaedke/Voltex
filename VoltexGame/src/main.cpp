#include <VoltexEngine.h>
#include <random>

using namespace VoltexEngine;

/* Lets make an object that bounces left and right on the screen */
class BounceObject : public GameObject
{

private:

	// Here we track the initial position of our object and the speed at which it should bounce
	float m_InitialX = 0.0f;
	float m_BounceSpeed = 2.0f;

	// Here we track how much time has passed since the game began
	float m_TimeElapsed = 0.0f;

public:

	/* This function is called once at the beginning of the frame following the creation of this object */
	virtual void Initialize() override
	{
		// Make sure to initialize the base class
		GameObject::Initialize();

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
		m_InitialX = randomX;
	}

	/* This function is called once every frame, and takes in the amount of time since the last frame begun */
	virtual void Update(float deltaTime) override
	{
		// Make sure to update the base class
		GameObject::Update(deltaTime);

		// Lets have our object bounce back and fourth on the x-axis
		m_TimeElapsed += deltaTime;
		Vector currentPosition = GetPosition();
		Vector newPosition(m_InitialX + sin(m_TimeElapsed * m_BounceSpeed), currentPosition.Y());
		SetPosition(newPosition);
	}

};

/* Lets subclass our BounceObject and add functionality to move it up and down with the keyboard */
class MoveObject : public BounceObject
{

private:

	// Here we track the direction and speed our object should move on the y-axis
	int m_YDir = 0;
	float m_Speed = 2.0f;

	// Here we track which of our inputs are held down
	int m_UpHeld = 0;
	int m_DownHeld = 0;

public:

	virtual void Update(float deltaTime) override
	{
		BounceObject::Update(deltaTime);

		// Here we check for inputs, and set our y-direction accordingly
		if (Input::KeyDown(KeyCode::W) || Input::KeyDown(KeyCode::UpArrow))
		{
			m_UpHeld = 1;
		}
		else if (Input::KeyUp(KeyCode::W) || Input::KeyUp(KeyCode::UpArrow))
		{
			m_UpHeld = 0;
		}
		if (Input::KeyDown(KeyCode::S) || Input::KeyDown(KeyCode::DownArrow))
		{
			m_DownHeld = 1;
		}
		else if (Input::KeyUp(KeyCode::S) || Input::KeyUp(KeyCode::DownArrow))
		{
			m_DownHeld = 0;
		}

		// Here we calculate our y-direction based on our inputs and move in that direction by our specified speed
		m_YDir = m_UpHeld - m_DownHeld;
		Vector currentPosition = GetPosition();
		Vector newPosition = Vector(currentPosition.X(), currentPosition.Y() + (m_YDir * m_Speed * deltaTime));
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

		// Create some instances of our custom game objects
		std::shared_ptr<BounceObject> barrel = CreateObject<BounceObject>();
		barrel->SetSprite(barrelSprite);

		std::shared_ptr<BounceObject> present = CreateObject<BounceObject>();
		present->SetSprite(presentSprite);

		std::shared_ptr<MoveObject> crate = CreateObject<MoveObject>();
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
#include <VECore.h>
#include <random>

using namespace VoltexEngine;

/*
* Voltex is a small engine that allows sprites to be bound to customizable objects and keyboard input to be handeled
* Below is an example of a small application that has been made using classes within the engine
*/

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

		// Here we set the position of our game object using our randomly generated x and y values
		Position = Vector(randomX, randomY);

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
		Vector newPosition(m_InitialX + sin(m_TimeElapsed * m_BounceSpeed), Position.Y);
		Position = newPosition;
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
		Vector newPosition = Vector(Position.X, Position.Y + (m_YDir * m_Speed * deltaTime));
		Position = newPosition;
	}
};

/* Lets create a class from the Application class, which handels creating and updating a window */
class Game : public Application
{

public:

	Game()
	{
		// Here we create some sprites from images in our "textures" folder
		std::shared_ptr<Sprite> barrelSprite = CreateSprite("textures/Barrel.png");
		std::shared_ptr<Sprite> presentSprite = CreateSprite("textures/Present.png");
		std::shared_ptr<Sprite> crateSprite = CreateSprite("textures/Crate.png");

		// Here we create some instances of our custom game objects
		std::shared_ptr<BounceObject> barrel = CreateObject<BounceObject>();
		barrel->Sprite = barrelSprite;

		std::shared_ptr<BounceObject> present = CreateObject<BounceObject>();
		present->Sprite = presentSprite;

		std::shared_ptr<MoveObject> crate = CreateObject<MoveObject>();
		crate->Sprite = crateSprite;

		// Here we tell our objects to render in front or behind each other
		present->Depth = 1;
		barrel->Depth = 0;
		crate->Depth = -1;
	}

};

int main()
{
	// In our main function, we create an instance of our game application, and we run it
	std::unique_ptr<Game> game = std::make_unique<Game>();
	game->Run();

	return 0;
}
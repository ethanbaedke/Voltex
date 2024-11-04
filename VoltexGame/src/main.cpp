#include <VXCore.h>

#include "Level.h"

using namespace VoltexEngine;

std::shared_ptr<Sprite> playerSpr;

class Player : public GameObject
{

private:

	std::shared_ptr<SpriteComponent> m_SprComp;
	std::shared_ptr<CollisionComponent> m_ColComp;

	Vector m_Input;

	float m_Speed;

public:

	Player()
		: m_SprComp(nullptr), m_ColComp(nullptr), m_Input(Vector(0, 0)), m_Speed(1.0f)
	{
	}

	virtual void Initialize() override
	{
		GameObject::Initialize();

		m_SprComp = AddComponent<SpriteComponent>();
		m_SprComp->Sprite = playerSpr;
		m_ColComp = AddComponent<CollisionComponent>();
		m_ColComp->IsDynamic = true;
		m_ColComp->OnCollision.AddCallback([&](std::shared_ptr<CollisionComponent> otherColComp) { OnCollisionCallback(otherColComp); });
	}

	virtual void Update(float deltaTime) override
	{
		GameObject::Update(deltaTime);

		if (Input::KeyDown(KeyCode::LeftArrow))
			m_Input.X--;
		else if (Input::KeyUp(KeyCode::LeftArrow))
			m_Input.X++;

		if (Input::KeyDown(KeyCode::RightArrow))
			m_Input.X++;
		else if (Input::KeyUp(KeyCode::RightArrow))
			m_Input.X--;

		if (Input::KeyDown(KeyCode::DownArrow))
			m_Input.Y--;
		else if (Input::KeyUp(KeyCode::DownArrow))
			m_Input.Y++;

		if (Input::KeyDown(KeyCode::UpArrow))
			m_Input.Y++;
		else if (Input::KeyUp(KeyCode::UpArrow))
			m_Input.Y--;

		Position += (m_Input * deltaTime);
	}

private:

	void OnCollisionCallback(std::shared_ptr<CollisionComponent> otherColComp)
	{
		//VX_LOG("Player Collision Detected");
	}

};

/* Lets create a class from the Application class, which handels creating and updating a window */
class Game : public Application
{

public:

	Game()
	{
		playerSpr = Sprite::Create("textures/Player.png");

		// Create a level
		Level level(Vector(0, 0), Vector(4, 4));
		std::shared_ptr<Player> player = GameObject::Create<Player>();
		player->Depth = 1;
	}

};

int main()
{
	// In our main function, we create an instance of our game application, and we run it
	std::unique_ptr<Game> game = std::make_unique<Game>();
	game->Run();

	return 0;
}

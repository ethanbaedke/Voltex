#include <VXCore.h>
#include <VXUI.h>

#include <fstream>

using namespace VoltexEngine;

static std::vector<std::shared_ptr<Sprite>> TileSprites;
static int SelectionIndex = 0;

class RoomTile : public ButtonGizmo
{
	
public:

	unsigned char TileIndex;

public:

	RoomTile()
		: TileIndex(0)
	{
		OnButtonPressed.AddCallback([&] { HandleButtonPressed(); });
	}

	virtual void Tick() override
	{
		ButtonGizmo::Tick();

		UISprite = TileSprites[TileIndex];
	}

private:

	void HandleButtonPressed()
	{
		TileIndex = SelectionIndex;
	}

};

class Tool : public Application
{

private:

	static const int ROOM_WIDTH = 20;
	static const int ROOM_HEIGHT = 10;

private:

	std::shared_ptr<GridLayoutGizmo> m_RoomEditor;

public:

	Tool()
	{
		TileSprites.push_back(nullptr);
		TileSprites.push_back(Sprite::Create("textures/DirtBlock.png"));
		TileSprites.push_back(Sprite::Create("textures/GrassBlock.png"));
		TileSprites.push_back(Sprite::Create("textures/StoneBlock.png"));

		// Make the base gizmo that will hold all other gizmos
		std::shared_ptr<HorizontalLayoutGizmo> canvas = Gizmo::Create<HorizontalLayoutGizmo>();

		// Tile selection layout will be vertical on the left side of the screen
		std::shared_ptr<VerticalLayoutGizmo> tileSelector = Gizmo::Create<VerticalLayoutGizmo>();
		canvas->AddChild(tileSelector);

		// Populate tile selection with buttons
		for (int i = 0; i < 4; i++)
		{
			std::shared_ptr<ButtonGizmo> selectionTile = Gizmo::Create<ButtonGizmo>();
			tileSelector->AddChild(selectionTile);
			selectionTile->UISprite = TileSprites[i];
			selectionTile->OnButtonPressed.AddCallback([&, i] { Select(i); });
		}

		// Layout to hold the room editor and save/load buttons on the right side of the screen
		std::shared_ptr<VerticalLayoutGizmo> vertLayout = Gizmo::Create<VerticalLayoutGizmo>();
		vertLayout->Weight = 15;
		canvas->AddChild(vertLayout);

		// The grid layout that will hold the RoomTile buttons for room editing
		m_RoomEditor = Gizmo::Create<GridLayoutGizmo>();
		m_RoomEditor->SetNumSlots(ROOM_WIDTH, ROOM_HEIGHT);
		m_RoomEditor->Weight = 3;
		vertLayout->AddChild(m_RoomEditor);

		// Populate the room editor with the RoomTile buttons
		for (int i = 0; i < ROOM_WIDTH * ROOM_HEIGHT; i++)
		{
			std::shared_ptr<RoomTile> roomTile = Gizmo::Create<RoomTile>();
			m_RoomEditor->AddChild(roomTile);
		}

		// The horizontal layout for saving/loading buttons
		std::shared_ptr<HorizontalLayoutGizmo> fileSelector = Gizmo::Create<HorizontalLayoutGizmo>();
		vertLayout->AddChild(fileSelector);

		// Save layout to hold button and text
		std::shared_ptr<OverlapLayoutGizmo> saveLayout = Gizmo::Create<OverlapLayoutGizmo>();
		fileSelector->AddChild(saveLayout);

		// Save button
		std::shared_ptr<ButtonGizmo> saveButton = Gizmo::Create<ButtonGizmo>();
		saveButton->OnButtonPressed.AddCallback([&] { Save(); });
		saveLayout->AddChild(saveButton);

		// Save text
		std::shared_ptr<TextGizmo> saveText = Gizmo::Create<TextGizmo>(); saveText->SetColor(Color(0, 0, 128, 255));
		saveText->Text = "SAVE ROOM";
		saveLayout->AddChild(saveText);

		// Load layout to hold button and text
		std::shared_ptr<OverlapLayoutGizmo> loadLayout = Gizmo::Create<OverlapLayoutGizmo>();
		fileSelector->AddChild(loadLayout);

		// Load button
		std::shared_ptr<ButtonGizmo> loadButton = Gizmo::Create<ButtonGizmo>();
		loadButton->OnButtonPressed.AddCallback([&] { Load(); });
		loadLayout->AddChild(loadButton);

		// Load text
		std::shared_ptr<TextGizmo> loadText = Gizmo::Create<TextGizmo>(); loadText->SetColor(Color(0, 0, 128, 255));
		loadText->Text = "LOAD ROOM";
		loadLayout->AddChild(loadText);
	}

private:

	void Save()
	{
		const char* filePath = SaveFileDialog();

		// Return if the user cancelled a file selection
		if (!filePath)
			return;

		std::ofstream file(filePath, std::ios::binary);

		if (!file)
		{
			VX_ERROR("Could not write to location: " + filePath);
			return;
		}

		// Save the RoomTiles indices as bytes in the room file
		std::vector<std::shared_ptr<Gizmo>> tiles = m_RoomEditor->GetChildren();	
		for (std::shared_ptr<Gizmo> giz : tiles)
			if (std::shared_ptr<RoomTile> tile = std::static_pointer_cast<RoomTile>(giz))
				file.write(reinterpret_cast<const char*>(&(tile->TileIndex)), sizeof(unsigned char));

		file.close();
		VX_LOG("Saved room to: " + filePath);
	}

	void Load()
	{
		const char* filePath = LoadFileDialog();

		// Return if the user cancelled a file selection
		if (!filePath)
			return;

		std::ifstream file(filePath, std::ios::binary);

		if (!file)
		{
			VX_ERROR("Could not write to location: " + filePath);
			return;
		}

		// Read the bytes of the file into the indices of the RoomTiles
		std::vector<std::shared_ptr<Gizmo>> tiles = m_RoomEditor->GetChildren();
		for (std::shared_ptr<Gizmo> giz : tiles)
			if (std::shared_ptr<RoomTile> tile = std::static_pointer_cast<RoomTile>(giz))
				file.read(reinterpret_cast<char*>(&(tile->TileIndex)), sizeof(unsigned char));

		file.close();
		VX_LOG("Loaded room from file: " + filePath);
	}

	void Select(int index)
	{
		SelectionIndex = index;
	}

};

int main()
{
	// Make and run the tool
	std::unique_ptr<Tool> tool = std::make_unique<Tool>();
	tool->Run();

	return 0;
}
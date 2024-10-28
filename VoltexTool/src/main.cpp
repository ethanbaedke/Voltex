#include <VECore.h>
#include <VEUI.h>

#include <fstream>

using namespace VoltexEngine;

static std::shared_ptr<Sprite> BlockSprite;

class RoomTile : public ButtonGizmo
{
	
public:

	bool Filled;

public:

	RoomTile()
		: Filled(false)
	{
		OnButtonPressed.AddCallback([&] { HandleButtonPressed(); });
	}

	virtual void Tick() override
	{
		ButtonGizmo::Tick();

		if (Filled)
			UISprite = BlockSprite;
		else
			UISprite = nullptr;
	}

private:

	void HandleButtonPressed()
	{
		Filled = !Filled;
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
		: m_RoomEditor(nullptr)
	{
		BlockSprite = CreateSprite("textures/Block.png");

		// Make the base gizmo that will hold all other gizmos
		std::shared_ptr<HorizontalLayoutGizmo> canvas = CreateGizmo<HorizontalLayoutGizmo>();

		// Tile selection layout will be vertical on the left side of the screen
		std::shared_ptr<VerticalLayoutGizmo> tileSelector = CreateGizmo<VerticalLayoutGizmo>();
		canvas->AddChild(tileSelector);

		// Populate tile selection with buttons
		for (int i = 0; i < 10; i++)
		{
			std::shared_ptr<ButtonGizmo> selectionTile = CreateGizmo<ButtonGizmo>();
			tileSelector->AddChild(selectionTile);
		}

		// Layout to hold the room editor and save/load buttons on the right side of the screen
		std::shared_ptr<VerticalLayoutGizmo> vertLayout = CreateGizmo<VerticalLayoutGizmo>();
		vertLayout->Weight = 15;
		canvas->AddChild(vertLayout);

		// The grid layout that will hold the RoomTile buttons for room editing
		m_RoomEditor = CreateGizmo<GridLayoutGizmo>();
		m_RoomEditor->SetNumSlots(ROOM_WIDTH, ROOM_HEIGHT);
		m_RoomEditor->Weight = 3;
		vertLayout->AddChild(m_RoomEditor);

		// Populate the room editor with the RoomTile buttons
		for (int i = 0; i < ROOM_WIDTH * ROOM_HEIGHT; i++)
		{
			std::shared_ptr<RoomTile> roomTile = CreateGizmo<RoomTile>();
			m_RoomEditor->AddChild(roomTile);
		}

		// The horizontal layout for saving/loading buttons
		std::shared_ptr<HorizontalLayoutGizmo> fileSelector = CreateGizmo<HorizontalLayoutGizmo>();
		vertLayout->AddChild(fileSelector);

		// Save button
		std::shared_ptr<ButtonGizmo> save = CreateGizmo<ButtonGizmo>();
		save->OnButtonPressed.AddCallback([&] { Save(); });
		fileSelector->AddChild(save);

		// Load button
		std::shared_ptr<ButtonGizmo> load = CreateGizmo<ButtonGizmo>();
		load->OnButtonPressed.AddCallback([&] { Load(); });
		fileSelector->AddChild(load);
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

		// The first two pieces of data are integers for the width of the room, followed by the height of the room
		file.write(reinterpret_cast<const char*>(&ROOM_WIDTH), sizeof(ROOM_WIDTH));
		file.write(reinterpret_cast<const char*>(&ROOM_HEIGHT), sizeof(ROOM_HEIGHT));

		// Save the room as a straight shot of bytes representing whether there is a solid block or not
		std::vector<std::shared_ptr<Gizmo>> tiles = m_RoomEditor->GetChildren();	
		for (std::shared_ptr<Gizmo> giz : tiles)
		{
			if (std::shared_ptr<RoomTile> tile = std::static_pointer_cast<RoomTile>(giz))
			{
				if (tile->Filled)
				{
					char byte = 0x01;
					file.write(reinterpret_cast<const char*>(&byte), sizeof(byte));
				}
				else
				{
					char byte = 0x00;
					file.write(reinterpret_cast<const char*>(&byte), sizeof(byte));
				}
			}
		}

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

		// The first two pieces of data are integers for the width of the room, followed by the height of the room
		int width, height;
		file.read(reinterpret_cast<char*>(&width), sizeof(width));
		file.read(reinterpret_cast<char*>(&height), sizeof(height));

		if (width != ROOM_WIDTH || height != ROOM_HEIGHT)
		{
			VX_ERROR("Room dimensions did not match expected dimensions. This room may have been made with an older version of the VoltexTool.");
			return;
		}

		// Read the file byte by byte and fill in RoomTiles where necessary
		std::vector<std::shared_ptr<Gizmo>> tiles = m_RoomEditor->GetChildren();
		for (std::shared_ptr<Gizmo> giz : tiles)
		{
			if (std::shared_ptr<RoomTile> tile = std::static_pointer_cast<RoomTile>(giz))
			{
				char byte;
				file.read(reinterpret_cast<char*>(&byte), sizeof(byte));
				if (byte == 0x00)
					tile->Filled = false;
				else if (byte == 0x01)
					tile->Filled = true;
			}
		}

		file.close();
		VX_LOG("Loaded room from file: " + filePath);
	}

};

int main()
{
	// Make and run the tool
	std::unique_ptr<Tool> tool = std::make_unique<Tool>();
	tool->Run();

	return 0;
}
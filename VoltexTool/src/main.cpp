#include <VECore.h>
#include <VEUI.h>

using namespace VoltexEngine;

class Tool : public Application
{

public:

	Tool()
	{
		std::shared_ptr<HorizontalLayoutGizmo> canvas = CreateGizmo<HorizontalLayoutGizmo>();

		std::shared_ptr<VerticalLayoutGizmo> tileSelector = CreateGizmo<VerticalLayoutGizmo>();
		canvas->AddChild(tileSelector);

		for (int i = 0; i < 10; i++)
		{
			std::shared_ptr<ButtonGizmo> selectionTile = CreateGizmo<ButtonGizmo>();
			tileSelector->AddChild(selectionTile);
		}

		std::shared_ptr<VerticalLayoutGizmo> vertLayout = CreateGizmo<VerticalLayoutGizmo>();
		vertLayout->Weight = 15;
		canvas->AddChild(vertLayout);

		std::shared_ptr<GridLayoutGizmo> roomEditor = CreateGizmo<GridLayoutGizmo>();
		roomEditor->SetNumSlots(20, 10);
		roomEditor->Weight = 3;
		vertLayout->AddChild(roomEditor);

		for (int i = 0; i < 20 * 10; i++)
		{
			std::shared_ptr<ButtonGizmo> roomTile = CreateGizmo<ButtonGizmo>();
			roomEditor->AddChild(roomTile);
		}

		std::shared_ptr<HorizontalLayoutGizmo> fileSelector = CreateGizmo<HorizontalLayoutGizmo>();
		vertLayout->AddChild(fileSelector);

		std::shared_ptr<ButtonGizmo> save = CreateGizmo<ButtonGizmo>();
		save->OnButtonPressed.AddCallback([] { VX_LOG("Saving"); });
		fileSelector->AddChild(save);

		std::shared_ptr<ButtonGizmo> load = CreateGizmo<ButtonGizmo>();
		load->OnButtonPressed.AddCallback([] { VX_LOG("Loading"); });
		fileSelector->AddChild(load);
	}

};

int main()
{
	// Make and run the tool
	std::unique_ptr<Tool> tool = std::make_unique<Tool>();
	tool->Run();

	return 0;
}
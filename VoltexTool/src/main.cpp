#include <VoltexEngine.h>

using namespace VoltexEngine;

class Tool : public Application
{

public:

	Tool()
	{
		std::shared_ptr<HorizontalLayoutGizmo> g1 = CreateGizmo<HorizontalLayoutGizmo>();
		SetRootGizmo(g1);

		std::shared_ptr<VerticalLayoutGizmo> g2 = CreateGizmo<VerticalLayoutGizmo>(); g2->SetWeight(2);
		std::shared_ptr<HorizontalLayoutGizmo> g3 = CreateGizmo<HorizontalLayoutGizmo>();
		std::shared_ptr<HorizontalLayoutGizmo> g4 = CreateGizmo<HorizontalLayoutGizmo>();
		g1->AddChild(g2);
		g1->AddChild(g3);
		g1->AddChild(g4);

		std::shared_ptr<HorizontalLayoutGizmo> g5 = CreateGizmo<HorizontalLayoutGizmo>();
		std::shared_ptr<HorizontalLayoutGizmo> g6 = CreateGizmo<HorizontalLayoutGizmo>(); g6->SetWeight(2);
		g2->AddChild(g5);
		g2->AddChild(g6);

		std::shared_ptr<HorizontalLayoutGizmo> g7 = CreateGizmo<HorizontalLayoutGizmo>();
		std::shared_ptr<HorizontalLayoutGizmo> g8 = CreateGizmo<HorizontalLayoutGizmo>();
		g4->AddChild(g7);
		g4->AddChild(g8);
	}

};

int main()
{
	// Make and run the tool
	std::unique_ptr<Tool> tool = std::make_unique<Tool>();
	tool->Run();

	return 0;
}
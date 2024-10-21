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
		std::shared_ptr<VerticalLayoutGizmo> g3 = CreateGizmo<VerticalLayoutGizmo>();
		std::shared_ptr<HorizontalLayoutGizmo> g4 = CreateGizmo<HorizontalLayoutGizmo>();
		g1->AddChild(g2);
		g1->AddChild(g3);
		g1->AddChild(g4);

		std::shared_ptr<HorizontalLayoutGizmo> g5 = CreateGizmo<HorizontalLayoutGizmo>();
		std::shared_ptr<GridLayoutGizmo> g6 = CreateGizmo<GridLayoutGizmo>(); g6->SetWeight(2);
		g2->AddChild(g5);
		g2->AddChild(g6);

		std::shared_ptr<HorizontalLayoutGizmo> g7 = CreateGizmo<HorizontalLayoutGizmo>();
		std::shared_ptr<HorizontalLayoutGizmo> g8 = CreateGizmo<HorizontalLayoutGizmo>();
		std::shared_ptr<HorizontalLayoutGizmo> g9 = CreateGizmo<HorizontalLayoutGizmo>();
		std::shared_ptr<HorizontalLayoutGizmo> g10 = CreateGizmo<HorizontalLayoutGizmo>();
		g6->AddChild(g7);
		g6->AddChild(g8);
		g6->AddChild(g9);
		g6->AddChild(g10);

		std::shared_ptr<HorizontalLayoutGizmo> g11 = CreateGizmo<HorizontalLayoutGizmo>();
		std::shared_ptr<HorizontalLayoutGizmo> g12 = CreateGizmo<HorizontalLayoutGizmo>();
		g3->AddChild(g11);
		g3->AddChild(g12);

		std::shared_ptr<HorizontalLayoutGizmo> g13 = CreateGizmo<HorizontalLayoutGizmo>();
		std::shared_ptr<HorizontalLayoutGizmo> g14 = CreateGizmo<HorizontalLayoutGizmo>();
		g4->AddChild(g13);
		g4->AddChild(g14);
	}

};

int main()
{
	// Make and run the tool
	std::unique_ptr<Tool> tool = std::make_unique<Tool>();
	tool->Run();

	return 0;
}
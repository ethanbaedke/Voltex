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

		std::shared_ptr<HorizontalLayoutGizmo> g5 = CreateGizmo<HorizontalLayoutGizmo>(); g5->SetColor(Color(0, 0, 255, 255));
		std::shared_ptr<GridLayoutGizmo> g6 = CreateGizmo<GridLayoutGizmo>(); g6->SetWeight(2);
		g2->AddChild(g5);
		g2->AddChild(g6);

		std::shared_ptr<HorizontalLayoutGizmo> g7 = CreateGizmo<HorizontalLayoutGizmo>(); g7->SetColor(Color(0, 255, 0, 255));
		std::shared_ptr<HorizontalLayoutGizmo> g8 = CreateGizmo<HorizontalLayoutGizmo>(); g8->SetColor(Color(0, 255, 255, 255));
		std::shared_ptr<HorizontalLayoutGizmo> g9 = CreateGizmo<HorizontalLayoutGizmo>(); g9->SetColor(Color(255, 0, 0, 255));
		std::shared_ptr<HorizontalLayoutGizmo> g10 = CreateGizmo<HorizontalLayoutGizmo>(); g10->SetColor(Color(255, 0, 255, 255));
		g6->AddChild(g7);
		g6->AddChild(g8);
		g6->AddChild(g9);
		g6->AddChild(g10);

		std::shared_ptr<HorizontalLayoutGizmo> g11 = CreateGizmo<HorizontalLayoutGizmo>(); g11->SetColor(Color(255, 255, 0, 255));
		std::shared_ptr<VerticalLayoutGizmo> g12 = CreateGizmo<VerticalLayoutGizmo>();
		std::shared_ptr<HorizontalLayoutGizmo> g13 = CreateGizmo<HorizontalLayoutGizmo>(); g13->SetColor(Color(0, 0, 255, 255));
		g3->AddChild(g11);
		g3->AddChild(g12);
		g3->AddChild(g13);

		std::shared_ptr<HorizontalLayoutGizmo> g14 = CreateGizmo<HorizontalLayoutGizmo>(); g14->SetColor(Color(0, 255, 0, 255));
		std::shared_ptr<HorizontalLayoutGizmo> g15 = CreateGizmo<HorizontalLayoutGizmo>(); g15->SetColor(Color(0, 255, 255, 255));
		std::shared_ptr<HorizontalLayoutGizmo> g16 = CreateGizmo<HorizontalLayoutGizmo>(); g16->SetColor(Color(255, 0, 0, 255));
		g12->AddChild(g14);
		g12->AddChild(g15);
		g12->AddChild(g16);

		std::shared_ptr<HorizontalLayoutGizmo> g17 = CreateGizmo<HorizontalLayoutGizmo>(); g17->SetColor(Color(255, 0, 255, 255));
		std::shared_ptr<HorizontalLayoutGizmo> g18 = CreateGizmo<HorizontalLayoutGizmo>(); g18->SetColor(Color(255, 255, 0, 255));
		g4->AddChild(g17);
		g4->AddChild(g18);
	}

};

int main()
{
	// Make and run the tool
	std::unique_ptr<Tool> tool = std::make_unique<Tool>();
	tool->Run();

	return 0;
}
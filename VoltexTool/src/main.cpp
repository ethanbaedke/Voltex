#include <VoltexEngine.h>

using namespace VoltexEngine;

class Tool : public Application
{

public:

	Tool()
	{
		std::shared_ptr<HorizontalLayoutGizmo> g1 = CreateGizmo<HorizontalLayoutGizmo>();
		g1->SetDimensions(0.0f, 0.0f, 0.5f, 0.5f);

		std::shared_ptr<VerticalLayoutGizmo> g2 = CreateGizmo<VerticalLayoutGizmo>(); g2->SetWeight(2);
		std::shared_ptr<VerticalLayoutGizmo> g3 = CreateGizmo<VerticalLayoutGizmo>();
		std::shared_ptr<HorizontalLayoutGizmo> g4 = CreateGizmo<HorizontalLayoutGizmo>();
		g1->AddChild(g2);
		g1->AddChild(g3);
		g1->AddChild(g4);

		std::shared_ptr<Gizmo> g5 = CreateGizmo<Gizmo>(); g5->SetColor(Color(0, 0, 255, 255));
		std::shared_ptr<GridLayoutGizmo> g6 = CreateGizmo<GridLayoutGizmo>(); g6->SetWeight(2);
		g2->AddChild(g5);
		g2->AddChild(g6);

		std::shared_ptr<Gizmo> g7 = CreateGizmo<Gizmo>(); g7->SetColor(Color(0, 255, 0, 255));
		std::shared_ptr<Gizmo> g8 = CreateGizmo<Gizmo>(); g8->SetColor(Color(0, 255, 255, 255));
		std::shared_ptr<Gizmo> g9 = CreateGizmo<Gizmo>(); g9->SetColor(Color(255, 0, 0, 255));
		std::shared_ptr<Gizmo> g10 = CreateGizmo<Gizmo>(); g10->SetColor(Color(255, 0, 255, 255));
		g6->AddChild(g7);
		g6->AddChild(g8);
		g6->AddChild(g9);
		g6->AddChild(g10);

		std::shared_ptr<Gizmo> g11 = CreateGizmo<Gizmo>(); g11->SetColor(Color(255, 255, 0, 255));
		std::shared_ptr<VerticalLayoutGizmo> g12 = CreateGizmo<VerticalLayoutGizmo>();
		std::shared_ptr<Gizmo> g13 = CreateGizmo<Gizmo>(); g13->SetColor(Color(0, 0, 255, 255));
		g3->AddChild(g11);
		g3->AddChild(g12);
		g3->AddChild(g13);

		std::shared_ptr<Gizmo> g14 = CreateGizmo<Gizmo>(); g14->SetColor(Color(0, 255, 0, 255));
		std::shared_ptr<Gizmo> g15 = CreateGizmo<Gizmo>(); g15->SetColor(Color(0, 255, 255, 255));
		std::shared_ptr<Gizmo> g16 = CreateGizmo<Gizmo>(); g16->SetColor(Color(255, 0, 0, 255));
		g12->AddChild(g14);
		g12->AddChild(g15);
		g12->AddChild(g16);

		std::shared_ptr<Gizmo> g17 = CreateGizmo<Gizmo>(); g17->SetColor(Color(255, 0, 255, 255));
		std::shared_ptr<Gizmo> g18 = CreateGizmo<Gizmo>(); g18->SetColor(Color(255, 255, 0, 255));
		g4->AddChild(g17);
		g4->AddChild(g18);

		std::shared_ptr<Gizmo> g19 = CreateGizmo<Gizmo>();
		g19->SetDimensions(0.5f, 0.5f, 0.5f, 0.5f);
	}

};

int main()
{
	// Make and run the tool
	std::unique_ptr<Tool> tool = std::make_unique<Tool>();
	tool->Run();

	return 0;
}
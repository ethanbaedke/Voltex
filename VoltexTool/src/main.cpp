#include <VoltexEngine.h>

using namespace VoltexEngine;

class Tool : public Application
{

public:

	Tool()
	{
		std::shared_ptr<Gizmo> g1 = CreateGizmo();
		SetRootGizmo(g1);

		std::shared_ptr<Gizmo> g2 = CreateGizmo();
		std::shared_ptr<Gizmo> g3 = CreateGizmo();
		g1->AddChild(g2);
		g1->AddChild(g3);

		g2->SetLayout(UILayout::Vertical);
		std::shared_ptr<Gizmo> g4 = CreateGizmo();
		std::shared_ptr<Gizmo> g5 = CreateGizmo();
		g2->AddChild(g4);
		g2->AddChild(g5);
	}

};

int main()
{
	// Make and run the tool
	std::unique_ptr<Tool> tool = std::make_unique<Tool>();
	tool->Run();

	return 0;
}
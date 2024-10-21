#include <VoltexEngine.h>

using namespace VoltexEngine;

class Tool : public Application
{

public:

	Tool()
	{
		std::shared_ptr<Gizmo> g1 = CreateGizmo();
		SetRootGizmo(g1);

		std::shared_ptr<Gizmo> g2 = CreateGizmo(g1, UILayout::Vertical, 2);
		std::shared_ptr<Gizmo> g5 = CreateGizmo(g2);
		std::shared_ptr<Gizmo> g6 = CreateGizmo(g2, 2);

		std::shared_ptr<Gizmo> g3 = CreateGizmo(g1);

		std::shared_ptr<Gizmo> g4 = CreateGizmo(g1, UILayout::Vertical);
		std::shared_ptr<Gizmo> g7 = CreateGizmo(g4);
		std::shared_ptr<Gizmo> g8 = CreateGizmo(g4);

	}

};

int main()
{
	// Make and run the tool
	std::unique_ptr<Tool> tool = std::make_unique<Tool>();
	tool->Run();

	return 0;
}
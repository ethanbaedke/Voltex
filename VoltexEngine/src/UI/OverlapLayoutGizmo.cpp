#include "vxpch.h"
#include "OverlapLayoutGizmo.h"

namespace VoltexEngine {

	void OverlapLayoutGizmo::Tick()
	{
		if (m_Children.size() == 0)
			return;

		for (int i = 0; i < m_Children.size(); i++)
		{
			m_Children[i]->SetDimensions(m_PosX, m_PosY, m_SizeX, m_SizeY);
			m_Children[i]->Depth = Depth + 1 + i;
			m_Children[i]->Tick();
		}
	}

}
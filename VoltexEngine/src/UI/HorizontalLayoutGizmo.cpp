#include "vxpch.h"
#include "HorizontalLayoutGizmo.h"

namespace VoltexEngine {


	void HorizontalLayoutGizmo::Tick()
	{
		if (m_Children.size() == 0)
			return;

		float totalWeight = 0;
		for (std::shared_ptr<Gizmo> giz : m_Children)
			totalWeight += giz->GetWeight();

		float currentX = m_PosX;
		for (int i = 0; i < m_Children.size(); i++)
		{
			float sizeX = m_SizeX * (m_Children[i]->GetWeight() / totalWeight);
			int depth = m_Depth + 1;
			m_Children[i]->SetDimensions(currentX, m_PosY, sizeX, m_SizeY);
			m_Children[i]->SetDepth(depth);
			m_Children[i]->Tick();
			currentX += sizeX;
		}
	}

}
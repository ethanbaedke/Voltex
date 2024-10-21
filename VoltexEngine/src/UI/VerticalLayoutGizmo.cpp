#include "vxpch.h"
#include "VerticalLayoutGizmo.h"

namespace VoltexEngine {


	void VerticalLayoutGizmo::Tick()
	{
		if (m_Children.size() == 0)
			return;

		float totalWeight = 0;
		for (std::shared_ptr<Gizmo> giz : m_Children)
			totalWeight += giz->GetWeight();

		float currentY = m_PosY + m_SizeY;
		for (int i = 0; i < m_Children.size(); i++)
		{
			float sizeY = m_SizeY * (m_Children[i]->GetWeight() / totalWeight);
			currentY -= sizeY;
			m_Children[i]->SetDimensions(m_PosX, currentY, m_SizeX, sizeY);
			m_Children[i]->SetDepth(m_Depth + 1);
			m_Children[i]->Tick();
		}
	}

}
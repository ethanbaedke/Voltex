#include "vxpch.h"
#include "GridLayoutGizmo.h"

namespace VoltexEngine {

	GridLayoutGizmo::GridLayoutGizmo()
		: m_NumSlotsAcross(2), m_NumSlotsDown(2)
	{
	}

	void GridLayoutGizmo::Tick()
	{
		if (m_NumSlotsAcross == 0 || m_NumSlotsDown == 0)
			return;

		float hStep = m_SizeX / m_NumSlotsAcross;
		float vStep = m_SizeY / m_NumSlotsDown;

		int hInd = 0;
		int vInd = 0;
		float startY = (m_PosY + m_SizeY) - vStep;
		while ((hInd + (vInd * m_NumSlotsAcross)) < m_Children.size() && vInd != m_NumSlotsDown)
		{
			float xPos = m_PosX + (hInd * hStep);
			float yPos = startY - (vInd * vStep);
			float xSize = hStep;
			float ySize = vStep;
			m_Children[hInd + (vInd * m_NumSlotsAcross)]->SetDimensions(xPos, yPos, xSize, ySize);
			m_Children[hInd + (vInd * m_NumSlotsAcross)]->SetDepth(m_Depth + 1);
			m_Children[hInd + (vInd * m_NumSlotsAcross)]->Tick();

			hInd++;
			if (hInd == m_NumSlotsAcross)
			{
				vInd++;
				hInd = 0;
			}
		}
	}

	void GridLayoutGizmo::SetNumSlots(int across, int down)
	{
		m_NumSlotsAcross = across;
		m_NumSlotsDown = down;
	}

}
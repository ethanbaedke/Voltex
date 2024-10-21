#include "vxpch.h"
#include "Gizmo.h"

namespace VoltexEngine {

	Gizmo::Gizmo()
		: m_PosX(0.0f), m_PosY(0.0f), m_SizeX(1.0f), m_SizeY(1.0f), m_Weight(1), m_Depth(0), m_Layout(UILayout::Horizontal)
	{
	}

	Gizmo::Gizmo(unsigned int weight)
		: m_PosX(0.0f), m_PosY(0.0f), m_SizeX(1.0f), m_SizeY(1.0f), m_Weight(weight), m_Depth(0), m_Layout(UILayout::Horizontal)
	{
	}

	void Gizmo::Tick()
	{
		if (m_Children.size() == 0)
			return;

		if (m_Layout == UILayout::Horizontal)
		{
			float totalWeight = 0;
			for (std::shared_ptr<Gizmo> giz : m_Children)
				totalWeight += giz->m_Weight;

			float currentX = m_PosX;
			for (int i = 0; i < m_Children.size(); i++)
			{
				float step = m_SizeX * (m_Children[i]->m_Weight / totalWeight);
				m_Children[i]->m_PosX = currentX;
				m_Children[i]->m_SizeX = step;
				m_Children[i]->m_PosY = m_PosY;
				m_Children[i]->m_SizeY = m_SizeY;
				m_Children[i]->m_Depth = m_Depth + 1;
				m_Children[i]->Tick();
				currentX += step;
			}
		}
		else if (m_Layout == UILayout::Vertical)
		{
			float totalWeight = 0;
			for (std::shared_ptr<Gizmo> giz : m_Children)
				totalWeight += giz->m_Weight;

			float currentY = m_PosY;
			for (int i = 0; i < m_Children.size(); i++)
			{
				float step = m_SizeY * (m_Children[i]->m_Weight / totalWeight);
				m_Children[i]->m_PosX = m_PosX;
				m_Children[i]->m_SizeX = m_SizeX;
				m_Children[i]->m_PosY = currentY;
				m_Children[i]->m_SizeY = step;
				m_Children[i]->m_Depth = m_Depth + 1;
				m_Children[i]->Tick();
				currentY += step;
			}
		}
		else if (m_Layout == UILayout::Overlap)
		{
			for (int i = 0; i < m_Children.size(); i++)
			{
				m_Children[i]->m_PosX = m_PosX;
				m_Children[i]->m_SizeX = m_SizeX;
				m_Children[i]->m_PosY = m_PosY;
				m_Children[i]->m_SizeY = m_SizeY;
				m_Children[i]->m_Depth = m_Depth + 1;
				m_Children[i]->Tick();
			}
		}
	}

	void Gizmo::GetDimensions(float* outXPos, float* outYPos, float* outXSize, float* outYSize) const
	{
		*outXPos = m_PosX;
		*outYPos = m_PosY;
		*outXSize = m_SizeX;
		*outYSize = m_SizeY;
	}

}
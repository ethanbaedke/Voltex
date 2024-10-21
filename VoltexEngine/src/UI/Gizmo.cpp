#include "vxpch.h"
#include "Gizmo.h"

namespace VoltexEngine {

	Gizmo::Gizmo()
		: m_PosX(0.0f), m_PosY(0.0f), m_SizeX(1.0f), m_SizeY(1.0f), m_Depth(0), m_Weight(1), m_Color(Color(255, 255, 255, 255))
	{
	}

	void Gizmo::SetDimensions(float xPos, float yPos, float xSize, float ySize)
	{
		m_PosX = xPos;
		m_PosY = yPos;
		m_SizeX = xSize;
		m_SizeY = ySize;
	}

	void Gizmo::GetDimensions(float* outXPos, float* outYPos, float* outXSize, float* outYSize) const
	{
		*outXPos = m_PosX;
		*outYPos = m_PosY;
		*outXSize = m_SizeX;
		*outYSize = m_SizeY;
	}

}
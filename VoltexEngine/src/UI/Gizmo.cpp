#include "vxpch.h"
#include "Gizmo.h"

#include "Core/Console.h"

namespace VoltexEngine {

	std::shared_ptr<Gizmo> Gizmo::s_Focused;

	void Gizmo::CursorOverlap(std::shared_ptr<Gizmo> gizmo)
	{
		if (s_Focused == gizmo)
			return;

		if (s_Focused)
			s_Focused->HandleCursorLeave();

		if (gizmo)
			gizmo->HandleCursorEnter();

		s_Focused = gizmo;
	}

	void Gizmo::CursorSelect()
	{
		if (s_Focused)
			s_Focused->HandleCursorSelect();
	}

	Gizmo::Gizmo()
		: Weight(1), Depth(0), IsRoot(true), m_PosX(0.0f), m_PosY(0.0f), m_SizeX(1.0f), m_SizeY(1.0f), m_Color(Color(255, 255, 255, 255))
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

	std::string Gizmo::GetPrintable() const
	{
		std::string position = "Position: (" + std::to_string(m_PosX) + ", " + std::to_string(m_PosY) + ") ";
		std::string size = "Size: (" + std::to_string(m_SizeX) + ", " + std::to_string(m_SizeY) + ") ";
		std::string depth = "m_Depth: (" + std::to_string(Depth) + ") ";
		return position + size + depth;
	}

}
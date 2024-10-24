#include "vxpch.h"
#include "Gizmo.h"

#include "Core/Console.h"

namespace VoltexEngine {

	std::shared_ptr<Gizmo> Gizmo::s_Focused;

	Gizmo::Gizmo()
		: m_PosX(0.0f), m_PosY(0.0f), m_SizeX(1.0f), m_SizeY(1.0f), m_Depth(0), m_Weight(1), m_Color(Color(255, 255, 255, 255)), m_DefaultColor(Color(255, 255, 255, 255)), m_FocusedColor(Color(128, 128, 128, 255))
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

	void Gizmo::HandleCursorEnter()
	{
		if (s_Focused == shared_from_this())
			return;

		if (s_Focused)
			s_Focused->HandleCursorLeave();

		s_Focused = shared_from_this();
		m_Color = m_FocusedColor;
	}

	void Gizmo::HandleCursorLeave()
	{
		m_Color = m_DefaultColor;
	}

	void Gizmo::SetColor(const Color& color)
	{
		m_Color = color;
		m_DefaultColor = color;
		m_FocusedColor = Color(color.R() / 2, color.G() / 2, color.B() / 2, color.A());
	}

	std::string Gizmo::GetPrintable() const
	{
		std::string position = "Position: (" + std::to_string(m_PosX) + ", " + std::to_string(m_PosY) + ") ";
		std::string size = "Size: (" + std::to_string(m_SizeX) + ", " + std::to_string(m_SizeY) + ") ";
		std::string depth = "Depth: (" + std::to_string(m_Depth) + ") ";
		return position + size + depth;
	}

}
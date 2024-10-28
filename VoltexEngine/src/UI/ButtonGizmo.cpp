#include "vxpch.h"
#include "ButtonGizmo.h"

namespace VoltexEngine {

	ButtonGizmo::ButtonGizmo()
		: Gizmo(), m_DefaultColor(Color(255, 255, 255, 255)), m_FocusedColor(Color(128, 128, 128, 255))
	{
	}

	void VoltexEngine::ButtonGizmo::SetColor(const Color& color)
	{
		Gizmo::SetColor(color);
		
		m_DefaultColor = color;
		m_FocusedColor = Color(color.R() / 2, color.G() / 2, color.B() / 2, color.A());
	}

	void ButtonGizmo::HandleCursorEnter()
	{
		m_Color = m_FocusedColor;
	}

	void ButtonGizmo::HandleCursorLeave()
	{
		m_Color = m_DefaultColor;
	}

	void ButtonGizmo::HandleCursorSelect()
	{
		OnButtonPressed.Invoke();
	}

}
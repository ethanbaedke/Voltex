#pragma once
#include "vxpch.h"

#include "Gizmo.h"
#include "Core/Event.h"

namespace VoltexEngine {

	class ButtonGizmo : public Gizmo
	{

	private:

		Color m_DefaultColor;
		Color m_FocusedColor;

	public:


		Event_NoParam OnButtonPressed;

		ButtonGizmo();

		virtual void SetColor(const Color& color) override;

	protected:

		virtual void HandleCursorEnter() override;
		virtual void HandleCursorLeave() override;
		virtual void HandleCursorSelect() override;

	};

}
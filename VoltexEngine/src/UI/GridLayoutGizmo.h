#pragma once
#include "vxpch.h"

#include "LayoutGizmo.h"

namespace VoltexEngine {

	class GridLayoutGizmo : public LayoutGizmo
	{

	private:

		unsigned int m_NumSlotsAcross;
		unsigned int m_NumSlotsDown;

	public:

		GridLayoutGizmo();

		virtual void Tick() override;

	};

}
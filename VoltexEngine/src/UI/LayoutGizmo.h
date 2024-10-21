#pragma once
#include "vxpch.h"

#include "Gizmo.h"

namespace VoltexEngine {

	class LayoutGizmo : public Gizmo
	{

	protected:

		/* List of this elements child elements */
		std::vector<std::shared_ptr<Gizmo>> m_Children;

	public:

		inline void AddChild(std::shared_ptr<Gizmo> gizmo) { m_Children.push_back(gizmo); }
		inline const std::vector<std::shared_ptr<Gizmo>>& GetChildren() { return m_Children; }

	};

}
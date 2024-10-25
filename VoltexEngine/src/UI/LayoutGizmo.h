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

		void AddChild(std::shared_ptr<Gizmo> gizmo);

		inline const std::vector<std::shared_ptr<Gizmo>>& GetChildren() { return m_Children; }

	};

}
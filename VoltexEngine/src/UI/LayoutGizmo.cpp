#include "vxpch.h"
#include "LayoutGizmo.h"

namespace VoltexEngine {

    void LayoutGizmo::AddChild(std::shared_ptr<Gizmo> gizmo)
    {
        m_Children.push_back(gizmo);
        gizmo->IsRoot = false;
    }

}
#pragma once
#include "vxpch.h"

namespace VoltexEngine {

	enum class UILayout
	{
		Horizontal,
		Vertical,
		Overlap
	};

	class Gizmo
	{

	private:

		/* The position and size of this gizmo in UI coordinates */
		float m_PosX;
		float m_PosY;
		float m_SizeX;
		float m_SizeY;

		/* How much space this element should take up relative to its siblings in side by side layouts */
		unsigned int m_Weight;

		/* How far back or forward this element should be rendered */
		int m_Depth;

		/* Layout this element should use for its children */
		UILayout m_Layout;

		/* List of this elements child elements */
		std::vector<std::shared_ptr<Gizmo>> m_Children;

	public:

		Gizmo();
		Gizmo(unsigned int weight);

		void Tick();

		inline int GetDepth() const { return m_Depth; }
		inline void SetLayout(UILayout layout) { m_Layout = layout; }
		inline void AddChild(std::shared_ptr<Gizmo> gizmo) { m_Children.push_back(gizmo); }
		inline const std::vector<std::shared_ptr<Gizmo>>& GetChildren() { return m_Children; }

		/* Outputs this gizmos position and size data to the input variables */
		void GetDimensions(float* outXPos, float* outYPos, float* outXSize, float* outYSize) const;

	};

}


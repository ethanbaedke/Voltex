#pragma once
#include "vxpch.h"

#include "Core/Color.h"

namespace VoltexEngine {

	class Gizmo
	{

	protected:

		/* The position and size of this gizmo in UI coordinates */
		float m_PosX;
		float m_PosY;
		float m_SizeX;
		float m_SizeY;

		/* How far back or forward this element should be rendered */
		int m_Depth;

		/* How much space this element should take up relative to its siblings if in a weighted layout */
		unsigned int m_Weight;

	private:

		Color m_Color;

	public:

		Gizmo();

		virtual void Tick() {}

		void SetDimensions(float xPos, float yPos, float xSize, float ySize);
		void GetDimensions(float* outXPos, float* outYPos, float* outXSize, float* outYSize) const;

		inline void SetDepth(int depth) { m_Depth = depth; }
		inline int GetDepth() const { return m_Depth; }
		inline void SetWeight(unsigned int weight) { m_Weight = weight; }
		inline unsigned int GetWeight() const { return m_Weight; }
		inline void SetColor(const Color& color) { m_Color = color; }
		inline const Color& GetColor() const { return m_Color; }

	};

}


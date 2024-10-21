#pragma once
#include "vxpch.h"

#include "Core/Printable.h"
#include "Core/Color.h"

namespace VoltexEngine {

	class Gizmo : public IPrintable
	{

	private:

		// TODO: Make a gizmo manager and handle this, THIS IS BAD
		static Gizmo* s_Focused;

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
		Color m_DefaultColor;
		Color m_FocusedColor;

	public:

		Gizmo();

		virtual void Tick() {}

		void SetDimensions(float xPos, float yPos, float xSize, float ySize);
		void GetDimensions(float* outXPos, float* outYPos, float* outXSize, float* outYSize) const;

		void HandleCursorEnter();
		void HandleCursorLeave();

		void SetColor(const Color& color);

		inline void SetDepth(int depth) { m_Depth = depth; }
		inline int GetDepth() const { return m_Depth; }
		inline void SetWeight(unsigned int weight) { m_Weight = weight; }
		inline unsigned int GetWeight() const { return m_Weight; }
		inline const Color& GetColor() const { return m_Color; }

		virtual std::string GetPrintable() const override;
	};

}


#pragma once
#include "vxpch.h"

#include "Core/Printable.h"
#include "Core/Color.h"

namespace VoltexEngine {

	class Gizmo : public IPrintable, public std::enable_shared_from_this<Gizmo>
	{

	private:

		// Only one gizmo can ever be focused at a time so we store it here statically
		static std::shared_ptr<Gizmo> s_Focused;

	public:

		static void CursorOverlap(std::shared_ptr<Gizmo> gizmo);

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

		/* Whether this gizmo is a top level gizmo or a child of a top level gizmo */
		bool m_IsRoot;

		Color m_Color;
		Color m_DefaultColor;
		Color m_FocusedColor;

	public:

		Gizmo();

		virtual void Tick() {}

		void SetDimensions(float xPos, float yPos, float xSize, float ySize);
		void GetDimensions(float* outXPos, float* outYPos, float* outXSize, float* outYSize) const;

		void SetColor(const Color& color);

		inline void SetDepth(int depth) { m_Depth = depth; }
		inline int GetDepth() const { return m_Depth; }
		inline void SetWeight(unsigned int weight) { m_Weight = weight; }
		inline unsigned int GetWeight() const { return m_Weight; }
		inline void SetIsRoot(bool isRoot) { m_IsRoot = isRoot; }
		inline bool GetIsRoot() const { return m_IsRoot; }
		inline const Color& GetColor() const { return m_Color; }

		virtual std::string GetPrintable() const override;

	protected:

		virtual void HandleCursorEnter();
		virtual void HandleCursorLeave();

	};

}


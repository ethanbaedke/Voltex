#pragma once
#include "vxpch.h"

#include "Core/Printable.h"
#include "Core/Color.h"
#include "Core/Sprite.h"

namespace VoltexEngine {

	class Gizmo : public IPrintable, public std::enable_shared_from_this<Gizmo>
	{

	private:

		// Only one gizmo can ever be focused at a time so we store it here statically
		static std::shared_ptr<Gizmo> s_Focused;

	public:

		static void CursorOverlap(std::shared_ptr<Gizmo> gizmo);
		static void CursorSelect();

	public:

		std::shared_ptr<Sprite> UISprite;

		/* How much space this element should take up relative to its siblings if in a weighted layout */
		unsigned int Weight;

		/* How far back or forward this element should be rendered */
		int Depth;

		/* Whether this gizmo is a top level gizmo or a child of a top level gizmo */
		bool IsRoot;

	protected:

		/* The position and size of this gizmo in UI coordinates */
		float m_PosX;
		float m_PosY;
		float m_SizeX;
		float m_SizeY;

		/* Color of the gizmo */
		Color m_Color;

	public:

		Gizmo();

		virtual void Tick() {}

		void SetDimensions(float xPos, float yPos, float xSize, float ySize);
		void GetDimensions(float* outXPos, float* outYPos, float* outXSize, float* outYSize) const;

		virtual void SetColor(const Color& color) { m_Color = color; }
		inline const Color& GetColor() const { return m_Color; }

		virtual std::string GetPrintable() const override;

	protected:

		virtual void HandleCursorEnter() {};
		virtual void HandleCursorLeave() {};
		virtual void HandleCursorSelect() {};

	};

}


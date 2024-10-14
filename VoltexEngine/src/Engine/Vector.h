#pragma once
#include "vxpch.h"

#include "Printable.h"

namespace VoltexEngine {

	class Vector : public IPrintable
	{

	public:

		// Returns unit vectors in the up and right directions
		static inline Vector Up() { return Vector(0.0f, 1.0f); }
		static inline Vector Right() { return Vector(1.0f, 0.0f); }

	public:

		Vector();
		Vector(float units);
		Vector(float x, float y);

		float Length() const;

		float Dot(const Vector& other) const;

		Vector operator+(const Vector& other) const;
		Vector operator-(const Vector& other) const;
		Vector operator*(float scalar) const;
		Vector operator/(float divisor) const;

		inline float X() const { return m_X; }
		inline float Y() const { return m_Y; }

		virtual std::string GetPrintable() const override;

	private:

		float m_X;
		float m_Y;

	};

}


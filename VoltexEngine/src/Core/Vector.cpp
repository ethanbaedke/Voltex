#include "vxpch.h"
#include "Vector.h"

#include "Console.h"

namespace VoltexEngine {

	Vector::Vector()
		: m_X(0.0f), m_Y(0.0f)
	{
	}

	Vector::Vector(float units)
		: m_X(units), m_Y(units)
	{
	}

	Vector::Vector(float x, float y)
		: m_X(x), m_Y(y)
	{
	}
	
	float Vector::Length() const
	{
		return (float)sqrt((m_X * m_X) + (m_Y * m_Y));
	}

	float Vector::Dot(const Vector& other) const
	{
		return (m_X * other.m_X) + (m_Y * other.m_Y);
	}

	Vector Vector::operator+(const Vector& other) const
	{
		return Vector(m_X + other.m_X, m_Y + other.m_Y);
	}

	Vector Vector::operator-(const Vector& other) const
	{
		return Vector(m_X - other.m_X, m_Y - other.m_Y);
	}

	Vector Vector::operator*(float scalar) const
	{
		return Vector(m_X * scalar, m_Y * scalar);
	}

	Vector Vector::operator/(float divisor) const
	{
		if (divisor == 0)
		{
			VX_WARNING("Cannot divide vector by 0");
			return Vector();
		}

		return Vector(m_X / divisor, m_Y / divisor);
	}

	std::string Vector::GetPrintable() const
	{
		return "(" + std::to_string(m_X) + ", " + std::to_string(m_Y) + ")";
	}

}

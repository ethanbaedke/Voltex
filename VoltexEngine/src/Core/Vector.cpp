#include "vxpch.h"
#include "Vector.h"

#include "Console.h"

namespace VoltexEngine {

	Vector::Vector()
		: X(0.0f), Y(0.0f)
	{
	}

	Vector::Vector(float units)
		: X(units), Y(units)
	{
	}

	Vector::Vector(float x, float y)
		: X(x), Y(y)
	{
	}
	
	float Vector::Length() const
	{
		return (float)sqrt((X * X) + (Y * Y));
	}

	float Vector::Dot(const Vector& other) const
	{
		return (X * other.X) + (Y * other.Y);
	}

	Vector Vector::operator+(const Vector& other) const
	{
		return Vector(X + other.X, Y + other.Y);
	}

	Vector Vector::operator-(const Vector& other) const
	{
		return Vector(X - other.X, Y - other.Y);
	}

	Vector Vector::operator*(float scalar) const
	{
		return Vector(X * scalar, Y * scalar);
	}

	Vector Vector::operator/(float divisor) const
	{
		if (divisor == 0)
		{
			VX_WARNING("Cannot divide vector by 0");
			return Vector();
		}

		return Vector(X / divisor, Y / divisor);
	}

	std::string Vector::GetPrintable() const
	{
		return "(" + std::to_string(X) + ", " + std::to_string(Y) + ")";
	}

}

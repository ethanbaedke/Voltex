#pragma once
#include "vxpch.h"

namespace VoltexEngine {

	class Color
	{

	private:

		unsigned char m_R;
		unsigned char m_G;
		unsigned char m_B;
		unsigned char m_A;
		
	public:

		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

		inline unsigned char R() const { return m_R; }
		inline unsigned char G() const { return m_G; }
		inline unsigned char B() const { return m_B; }
		inline unsigned char A() const { return m_A; }

	};

}

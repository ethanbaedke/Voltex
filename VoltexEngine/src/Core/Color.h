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

		inline unsigned char R() { return m_R; }
		inline unsigned char G() { return m_G; }
		inline unsigned char B() { return m_B; }
		inline unsigned char A() { return m_A; }

	};

}

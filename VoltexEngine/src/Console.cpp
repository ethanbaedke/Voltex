#include "vxpch.h"
#include "Console.h"

#include <iostream>

namespace VoltexEngine {

	void Console::Log(const std::string& text)
	{
		std::cout << text << "\033[38;2;255;255;255m" << std::endl;
	}

}
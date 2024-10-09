#pragma once

#include <string>

#define VX_LOG(x) VoltexEngine::Console::Log(std::string("\033[38;2;255;255;255m") + x)
#define VX_WARNING(x) VoltexEngine::Console::Log(std::string("\033[38;2;255;255;0m") + x)
#define VX_ERROR(x) VoltexEngine::Console::Log(std::string("\033[38;2;255;0;0m") + x)

namespace VoltexEngine {
	
	/* This handles all output to the console */
	class Console
	{
		
	public:

		/* Prints a line of text to the console */
		static void Log(const std::string& text);

	};

}
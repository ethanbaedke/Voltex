#include "vxpch.h"
#include "Application.h"

#include "Console.h"

namespace VoltexEngine {

	Application::Application()
		: m_Window(std::make_unique<Window>("Voltex Window", 1280, 720))
	{
		VX_LOG("Application Initialized");
	}

	void Application::Run()
	{
		while (true)
		{
		}
	}

}
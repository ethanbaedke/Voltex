#include "vxpch.h"
#include "Application.h"

#include "Console.h"

namespace VoltexEngine {

	Application::Application()
		: m_Window(std::make_unique<Window>("Voltex Window", 800, 600))
	{
		VX_LOG("Application Initialized");
	}

	void Application::Run()
	{
		while (true)
		{
			m_Window->Update(1.f/60.f);
		}
	}

}
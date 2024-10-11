#include "vxpch.h"
#include "Application.h"

#include "Renderer.h"
#include "Console.h"

namespace VoltexEngine {

	Application::Application()
		: m_Window(nullptr)
	{
		if (!Renderer::Init())
			return;

		m_Window = std::make_unique<Window>("Voltex Window", 800, 600);

		VX_LOG("Application Initialized");
	}

	void Application::Run()
	{
		while (true)
		{
			Renderer::Tick(1.0f / 60.0f);
		}
	}

}
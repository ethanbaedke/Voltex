#include "vxpch.h"
#include "Application.h"

#include "Renderer.h"
#include "Console.h"

#include "Vector.h"

namespace VoltexEngine {

	Application::Application()
		: m_Window(nullptr)
	{
		if (!Renderer::Init())
			return;

		//m_Window = std::make_unique<Window>("Voltex Window", 1920, 1080);

		Vector v1(2, 1);
		Vector v2(1, 2);
		Vector v3 = v1.Dot(v2);

		VX_LOG(v3.GetPrintable());

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
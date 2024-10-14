#include "vxpch.h"
#include "Window.h"

#include "Console.h"

namespace VoltexEngine {

	// Initialize static variables
	Event_OneParam<std::weak_ptr<Window>> Window::s_OnWindowCreated;

	std::shared_ptr<Window> Window::Create(const std::string& name, int width, int height)
	{
		std::shared_ptr<Window> win(new Window(name, width, height));
		s_OnWindowCreated.Invoke(win);
		return win;
	}

	Window::Window(const std::string& name, int width, int height)
		: m_Name(name), m_Width(width), m_Height(height)
	{
	}

}
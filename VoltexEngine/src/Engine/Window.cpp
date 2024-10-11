#include "vxpch.h"
#include "Window.h"

#include "Console.h"

namespace VoltexEngine {

	// Initialize static variables
	Event_OneParam<Window*> Window::s_OnWindowCreated;

	Window::Window(const std::string& name, int width, int height)
		: m_Name(name), m_Width(width), m_Height(height)
	{
		// Broadcast this window
		s_OnWindowCreated.Invoke(this);
	}

}
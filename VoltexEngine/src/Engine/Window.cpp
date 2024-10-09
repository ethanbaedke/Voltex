#include "vxpch.h"
#include "Window.h"

#include "Console.h"

namespace VoltexEngine {

	bool Window::s_GLFWInitialized = false;

	Window::Window(const std::string& name, int width, int height)
		: m_Window(nullptr)
	{
		// Make sure GLFW is initialized
		if (!s_GLFWInitialized)
		{
			if (!glfwInit())
			{
				VX_ERROR("GLFW could not be initialized");
				return;
			}
			else
			{
				VX_LOG("GLFW initialized");
				s_GLFWInitialized = true;
			}
		}

		// Create a GLFW window
		m_Window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
		if (!m_Window)
		{
			VX_ERROR("Failed to create GLFW window");
		}

		// Make this window the current window context
		glfwMakeContextCurrent(m_Window);

		// Load GLAD
		if (!gladLoadGL(glfwGetProcAddress))
		{
			VX_ERROR("Failed to load GLAD");
			return;
		}
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_Window);
	}
}
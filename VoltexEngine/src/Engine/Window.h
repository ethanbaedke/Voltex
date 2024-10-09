#pragma once
#include "vxpch.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace VoltexEngine {

	class Window
	{

	private:

		/* Makes sure glfw is only initialized once */
		static bool s_GLFWInitialized;

	private:

		GLFWwindow* m_Window;

	public:

		Window(const std::string& name, int width, int height);
		~Window();

	};

}


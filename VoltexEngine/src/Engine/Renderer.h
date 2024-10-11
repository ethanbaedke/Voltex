#pragma once
#include "vxpch.h"

#include "Window.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace VoltexEngine {

	class Renderer
	{

	private:

		static bool s_Initialized;

		static Window* s_EngineWindow;
		static GLFWwindow* s_GLWindow;

	public:

		static bool Init();

		static void Tick(float deltaTime);

	private:

		static void DrawTriangle();

		static void WindowCreatedCallback(Window* window);

	};

}
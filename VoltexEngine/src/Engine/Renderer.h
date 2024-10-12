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

		static GLuint s_ShaderProgram;

	public:

		static bool Init();

		static void Tick(float deltaTime);

	private:

		/* TESTING ONLY: Draws a triangle to the screen */
		static void DrawTriangle();

		/* Returns a string that is the text of a shader file */
		static std::string ReadShader(const std::string& name);

		/* Called when an engine window is created */
		static void WindowCreatedCallback(Window* window);

	};

}
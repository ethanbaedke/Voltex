#include "vxpch.h"
#include "Renderer.h"

#include "Console.h"
#include "ShaderLoader.h"

namespace VoltexEngine {

	// Declare/Define static variables
	bool Renderer::s_Initialized = false;
	Window* Renderer::s_EngineWindow;
	GLFWwindow* Renderer::s_GLWindow;

	bool Renderer::Init()
	{
		if (s_Initialized)
		{
			VX_ERROR("Renderer is already initialized");
			return false;
		}

		if (!glfwInit())
		{
			VX_ERROR("GLFW could not be initialized");
			return false;
		}
		
		VX_LOG("GLFW initialized");

		// Set GLFW settings
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		// Add a callback for a window being created
		Window::s_OnWindowCreated.AddCallback([&](Window* window) { WindowCreatedCallback(window); });

		s_Initialized = true;
		VX_LOG("Renderer initialized");
		return true;
	}

	void Renderer::Tick(float deltaTime)
	{
		GLFWwindow* currentWindow = glfwGetCurrentContext();
		if (!glfwWindowShouldClose(currentWindow))
		{
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glfwSwapBuffers(currentWindow);
			glfwPollEvents();
		}
	}

	void Renderer::DrawTriangle()
	{
		// Load the shaders
		std::string vertexName = "Basic.vert";
		std::string vertexSource = ShaderLoader::Load(vertexName);

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		const char* vertexCStr = vertexSource.c_str();
		glShaderSource(vertexShader, 1, &vertexCStr, NULL);
		glCompileShader(vertexShader);

		GLint status;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			char buffer[512];
			glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
			VX_ERROR("Error compiling \"" + vertexName + "\":\n" + buffer);
		}

		std::string fragmentName = "Basic.frag";
		std::string fragmentSource = ShaderLoader::Load(fragmentName);

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		const char* fragmentCStr = fragmentSource.c_str();
		glShaderSource(fragmentShader, 1, &fragmentCStr, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			char buffer[512];
			glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
			VX_ERROR("Error compiling \"" + fragmentName + "\":\n" + buffer);
		}

		GLuint shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);

		glLinkProgram(shaderProgram);
		glUseProgram(shaderProgram);

		// Set the vertices
		float vertices[] = {
			0.0f, 0.5f,
			0.5f, -0.5f,
			-0.5f, -0.5f
		};

		// Load these verts into the vertex buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Get the position attribute off the shaders and set the rules for populating it with the data
		GLint positionAttribute = glGetAttribLocation(shaderProgram, "position");
		glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(positionAttribute);
	}

	void Renderer::WindowCreatedCallback(Window* window)
	{
		if (s_GLWindow)
		{
			VX_ERROR("Voltex only supports one window at a time");
			return;
		}

		s_EngineWindow = window;

		// Create a GLFW window
		s_GLWindow = glfwCreateWindow(window->GetWidth(), window->GetHeight(), window->GetName().c_str(), nullptr, nullptr);
		if (!s_GLWindow)
		{
			VX_ERROR("Failed to create GLFW window");
		}

		// Make this window the current window context
		glfwMakeContextCurrent(s_GLWindow);

		// Load GLAD
		if (!gladLoadGL(glfwGetProcAddress))
		{
			VX_ERROR("Failed to load GLAD");
			return;
		}
		VX_LOG("GLAD loaded");

		/*
		* IF RENDERING GETS SLOW, LOOK HERE!
		* 
		* Rather than creating different VAOs and buffers, we are using one for everything
		* This is not the "correct" way to handle rendering, but I don't want to get trapped in premature optimization
		* Once frames begin to drop, this should be the first place that is looked at for optimization
		*/

		// The window will use a single vertex array object
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// The window will use a single vertex buffer object
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		// TESTING ONLY
		DrawTriangle();
	}

}
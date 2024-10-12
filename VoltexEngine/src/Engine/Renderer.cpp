#include "vxpch.h"
#include "Renderer.h"

#include "Console.h"

#include <fstream>

namespace VoltexEngine {

	// Declare/Define static variables
	bool Renderer::s_Initialized = false;
	Window* Renderer::s_EngineWindow;
	GLFWwindow* Renderer::s_GLWindow;
	GLuint Renderer::s_ShaderProgram;

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
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glfwSwapBuffers(currentWindow);
			glfwPollEvents();
		}
	}

	void Renderer::DrawTriangle()
	{
		// Set the vertices
		GLfloat vertices[] = {
			-0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 1.0f, 0.0f, 1.0f
		};

		GLuint elements[] = {
			0, 1, 2,
			2, 3, 0
		};

		// Load verts into the vertex buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Load elements into the element buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

		// Set the position on the vertex
		GLint vertexInPosition = glGetAttribLocation(s_ShaderProgram, "inPosition");
		glVertexAttribPointer(vertexInPosition, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
		glEnableVertexAttribArray(vertexInPosition);

		// Set the color on the vertex
		GLint vertexInColor = glGetAttribLocation(s_ShaderProgram, "inColor");
		glVertexAttribPointer(vertexInColor, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(vertexInColor);
	}

	std::string Renderer::ReadShader(const std::string& name)
	{
		// Set the file path and open the file
		std::string path = "../VoltexEngine/src/Shaders/" + name;
		std::fstream file(path);

		// Make sure the file was opened correctly
		if (!file.is_open())
		{
			VX_WARNING("Could not find file: " + path);
			return "";
		}

		// Read the contents of the file
		std::string contents;
		std::string line;
		while (std::getline(file, line))
		{
			contents += (line + "\n");
		}

		// Close the file and return the contents
		file.close();
		return contents;
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

		// The window will use a single element buffer object
		GLuint ebo;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

		// Load the shaders
		std::string vertexName = "Basic.vert";
		std::string vertexSource = ReadShader(vertexName);

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
		std::string fragmentSource = ReadShader(fragmentName);

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

		s_ShaderProgram = glCreateProgram();
		glAttachShader(s_ShaderProgram, vertexShader);
		glAttachShader(s_ShaderProgram, fragmentShader);

		glLinkProgram(s_ShaderProgram);
		glUseProgram(s_ShaderProgram);

		// TESTING ONLY
		DrawTriangle();
	}

}
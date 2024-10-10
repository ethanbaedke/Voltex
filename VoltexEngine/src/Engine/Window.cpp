#include "vxpch.h"
#include "Window.h"

#include "Console.h"
#include "ShaderLoader.h"

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
				// Set GLFW settings
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
				glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
				glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

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

		// Make example triangle
		GLuint vertexArrayObject;
		glGenVertexArrays(1, &vertexArrayObject);
		glBindVertexArray(vertexArrayObject);

		float vertices[] = {
			-0.5f, 0.5f,
			0.5f, -0.5f,
			-0.5f, -0.5f
		};

		GLuint vertexBufferObject;
		glGenBuffers(1, &vertexBufferObject);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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

		GLint positionAttribute = glGetAttribLocation(shaderProgram, "position");
		glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(positionAttribute);
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_Window);
	}

	void Window::Update(float deltaTime)
	{
		if (!glfwWindowShouldClose(m_Window))
		{
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glfwSwapBuffers(m_Window);
			glfwPollEvents();
		}
	}

}
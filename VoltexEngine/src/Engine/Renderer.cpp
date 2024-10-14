#include "vxpch.h"
#include "Renderer.h"

#include "Console.h"
#include "FileLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
		// Load pixels as a texture
		int texWidth, texHeight, channels;
		unsigned char* pixels = stbi_load("../VoltexEngine/textures/MyHotGirlfriend.png", &texWidth, &texHeight, &channels, 0);
		GLenum format = (channels == 3 ? GL_RGB : GL_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, format, texWidth, texHeight, 0, format, GL_UNSIGNED_BYTE, pixels);

		// Set the vertices
		GLfloat vertices[] = {
		//	Position		Color				TexCoords
			-0.5f, -0.75f,	1.0f, 0.0f, 1.0f,	0.0f, 1.0f,
			0.5f, -0.75f,	0.0f, 1.0f, 1.0f,	1.0f, 1.0f,
			0.5f, 0.75f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
			-0.5f, 0.75f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f
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
		glVertexAttribPointer(vertexInPosition, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0);
		glEnableVertexAttribArray(vertexInPosition);

		// Set the color on the vertex
		GLint vertexInColor = glGetAttribLocation(s_ShaderProgram, "inColor");
		glVertexAttribPointer(vertexInColor, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(vertexInColor);

		// Set the texture coordinates on the vertex
		GLint vertexInTexCoords = glGetAttribLocation(s_ShaderProgram, "inTexCoords");
		glVertexAttribPointer(vertexInTexCoords, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(vertexInTexCoords);
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

		// The window will use a single texture
		GLuint tex;
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Load the shaders
		std::string vertexName = "Basic.vert";
		std::string vertexSource = FileLoader::LoadShader(vertexName);

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
		std::string fragmentSource = FileLoader::LoadShader(fragmentName);

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
#include "vxpch.h"
#include "Renderer.h"

#include "Console.h"
#include "FileLoader.h"
#include "Input.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace VoltexEngine {

	// Declare/Define static variables
	GLFWwindow* Renderer::s_Window;
	int Renderer::s_WindowWidth;
	int Renderer::s_WindowHeight;
	GLuint Renderer::s_ShaderProgram;
	GLuint Renderer::s_DefaultUITextureID;

	bool Renderer::Init(int windowWidth, int windowHeight)
	{
		if (s_Window)
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

		// Create a GLFW window
		s_Window = glfwCreateWindow(windowWidth, windowHeight, "Voltex Window", nullptr, nullptr);
		if (!s_Window)
		{
			VX_ERROR("Failed to create GLFW window");
			glfwTerminate();
			VX_LOG("GLFW terminated");
			return false;
		}
		s_WindowWidth = windowWidth;
		s_WindowHeight = windowHeight;

		// Make this window the current window context
		glfwMakeContextCurrent(s_Window);

		// Load GLAD
		if (!gladLoadGL(glfwGetProcAddress))
		{
			VX_ERROR("Failed to load GLAD");
			glfwTerminate();
			VX_LOG("GLFW terminated");
			return false;
		}
		VX_LOG("GLAD loaded");

		// Enable depth ordering for rendering
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

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
			VX_WARNING("Error compiling \"" + vertexName + "\":\n" + buffer);
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
			VX_WARNING("Error compiling \"" + fragmentName + "\":\n" + buffer);
		}

		s_ShaderProgram = glCreateProgram();
		glAttachShader(s_ShaderProgram, vertexShader);
		glAttachShader(s_ShaderProgram, fragmentShader);

		glLinkProgram(s_ShaderProgram);
		glUseProgram(s_ShaderProgram);

		// Set the vertices
		GLfloat vertices[] = {
			//	Position		TexCoords
				-0.5f, -0.5f,	0.0f, 1.0f,
				0.5f, -0.5f,	1.0f, 1.0f,
				0.5f, 0.5f,		1.0f, 0.0f,
				-0.5f, 0.5f,	0.0f, 0.0f
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
		glVertexAttribPointer(vertexInPosition, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glEnableVertexAttribArray(vertexInPosition);

		// Set the texture coordinates on the vertex
		GLint vertexInTexCoords = glGetAttribLocation(s_ShaderProgram, "inTexCoords");
		glVertexAttribPointer(vertexInTexCoords, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(vertexInTexCoords);

		// Load the UI texture
		int defaultUITextureWidth, defaultUITextureHeight;
		s_DefaultUITextureID = GenerateTexture("../VoltexEngine/textures/UIDefault.png", &defaultUITextureWidth, &defaultUITextureHeight);

		// Enable transparency rendering
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Bind input callbacks
		glfwSetKeyCallback(s_Window, KeyCallback);
		glfwSetMouseButtonCallback(s_Window, ClickCallback);
		glfwSetCursorPosCallback(s_Window, CursorCallback);

		VX_LOG("Renderer initialized");
		return true;
	}

	void Renderer::Tick(const std::vector<std::shared_ptr<GameObject>>& gameObjects, std::vector<std::shared_ptr<Gizmo>> gizmos)
	{
		GLFWwindow* currentWindow = glfwGetCurrentContext();

		if (!currentWindow)
		{
			VX_ERROR("GLFW window does not exist");
			return;
		}
		else if (glfwWindowShouldClose(currentWindow))
		{
			VX_ERROR("GLFW window should close");
			return;
		}

		glClearColor(0.13f, 0.16f, 0.27f, 1.0f); // Navy blue
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderGameObjects(gameObjects);

		RenderUI(gizmos);

		glfwSwapBuffers(currentWindow);
		glfwPollEvents();
	}

	unsigned int Renderer::GenerateTexture(const std::string& texturePath, int* outWidth, int* outHeight)
	{
		// Load image pixels into array
		int texWidth, texHeight, channels;
		unsigned char* pixels = stbi_load(texturePath.c_str(), &texWidth, &texHeight, &channels, 0);

		if (pixels == stbi__errpuc("can't fopen", "Unable to open file"))
		{
			VX_WARNING("Could not load texture: " + texturePath);
			return 0;
		}

		// Generate the texture
		GLuint texID;
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Load texture to graphics card
		GLenum format = (channels == 3 ? GL_RGB : GL_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, format, texWidth, texHeight, 0, format, GL_UNSIGNED_BYTE, pixels);

		*outWidth = texWidth;
		*outHeight = texHeight;

		VX_LOG("Loaded texture: " + texturePath);
		glBindTexture(GL_TEXTURE_2D, 0);
		return texID;
	}

	void Renderer::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		// Get the KeyCode
		KeyCode code;
		switch (key)
		{
		case 32:
			code = KeyCode::Space;
			break;
		case 48:
			code = KeyCode::Num0;
			break;
		case 49:
			code = KeyCode::Num1;
			break;
		case 50:
			code = KeyCode::Num2;
			break;
		case 51:
			code = KeyCode::Num3;
			break;
		case 52:
			code = KeyCode::Num4;
			break;
		case 53:
			code = KeyCode::Num5;
			break;
		case 54:
			code = KeyCode::Num6;
			break;
		case 55:
			code = KeyCode::Num7;
			break;
		case 56:
			code = KeyCode::Num8;
			break;
		case 57:
			code = KeyCode::Num9;
			break;
		case 65:
			code = KeyCode::A;
			break;
		case 66:
			code = KeyCode::B;
			break;
		case 67:
			code = KeyCode::C;
			break;
		case 68:
			code = KeyCode::D;
			break;
		case 69:
			code = KeyCode::E;
			break;
		case 70:
			code = KeyCode::F;
			break;
		case 71:
			code = KeyCode::G;
			break;
		case 72:
			code = KeyCode::H;
			break;
		case 73:
			code = KeyCode::I;
			break;
		case 74:
			code = KeyCode::J;
			break;
		case 75:
			code = KeyCode::K;
			break;
		case 76:
			code = KeyCode::L;
			break;
		case 77:
			code = KeyCode::M;
			break;
		case 78:
			code = KeyCode::N;
			break;
		case 79:
			code = KeyCode::O;
			break;
		case 80:
			code = KeyCode::P;
			break;
		case 81:
			code = KeyCode::Q;
			break;
		case 82:
			code = KeyCode::R;
			break;
		case 83:
			code = KeyCode::S;
			break;
		case 84:
			code = KeyCode::T;
			break;
		case 85:
			code = KeyCode::U;
			break;
		case 86:
			code = KeyCode::V;
			break;
		case 87:
			code = KeyCode::W;
			break;
		case 88:
			code = KeyCode::X;
			break;
		case 89:
			code = KeyCode::Y;
			break;
		case 90:
			code = KeyCode::Z;
			break;
		case 262:
			code = KeyCode::RightArrow;
			break;
		case 263:
			code = KeyCode::LeftArrow;
			break;
		case 264:
			code = KeyCode::DownArrow;
			break;
		case 265:
			code = KeyCode::UpArrow;
			break;
		case 257:
			code = KeyCode::Enter;
			break;
		case 258:
			code = KeyCode::Tab;
			break;
		case 340:
			code = KeyCode::Shift;
			break;
		case 341:
			code = KeyCode::Ctrl;
			break;
		case 342:
			code = KeyCode::Alt;
			break;
		default:
			return;
		}

		// Get the InputType
		Input::DiscreteType type;
		switch (action)
		{
		case 0:
			type = Input::DiscreteType::Up;
			break;
		case 1:
			type = Input::DiscreteType::Down;
			break;
		default:
			return;
		}

		Input::EnqueueKey(code, type);
	}

	void Renderer::ClickCallback(GLFWwindow* window, int button, int action, int mods)
	{
		// Get the KeyCode
		ClickCode code;
		switch (button)
		{
		case 0:
			code = ClickCode::LeftMouse;
			break;
		case 1:
			code = ClickCode::RightMouse;
			break;
		default:
			return;
		}

		// Get the InputType
		Input::DiscreteType type;
		switch (action)
		{
		case 0:
			type = Input::DiscreteType::Up;
			break;
		case 1:
			type = Input::DiscreteType::Down;
			break;
		default:
			return;
		}

		Input::EnqueueClick(code, type);
	}

	void Renderer::CursorCallback(GLFWwindow* window, double xPos, double yPos)
	{
		if (xPos > s_WindowWidth || yPos > s_WindowHeight)
			return;

		Input::SetCursorPosition(xPos / s_WindowWidth, 1 - (yPos / s_WindowHeight));
	}

	void Renderer::RenderGameObjects(const std::vector<std::shared_ptr<GameObject>>& gameObjects)
	{
		// Set the color on the fragment to white with full transparency
		GLint fragmentColor = glGetUniformLocation(s_ShaderProgram, "color");
		glUniform4f(fragmentColor, 1.0f, 1.0f, 1.0f, 1.0f);

		// The number of units visible on the screen horizontally
		float zoomOut = 10.0f;

		// Calculated from the center of the screen to the bounds
		float unitsX = zoomOut / 2.0f;
		float unitsY = ((float)s_WindowHeight / (float)s_WindowWidth) * unitsX;

		// The game objects passed into this function have already been filtered, no need to check for expiration
		for (std::shared_ptr<GameObject> obj : gameObjects)
		{
			if (std::shared_ptr<Sprite> spr = obj->GetSprite())
			{
				// Bind the texture for this sprite
				glBindTexture(GL_TEXTURE_2D, spr->GetTextureID());

				// Get the position, scale, and rotation of the game object
				Vector position = obj->GetPosition();
				float radianAngle = glm::radians(obj->GetRotation());
				int depth = obj->GetDepth();

				// Create and bind the model matrix
				glm::mat4 modelMatrix = glm::mat4(1.0f);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(position.X(), position.Y(), depth));
				modelMatrix = glm::rotate(modelMatrix, radianAngle, glm::vec3(0.0f, 0.0f, 1.0f));
				modelMatrix = glm::scale(modelMatrix, glm::vec3((float)spr->GetWidth() / PPU, (float)spr->GetHeight() / PPU, 0.0f));
				GLint vertexModelMatrix = glGetUniformLocation(s_ShaderProgram, "modelMatrix");
				glUniformMatrix4fv(vertexModelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

				// Create and bind the view matrix
				glm::mat4 viewMatrix = glm::lookAt(
					glm::vec3(0.0f, 0.0f, 5.0f),
					glm::vec3(0.0f, 0.0f, 0.0f),
					glm::vec3(0.0f, 1.0f, 0.0f)
				);
				GLint vertexViewMatrix = glGetUniformLocation(s_ShaderProgram, "viewMatrix");
				glUniformMatrix4fv(vertexViewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

				// Create and bind the projection matrix
				glm::mat4 projectionMatrix = glm::ortho(-unitsX, unitsX, -unitsY, unitsY, 1.0f, 10.0f);
				GLint vertexProjectionMatrix = glGetUniformLocation(s_ShaderProgram, "projectionMatrix");
				glUniformMatrix4fv(vertexProjectionMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
	}

	void Renderer::RenderUI(std::vector<std::shared_ptr<Gizmo>> gizmos)
	{
		// Draw the gizmo to the screen
		glBindTexture(GL_TEXTURE_2D, s_DefaultUITextureID);

		for (std::shared_ptr<Gizmo> giz : gizmos)
		{
			float xP, yP, xS, yS;
			giz->GetDimensions(&xP, &yP, &xS, &yS);

			// Set the UI color on the fragment shader
			Color uiColor = giz->GetColor();
			GLint fragmentColor = glGetUniformLocation(s_ShaderProgram, "color");
			glUniform4f(fragmentColor, uiColor.R() / 255.0f, uiColor.G() / 255.0f, uiColor.B() / 255.0f, uiColor.A() / 255.0f);

			// Create and bind the model matrix
			glm::mat4 modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::translate(modelMatrix, glm::vec3((2.0f * xP) + xS - 1.0f, (2.0f * yP) + yS - 1.0f, giz->GetDepth()));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(xS * 2.0f, yS * 2.0f, 1.0f));
			GLint vertexModelMatrix = glGetUniformLocation(s_ShaderProgram, "modelMatrix");
			glUniformMatrix4fv(vertexModelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

			// Create and bind the view matrix
			glm::mat4 viewMatrix = glm::mat4(1.0f);
			GLint vertexViewMatrix = glGetUniformLocation(s_ShaderProgram, "viewMatrix");
			glUniformMatrix4fv(vertexViewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

			// Create and bind the projection matrix
			glm::mat4 projectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -4.0f, 1.0f);
			GLint vertexProjectionMatrix = glGetUniformLocation(s_ShaderProgram, "projectionMatrix");
			glUniformMatrix4fv(vertexProjectionMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}

}
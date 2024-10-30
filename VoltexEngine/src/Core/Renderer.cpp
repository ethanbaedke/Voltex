#include "vxpch.h"
#include "Renderer.h"

#include "Console.h"
#include "FileLoader.h"
#include "Input.h"
#include "UI/LayoutGizmo.h"
#include "UI/TextGizmo.h"

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
	GLuint Renderer::s_FontTextureIDs[];

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

		// Load font textures, doing this manually to be explicit about the order they are stored in
		int width, height;
		s_FontTextureIDs[0] = GenerateTexture("../VoltexEngine/textures/font/Font_A.png", &width, &height);
		s_FontTextureIDs[1] = GenerateTexture("../VoltexEngine/textures/font/Font_B.png", &width, &height);
		s_FontTextureIDs[2] = GenerateTexture("../VoltexEngine/textures/font/Font_C.png", &width, &height);
		s_FontTextureIDs[3] = GenerateTexture("../VoltexEngine/textures/font/Font_D.png", &width, &height);
		s_FontTextureIDs[4] = GenerateTexture("../VoltexEngine/textures/font/Font_E.png", &width, &height);
		s_FontTextureIDs[5] = GenerateTexture("../VoltexEngine/textures/font/Font_F.png", &width, &height);
		s_FontTextureIDs[6] = GenerateTexture("../VoltexEngine/textures/font/Font_G.png", &width, &height);
		s_FontTextureIDs[7] = GenerateTexture("../VoltexEngine/textures/font/Font_H.png", &width, &height);
		s_FontTextureIDs[8] = GenerateTexture("../VoltexEngine/textures/font/Font_I.png", &width, &height);
		s_FontTextureIDs[9] = GenerateTexture("../VoltexEngine/textures/font/Font_J.png", &width, &height);
		s_FontTextureIDs[10] = GenerateTexture("../VoltexEngine/textures/font/Font_K.png", &width, &height);
		s_FontTextureIDs[11] = GenerateTexture("../VoltexEngine/textures/font/Font_L.png", &width, &height);
		s_FontTextureIDs[12] = GenerateTexture("../VoltexEngine/textures/font/Font_M.png", &width, &height);
		s_FontTextureIDs[13] = GenerateTexture("../VoltexEngine/textures/font/Font_N.png", &width, &height);
		s_FontTextureIDs[14] = GenerateTexture("../VoltexEngine/textures/font/Font_O.png", &width, &height);
		s_FontTextureIDs[15] = GenerateTexture("../VoltexEngine/textures/font/Font_P.png", &width, &height);
		s_FontTextureIDs[16] = GenerateTexture("../VoltexEngine/textures/font/Font_Q.png", &width, &height);
		s_FontTextureIDs[17] = GenerateTexture("../VoltexEngine/textures/font/Font_R.png", &width, &height);
		s_FontTextureIDs[18] = GenerateTexture("../VoltexEngine/textures/font/Font_S.png", &width, &height);
		s_FontTextureIDs[19] = GenerateTexture("../VoltexEngine/textures/font/Font_T.png", &width, &height);
		s_FontTextureIDs[20] = GenerateTexture("../VoltexEngine/textures/font/Font_U.png", &width, &height);
		s_FontTextureIDs[21] = GenerateTexture("../VoltexEngine/textures/font/Font_V.png", &width, &height);
		s_FontTextureIDs[22] = GenerateTexture("../VoltexEngine/textures/font/Font_W.png", &width, &height);
		s_FontTextureIDs[23] = GenerateTexture("../VoltexEngine/textures/font/Font_X.png", &width, &height);
		s_FontTextureIDs[24] = GenerateTexture("../VoltexEngine/textures/font/Font_Y.png", &width, &height);
		s_FontTextureIDs[25] = GenerateTexture("../VoltexEngine/textures/font/Font_Z.png", &width, &height);

		// Bind input callbacks
		glfwSetKeyCallback(s_Window, KeyCallback);
		glfwSetMouseButtonCallback(s_Window, ClickCallback);
		glfwSetCursorPosCallback(s_Window, CursorCallback);

		VX_LOG("Renderer initialized");
		return true;
	}

	void Renderer::Tick(const std::vector<std::shared_ptr<GameObject>>& gameObjects, std::vector<std::shared_ptr<Gizmo>> rootGizmos)
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

		RenderUI(rootGizmos);

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
		float zoomOut = 82.0f;

		// Calculated from the center of the screen to the bounds
		float unitsX = zoomOut / 2.0f;
		float unitsY = ((float)s_WindowHeight / (float)s_WindowWidth) * unitsX;

		// The game objects passed into this function have already been filtered, no need to check for expiration
		for (std::shared_ptr<GameObject> obj : gameObjects)
		{
			if (std::shared_ptr<Sprite> spr = obj->ObjectSprite)
			{
				// Bind the texture for this sprite
				glBindTexture(GL_TEXTURE_2D, spr->GetTextureID());

				// Get the position, scale, and rotation of the game object
				Vector position = obj->Position;
				float radianAngle = glm::radians(obj->Angle);
				int depth = obj->Depth;

				// Create and bind the model matrix
				glm::mat4 modelMatrix = glm::mat4(1.0f);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(position.X, position.Y, depth));
				modelMatrix = glm::rotate(modelMatrix, radianAngle, glm::vec3(0.0f, 0.0f, 1.0f));
				modelMatrix = glm::scale(modelMatrix, glm::vec3((float)spr->GetWidth() / PPU, (float)spr->GetHeight() / PPU, 0.0f));
				GLint vertexModelMatrix = glGetUniformLocation(s_ShaderProgram, "modelMatrix");
				glUniformMatrix4fv(vertexModelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

				// Create and bind the view matrix
				glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-unitsX + 1.5f, unitsY - 1.5f, 0.0f));
				GLint vertexViewMatrix = glGetUniformLocation(s_ShaderProgram, "viewMatrix");
				glUniformMatrix4fv(vertexViewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

				// Create and bind the projection matrix
				glm::mat4 projectionMatrix = glm::ortho(-unitsX, unitsX, -unitsY, unitsY);
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

	void Renderer::RenderUI(std::vector<std::shared_ptr<Gizmo>> rootGizmos)
	{
		// Create and bind the view matrix
		glm::mat4 viewMatrix = glm::mat4(1.0f);
		GLint vertexViewMatrix = glGetUniformLocation(s_ShaderProgram, "viewMatrix");
		glUniformMatrix4fv(vertexViewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

		// Create and bind the projection matrix
		glm::mat4 projectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -6.0f, 1.0f);
		GLint vertexProjectionMatrix = glGetUniformLocation(s_ShaderProgram, "projectionMatrix");
		glUniformMatrix4fv(vertexProjectionMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		// Depth first render all gizmos, render as we go
		std::vector<std::shared_ptr<Gizmo>> renderStack(rootGizmos);

		while (renderStack.size() > 0)
		{
			// Pop the next gizmo off the top of the stack
			std::shared_ptr<Gizmo> giz = renderStack.back();
			renderStack.pop_back();

			// Get the dimensions of the current gizmo
			float xP, yP, xS, yS;
			giz->GetDimensions(&xP, &yP, &xS, &yS);

			// Set the UI color on the fragment shader
			Color gizColor = giz->GetColor();
			GLint fragmentColor = glGetUniformLocation(s_ShaderProgram, "color");
			glUniform4f(fragmentColor, gizColor.R / 255.0f, gizColor.G / 255.0f, gizColor.B / 255.0f, gizColor.A / 255.0f);

			// If the current gizmo is a layout gizmo, get its children and add them to the render stack
			if (std::shared_ptr<LayoutGizmo> layGiz = std::dynamic_pointer_cast<LayoutGizmo>(giz))
			{
				std::vector<std::shared_ptr<Gizmo>> children = layGiz->GetChildren();
				for (int i = children.size() - 1; i > -1; i--)
					renderStack.push_back(children[i]);
			}
			// Otherwise, if the current gizmo is a text gizmo, render its text to the screen and move to the next iteration
			else if (std::shared_ptr<TextGizmo> textGiz = std::dynamic_pointer_cast<TextGizmo>(giz))
			{
				int len = textGiz->Text.length();
				xS /= textGiz->Text.length();
				for (int i = 0; i < textGiz->Text.length(); i++)
				{
					if ((textGiz->Text[i] > 0x40 && textGiz->Text[i] < 0x5B))
					{
						glm::mat4 modelMatrix = glm::mat4(1.0f);
						modelMatrix = glm::translate(modelMatrix, glm::vec3((2.0f * xP) + xS - 1.0f, (2.0f * yP) + yS - 1.0f, giz->Depth));
						modelMatrix = glm::scale(modelMatrix, glm::vec3(xS * 2.0f, yS * 2.0f, 1.0f));
						GLint vertexModelMatrix = glGetUniformLocation(s_ShaderProgram, "modelMatrix");
						glUniformMatrix4fv(vertexModelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

						glBindTexture(GL_TEXTURE_2D, s_FontTextureIDs[textGiz->Text[i] - 0x41]);
						glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
					}

					xP += xS;
				}
				// We do this because we dont want to render the text gizmo itself, just the text it holds
				continue;
			}

			// Set the texture for this gizmo
			if (giz->UISprite)
				glBindTexture(GL_TEXTURE_2D, giz->UISprite->GetTextureID());
			else
				glBindTexture(GL_TEXTURE_2D, s_DefaultUITextureID);

			// Create and bind the model matrix
			glm::mat4 modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::translate(modelMatrix, glm::vec3((2.0f * xP) + xS - 1.0f, (2.0f * yP) + yS - 1.0f, giz->Depth));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(xS * 2.0f, yS * 2.0f, 1.0f));
			GLint vertexModelMatrix = glGetUniformLocation(s_ShaderProgram, "modelMatrix");
			glUniformMatrix4fv(vertexModelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

			// Draw the elements to the screen
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}

}
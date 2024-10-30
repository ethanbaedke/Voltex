#pragma once
#include "vxpch.h"

#include "GameObject.h"
#include "Sprite.h"
#include "UI/Gizmo.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace VoltexEngine {

	class Renderer
	{

	private:

		static const int PPU = 16;

		static GLFWwindow* s_Window;
		static int s_WindowWidth;
		static int s_WindowHeight;

		static GLuint s_ShaderProgram;

		static GLuint s_DefaultUITextureID;

		/* A list containing the texture IDs of all the letters of the alphabet for text rendering */
		static GLuint s_FontTextureIDs[26];

	public:

		static bool Init(int windowWidth, int windowHeight);

		/* Update all images on the screen */
		static void Tick(const std::vector<std::shared_ptr<GameObject>>& gameObjects, std::vector<std::shared_ptr<Gizmo>> gizmos);

		/* Reads the image at texture path, generates a texture, and returns its textureID */
		static unsigned int GenerateTexture(const std::string& texturePath, int* outWidth, int* outHeight);

		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void ClickCallback(GLFWwindow* window, int button, int action, int mods);
		static void CursorCallback(GLFWwindow* window, double xPos, double yPos);

		inline static bool GetCursorEnabled() { return glfwGetInputMode(s_Window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL; }

	private:

		static void RenderGameObjects(const std::vector<std::shared_ptr<GameObject>>& gameObjects);
		static void RenderUI(std::vector<std::shared_ptr<Gizmo>> gizmos);

	};

}
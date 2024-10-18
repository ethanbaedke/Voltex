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

	public:

		static bool Init(int windowWidth, int windowHeight);

		/* Update all images on the screen */
		static void Tick(const std::vector<std::shared_ptr<GameObject>>& gameObjects, std::shared_ptr<Gizmo> rootGizmo);

		/* Reads the image at texture path, generates a texture, and returns its textureID */
		static unsigned int GenerateTexture(const std::string& texturePath, int* outWidth, int* outHeight);

		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void ClickCallback(GLFWwindow* window, int button, int action, int mods);

	private:

		static void RenderGameObjects(const std::vector<std::shared_ptr<GameObject>>& gameObjects);
		static void RenderUI(std::shared_ptr<Gizmo> rootGizmo);

	};

}
#pragma once
#include "vxpch.h"

#include "GameObject.h"
#include "Sprite.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace VoltexEngine {

	class Renderer
	{

	private:

		static GLFWwindow* s_Window;
		static int s_WindowWidth;
		static int s_WindowHeight;

		static GLuint s_ShaderProgram;

	public:

		static bool Init(int windowWidth, int windowHeight);

		static void Tick(const std::vector<std::shared_ptr<GameObject>>& gameObjects);

		/* Reads the image at texture path, generates a texture, and returns its textureID */
		static unsigned int GenerateTexture(const std::string& texturePath);

	};

}
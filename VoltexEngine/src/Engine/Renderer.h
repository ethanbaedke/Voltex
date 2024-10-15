#pragma once
#include "vxpch.h"

#include "Window.h"
#include "GameObject.h"
#include "Sprite.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace VoltexEngine {

	class Renderer
	{

	private:

		static bool s_Initialized;

		static std::weak_ptr<Window> s_EngineWindow;
		static GLFWwindow* s_GLWindow;

		static GLuint s_ShaderProgram;

	public:

		static bool Init();

		static void Tick(float deltaTime, const std::vector<std::weak_ptr<GameObject>>& gameObjects);

	private:

		/* Called when an engine window is created */
		static void HandleWindowCreated(std::weak_ptr<Window> window);

		/* Called when a sprite is created */
		static void HandleSpriteCreated(std::weak_ptr<Sprite> sprite, const std::string& texturePath);

	};

}
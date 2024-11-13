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

		/* The following two structs and map are to hold generated sprites with their coresponding texture IDs */
		struct SpriteHash {
			std::size_t operator()(const std::shared_ptr<Sprite>& ptr) const {
				return std::hash<std::uintptr_t>()(reinterpret_cast<std::uintptr_t>(ptr.get()));
			}
		};
		struct SpriteEqual {
			bool operator()(const std::shared_ptr<Sprite>& lhs, const std::shared_ptr<Sprite>& rhs) const {
				return lhs.get() == rhs.get();
			}
		};
		static std::unordered_map<std::shared_ptr<Sprite>, GLuint, SpriteHash, SpriteEqual> s_SpriteMap;

		static std::shared_ptr<Sprite> s_DefaultUISprite;

		/* A list containing the texture IDs of all the letters of the alphabet for text rendering */
		static std::shared_ptr<Sprite> s_FontSprites[26];

	public:

		static bool Init(int windowWidth, int windowHeight);

		/* Update all images on the screen */
		static bool Tick(const std::vector<std::shared_ptr<GameObject>>& gameObjects, std::vector<std::shared_ptr<Gizmo>> gizmos);

		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void ClickCallback(GLFWwindow* window, int button, int action, int mods);
		static void CursorCallback(GLFWwindow* window, double xPos, double yPos);

		inline static bool GetCursorEnabled() { return glfwGetInputMode(s_Window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL; }

	private:

		static void HandleSpriteCreated(std::shared_ptr<Sprite> sprite, const std::string& texturePath);

		static void RenderGameObjects(const std::vector<std::shared_ptr<GameObject>>& gameObjects);
		static void RenderUI(std::vector<std::shared_ptr<Gizmo>> gizmos);

	};

}
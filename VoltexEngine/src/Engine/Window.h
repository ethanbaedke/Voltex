#pragma once
#include "vxpch.h"

#include "Event.h"

namespace VoltexEngine {

	class Window
	{

	public:

		/* Broadcasts a window whenever one is created */
		static Event_OneParam<std::weak_ptr<Window>> s_OnWindowCreated;

	public:

		/* Creates and returns a window */
		static std::shared_ptr<Window> Create(const std::string& name, int width, int height);

	private:

		std::string m_Name;
		
		int m_Width;
		int m_Height;

	public:

		inline const std::string& GetName() { return m_Name; }
		inline int GetWidth() { return m_Width; }
		inline int GetHeight() { return m_Height; }

	private:

		/* Private constructor forces the use of the create function */
		Window(const std::string& name, int width, int height);

	};

}


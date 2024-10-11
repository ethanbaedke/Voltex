#pragma once
#include "vxpch.h"

#include "Event.h"

namespace VoltexEngine {

	class Window
	{

	public:

		/* Broadcasts a window whenever one is created */
		static Event_OneParam<Window*> s_OnWindowCreated;

	private:

		std::string m_Name;
		
		int m_Width;
		int m_Height;

	public:

		Window(const std::string& name, int width, int height);

		inline const std::string& GetName() { return m_Name; }
		inline int GetWidth() { return m_Width; }
		inline int GetHeight() { return m_Height; }

	};

}


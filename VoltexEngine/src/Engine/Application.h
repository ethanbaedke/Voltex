#pragma once
#include "vxpch.h"

#include "Window.h"

namespace VoltexEngine {

	/*
	* The "Voltex Engine" is a set of classes meant to abstract complex logic so applications can be developed at a higher level
	* The Application class is meant to be derived from by the application using the engine
	*/
	class Application
	{

	private:

		std::unique_ptr<Window> m_Window;

	public:

		Application();

		void Run();

	};

}

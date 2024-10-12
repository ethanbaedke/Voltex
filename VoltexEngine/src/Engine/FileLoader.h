#pragma once
#include "vxpch.h"

namespace VoltexEngine {

	/* Handles loading various types of files the engine will use */
	class FileLoader
	{

	public:

		static std::string LoadShader(const std::string& fileName);

	};

}

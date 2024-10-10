#include "vxpch.h"
#include "ShaderLoader.h"

#include "Console.h"

#include <fstream>

namespace VoltexEngine {

	std::string ShaderLoader::Load(const std::string& fileName)
	{
		// Set the file path and open the file
		std::string path = "../VoltexEngine/src/Shaders/" + fileName;
		std::fstream file(path);

		// Make sure the file was opened correctly
		if (!file.is_open())
		{
			VX_WARNING("Could not find file: " + path);
			return "";
		}

		// Read the contents of the file
		std::string contents;
		std::string line;
		while (std::getline(file, line))
		{
			contents += (line + "\n");
		}

		// Close the file and return the contents
		file.close();
		return contents;
	}

}

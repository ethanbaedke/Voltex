#pragma once
#include "vxpch.h"

namespace VoltexEngine {

	class IPrintable {

	public:

		/* Called to get a string representation of an object */
		virtual std::string GetPrintable() const = 0;

	};

}
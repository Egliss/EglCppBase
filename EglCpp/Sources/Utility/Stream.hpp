#pragma once

#include <fstream>
#include <filesystem>

namespace Egliss
{
	enum struct IOMode : char
	{
		Output = std::ios::out,
		Append = std::ios::app,
		Trunk = std::ios::trunc,
		Input = std::ios::in,
		Binary = std::ios::binary
	};
}

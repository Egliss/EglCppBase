#pragma once

#include <string_view>

namespace Egliss
{
	class AppConfiguration
	{
	public:
		static constexpr std::string_view ApplicationName = "App";
		static constexpr std::string_view ApplicationClassName = "App";
		static constexpr std::string_view ShaderRoot = "Resources/Shaders/";
		static constexpr std::string_view FontRoot = "Resources/Fonts/";
		static constexpr std::wstring_view DefaultFont = L"Resources/Fonts/default.spritefont";
	};
}
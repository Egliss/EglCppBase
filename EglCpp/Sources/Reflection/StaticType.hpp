#pragma once

#include <string_view>

namespace Egliss::Reflection
{
	template<class T>
	class StaticTypeDescription
	{
		// å^å≈óLÇÃId
		// static constexpr int Id = 0;
		// ÉNÉâÉXÇÃñºèÃ
		// static constexpr std::string_view Name = "NameSpace::ClassName";
	};
	template<class T>
	using StaticType = StaticTypeDescription<T>;
}

#include "StaticType.inl"

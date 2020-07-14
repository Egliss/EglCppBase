#pragma once

#include <string_view>

namespace Egliss::Reflection
{
	template<class T>
	class StaticTypeDescription
	{
		// ^ÅLÌId
		// static constexpr int Id = 0;
		// NXÌ¼Ì
		// static constexpr std::string_view Name = "NameSpace::ClassName";
	};
	template<class T>
	using StaticType = StaticTypeDescription<T>;
}

#include "StaticType.inl"

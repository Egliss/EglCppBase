#pragma once

#include <string_view>

namespace Egliss::Reflection
{
	template<class T>
	class StaticTypeDescription
	{
		// �^�ŗL��Id
		// static constexpr int Id = 0;
		// �N���X�̖���
		// static constexpr std::string_view Name = "NameSpace::ClassName";
	};
}
namespace Egliss
{
	template<class T>
	using StaticType = Reflection::StaticTypeDescription<T>;
}
#include "StaticType.inl"

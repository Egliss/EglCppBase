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

#include "StaticType.inl"

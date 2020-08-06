#pragma once

// TODO �v�R���p�C�����Ԍv��
#include "StaticType.hpp"

namespace Egliss::Reflection
{
	// ���t���N�V�����Ώ�
	class Reflectable abstract
	{
	public:
		Reflectable(int typeId) noexcept : TypeId(typeId) { }
		virtual ~Reflectable() noexcept { };
	private:
		const int TypeId = -1;

		void Validate(int typeId);
		bool TryFastValidate(int typeId) noexcept
		{
			return typeId == TypeId;
		}
	};
}
#pragma once

// TODO 要コンパイル時間計測
#include "StaticType.hpp"

namespace Egliss::Reflection
{
	// リフレクション対象
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
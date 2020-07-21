#pragma once

#include <winerror.h>
#include <exception>

namespace Egliss
{
	class DirectXUtility
	{
	public:
		static inline void ThrowIfFailed(HRESULT result)
		{
			if (FAILED(result))
				throw std::exception("create command list was failed.");
		}
	};
}
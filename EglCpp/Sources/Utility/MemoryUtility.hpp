#pragma once

#include <cstring>

namespace Egliss
{
	class MemoryUtility
	{
	public:
		template<class T>
		static void Fill(T& dest, unsigned char fillValue = 0, std::size_t byteSize = sizeof(T))
		{
			std::memset(&dest, fillValue, byteSize);
		}
		template<class T>
		static const void* AsVoidPtr(const T& ref)
		{
			return reinterpret_cast<const void*>(&ref);
		}

		template<class T>
		static void* AsVoidPtr(T& ref)
		{
			return reinterpret_cast<const void*>(&ref);
		}
		template<class T>
		static const void* AsVoidPtr(const T* ptr)
		{
			return reinterpret_cast<const void*>(&ptr);
		}
		template<class T>
		static void* AsVoidPtr(T* ptr)
		{
			return reinterpret_cast<const void*>(&ptr);
		}
	};
}
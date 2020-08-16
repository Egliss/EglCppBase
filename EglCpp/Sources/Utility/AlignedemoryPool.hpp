#pragma once

#include <memory_resource>
#include "../App/IApplicationComponent.hpp"

namespace Egliss
{
	// 4byte step aligned pool
	template<class T, unsigned int AlignedByteRequest = 0>
	class AlignedemoryPool : public std::pmr::synchronized_pool_resource
	{
	public:
		static constexpr int AlignedByte = (AlignedByteRequest % 4 == 0)
			? AlignedByteRequest
			: AlignedByteRequest + AlignedByteRequest % 4;
		AlignedemoryPool() : std::pmr::synchronized_pool_resource(std::pmr::pool_options({ 8192, 4096 }))
		{
		}
		T* Allocate()
		{
			return reinterpret_cast<T*>(this->do_allocate(sizeof(T), AlignedByte));
		}
		void Deallocate(T* ptr)
		{
			this->do_deallocate(reinterpret_cast<void*>(ptr), sizeof(T), AlignedByte);
		}
		void DisposePool()
		{
			this->release();
		}
	};

	// un typed memory pool (default is 4byte step aligne)
	template<>
	class AlignedemoryPool<void> : public std::pmr::synchronized_pool_resource
	{
	public:
		static constexpr int DefaultAlignedByte = 4;
		
		AlignedemoryPool(int aligne = DefaultAlignedByte) :
			std::pmr::synchronized_pool_resource(std::pmr::pool_options({ 8192, 4096 })),
			Aligne(aligne)
		{}
		template<class T>
		T* Allocate()
		{
			return reinterpret_cast<T*>(this->do_allocate(sizeof(T), Aligne));
		}
		template<class T>
		void Deallocate(T* ptr)
		{
			this->do_deallocate(reinterpret_cast<void*>(ptr), sizeof(T), Aligne);
		}
		void DisposePool()
		{
			this->release();
		}
	private:
		const int Aligne;
	};

	template<class T, unsigned int AlignedByteRequest = 0>
	using TypedAlignedMemoryPool = AlignedemoryPool<T, AlignedByteRequest>;
	using UnTypedAlignedMemoryPool = AlignedemoryPool<void>;
}


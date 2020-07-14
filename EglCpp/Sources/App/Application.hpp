#pragma once

#include "IApplicationImpl.h"
#include "IApplicationInitializeArg.h"

#include <memory>

namespace Egliss
{
	class Application
	{
	public:
		static bool Initialize(std::unique_ptr<IApplicationImpl>&& impl, IApplicationInitializeArg&& arg);
		static void Finalize();

		// Cast to platform implement 
		template<class T>
		static T& GetImplAs()
		{
			return static_cast<T&>(*_impl.get());
		}
		// Get platform implement
		static IApplicationImpl& GetImpl()
		{
			return *_impl.get();
		}

	private:
		static std::unique_ptr<IApplicationImpl> _impl;
	};
}
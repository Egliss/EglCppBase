#pragma once

#include "../App/IApplicationInitializeArg.h"
#include <string>

namespace Egliss
{
	class IApplicationImpl
	{
	public:

		virtual ~IApplicationImpl() {};

		virtual bool Initialize(IApplicationInitializeArg&& arg) = 0;
		virtual void Finalize() = 0;

		virtual std::string GetName() const = 0;
	};
}
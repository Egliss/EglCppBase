#pragma once

#include <string>

namespace Egliss
{
	class IApplicationComponent
	{
	public:
		virtual ~IApplicationComponent() {}

		virtual void Initialize() = 0;
		virtual void Finalize() = 0;

		virtual std::string Name() const = 0;
	};
	using IAppComponent = IApplicationComponent;
}
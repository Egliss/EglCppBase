#pragma once

#include "../Reflection/IReflectable.hpp"
#include <string>

namespace Egliss
{
	class IApplicationComponent : public IReflectable
	{
	public:
		virtual ~IApplicationComponent() {}

		virtual void Initialize() = 0;
		virtual void Finalize() = 0;
		virtual void AllComponentInitialized() {};

		virtual std::string Name() const = 0;
	};
	using IAppComponent = IApplicationComponent;

	class IAppComponentUpdateLister : public IReflectable
	{
	public:
		virtual ~IAppComponentUpdateLister() {}
		virtual void Update() = 0;
	};
}
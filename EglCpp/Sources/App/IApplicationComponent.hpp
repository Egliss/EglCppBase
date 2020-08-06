#pragma once

#include <string>
#include "../Reflection/Reflectable.hpp"

namespace Egliss
{
	class IApplicationComponent : public Reflection::Reflectable
	{
	public:
		IApplicationComponent(int typeId) : Reflectable(typeId) {}
		virtual ~IApplicationComponent() {}

		virtual void Initialize() = 0;
		virtual void Finalize() = 0;
		virtual void AllComponentInitialized() {};

		virtual std::string Name() const = 0;
	};
	using IAppComponent = IApplicationComponent;

	class IAppComponentUpdateLister : public Reflection::Reflectable
	{
	public:
		IAppComponentUpdateLister(int typeId) : Reflectable(typeId) {}
		virtual ~IAppComponentUpdateLister() {}
		virtual void Update() = 0;
	};
}
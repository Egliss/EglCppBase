#pragma once

#include "IApplicationImpl.hpp"
#include "IApplicationInitializeArg.hpp"
#include "IApplicationComponent.hpp"
#include "Reflection/StaticType.hpp"
#include "Reflection/DynamicType.hpp"

#include <memory>
#include <map>

namespace Egliss
{
	class Application
	{
		// TODO Delay attachable app component
	public:
		static bool Initialize(std::unique_ptr<IApplicationImpl>&& impl, IApplicationInitializeArg&& arg);
		static void Finalize();
		static void Update()
		{
			for (auto&& component : Application::_updatableComponents)
			{
				component.second->Update();
			}
		}
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
		template<class T>
		static T& GetAppComponent()
		{
			// GetAppComponent failed. the type is ambiguous definition
			// please include T definition header.
			constexpr auto id = StaticType<T>::Id;
		#ifdef _DEBUG
			constexpr auto name = StaticType<T>::Name;
		#endif // _DEBUG
			auto& ref = *(_components[id].get());
			return static_cast<T&>(ref);
		}
		template<class T>
		static T* FindAppComponent()
		{
			constexpr auto id = StaticType<T>::Id;
			if (Reflection::DynamicTypeManager::Find(id) == nullptr)
				return nullptr;
			
			auto& ref = *(_components[id].get());
			return static_cast<T&>(ref);
		}

	private:
		static std::unique_ptr<IApplicationImpl> _impl;
		static std::map<int, std::unique_ptr<IApplicationComponent>> _components;
		static std::map<int, IAppComponentUpdateLister*> _updatableComponents;
	};
}
#include "pch.h"
#include "Application.hpp"
#include "AppConfiguration.hpp"
#include "Reflection/StaticType.hpp"
#include "Reflection/DynamicType.hpp"

using namespace Egliss;

std::unique_ptr<IApplicationImpl> Application::_impl = std::unique_ptr<IApplicationImpl>(nullptr);
std::map<int, std::unique_ptr<IApplicationComponent>> Application::_components;
std::map<int, IAppComponentUpdateLister*> Application::_updatableComponents;

bool Application::Initialize(std::unique_ptr<IApplicationImpl>&& impl, IApplicationInitializeArg&& arg)
{
	using namespace Egliss::Reflection;
	DynamicTypeManager::Initialize();

	Application::_impl = std::move(impl);
	auto initialized = Application::_impl->Initialize(std::move(arg));
	if (!initialized)
		return false;

	// Initialize All AppComponents
	const auto appComponentId = DynamicTypeManager::IndexOf(StaticType<IAppComponent>::Id).id;
	const auto updatableComponentId = DynamicTypeManager::IndexOf(StaticType<IAppComponentUpdateLister>::Id).id;
	for (auto&& type : DynamicTypeManager::Types())
	{
		if (type.isAbstract)
			continue;
		if (!type.IsChildOf(appComponentId))
			continue;

		auto component = reinterpret_cast<IAppComponent*>(type.constructor());
		auto uniquePtr = std::unique_ptr<IAppComponent>(component);

		Application::_components.emplace(type.id, std::move(uniquePtr));

		if (type.IsChildOf(updatableComponentId))
		{
			auto updatable = reinterpret_cast<IAppComponentUpdateLister*>(component);
			Application::_updatableComponents.emplace(type.id, updatable);
		}
		
		component->Initialize();
	}

	// Initialized Event
	for (auto&& component : Application::_components)
	{
		component.second->AllComponentInitialized();
	}

	return true;
}

void Application::Finalize()
{
	Application::_updatableComponents.clear();
	Application::_components.clear();
	Application::_impl->Finalize();
	Application::_impl.reset(nullptr);

	Reflection::DynamicTypeManager::Finalize();
}

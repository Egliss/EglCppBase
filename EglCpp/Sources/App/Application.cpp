#include "pch.h"
#include "Application.hpp"
#include "AppConfiguration.hpp"
#include "Reflection/StaticType.hpp"
#include "Reflection/DynamicType.hpp"

using namespace Egliss;

std::unique_ptr<IApplicationImpl> Application::_impl = std::unique_ptr<IApplicationImpl>(nullptr);
std::map<int, std::unique_ptr<IApplicationComponent>> Application::_components;

bool Application::Initialize(std::unique_ptr<IApplicationImpl>&& impl, IApplicationInitializeArg&& arg)
{
	using namespace Egliss::Reflection;
	DynamicTypeManager::Initialize();

	Application::_impl = std::move(impl);
	auto initialized = Application::_impl->Initialize(std::move(arg));
	if (!initialized)
		return false;

	const auto interfaceId = DynamicTypeManager::IndexOf(StaticType<IAppComponent>::Id).Id();
	for (auto&& type : DynamicTypeManager::Types())
	{
		if (type.isAbstract)
			continue;
		if (!type.HasTypeRelation(interfaceId))
			continue;
		auto ptr = std::unique_ptr<IAppComponent>(reinterpret_cast<IAppComponent*>(type.constructor()));
		ptr->Initialize();
		Application::_components.emplace(type.Id(), std::move(ptr));
	}
	return true;
}

void Application::Finalize()
{
	Application::_components.clear();
	Application::_impl->Finalize();
	Application::_impl.reset(nullptr);

	Reflection::DynamicTypeManager::Finalize();
}

#include "pch.h"
#include "Application.hpp"
#include "AppConfiguration.hpp"

using namespace Egliss;

std::unique_ptr<IApplicationImpl> Application::_impl = std::unique_ptr<IApplicationImpl>(nullptr);

bool Application::Initialize(std::unique_ptr<IApplicationImpl>&& impl, IApplicationInitializeArg&& arg)
{
	Application::_impl = std::move(impl);
	return Application::_impl->Initialize(std::move(arg));
}

void Application::Finalize()
{
	Application::_impl->Finalize();
	Application::_impl.reset(nullptr);
}

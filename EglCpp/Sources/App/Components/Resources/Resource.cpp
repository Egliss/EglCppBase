#include "pch.h"
#include "Resource.hpp"
#include "Resources.hpp"
#include "App/Application.hpp"

using namespace Egliss;

void Resource::Initialize(std::shared_ptr<Resource> resource)
{
	if (!resource)
	{
		throw std::exception("invalid resource detected --> resource reference(_ownRef) not initialized.");
	}
	this->_ownRef = resource;
	const auto avalable = this->InternalInitialize();
	if (avalable)
	{
		Application::GetAppComponent<Resources>().Register(this->_key, resource);
	}
	else
	{
		throw std::exception("not implement --> Resources::Register failed.");
	}
}

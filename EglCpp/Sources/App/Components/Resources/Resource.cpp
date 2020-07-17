#include "pch.h"
#include "Resource.hpp"
#include "Resources.hpp"
#include "App/Application.hpp"

using namespace Egliss;

void Resource::Initialize(const std::string& key, const std::string& path)
{
	this->_key = key;

	const auto avalable = this->InternalInitialize(path);
	if (avalable)
	{
		Application::GetAppComponent<Resources>().Register(key, path);
	}
	else
	{
		throw std::exception("not implement --> Resources::Register failed.");
	}
}

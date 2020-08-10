#include "pch.h"
#include "Resource.hpp"
#include "Resources.hpp"
#include "App/Application.hpp"

using namespace Egliss;

Resource::Resource()
{
	static int InternalIndex = 0;
	this->_internalIndex = InternalIndex++;
}

void Resource::Register(std::shared_ptr<Resource> resource)
{
	if (!resource)
	{
		throw std::exception("invalid resource detected --> Initialize require constructed resource shared pointer.");
	}
	this->_ownRef = resource;
	
	Application::GetAppComponent<Resources>().Register(resource);
}

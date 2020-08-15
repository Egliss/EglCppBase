#include "pch.h"
#include "Resource.hpp"
#include "Resources.hpp"
#include "App/Application.hpp"

using namespace Egliss;

Resource::Resource()
{
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

bool Resource::Initialize()
{
	static int InternalIndex = 0;
	this->_internalIndex = InternalIndex++;
	return true;
}

void Resource::Finalize()
{
	this->_internalIndex = -1;
	this->_initialized = false;
	this->_ownRef = std::shared_ptr<Resource>();
	this->_key += "(finalized)";
}

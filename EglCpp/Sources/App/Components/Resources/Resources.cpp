#include "pch.h"
#include "Resources.hpp"

// #include "Windows/Rendering/Texture2D.hpp"

using namespace Egliss;

void Resources::Initialize()
{
	// insert core resource
	// Rendering::Texture2D::Create("Icon", "Resources/app_icon.png");
}

void Resources::Finalize()
{
	this->_dynamicResource.clear();
	this->_physicalResources.clear();
}

void Resources::Register(std::shared_ptr<Resource> resource)
{
	if (resource->Key().empty())
	{
		this->_dynamicResource.emplace_back(resource);
	}
	else
	{
		this->_physicalResources.emplace(resource->Key(), resource);
	}
}

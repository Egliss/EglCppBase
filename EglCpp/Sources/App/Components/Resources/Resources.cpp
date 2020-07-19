#include "pch.h"
#include "Resources.hpp"

#include "Windows/Rendering/Texture2D.hpp"

using namespace Egliss;

void Resources::Initialize()
{
	// insert core resource
	Rendering::Texture2D::Create("Icon","Resources/app_icon.png");
}

void Resources::Finalize()
{
	this->_keyResource.clear();
	this->_physicalResources.clear();
}

void Resources::Register(const std::string& key, std::shared_ptr<Resource> resource)
{
	this->_keyResource.emplace(key, resource);
}

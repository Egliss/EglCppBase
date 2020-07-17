#include "pch.h"
#include "Resources.hpp"

using namespace Egliss;

void Resources::Initialize()
{
	// insert core resource
}

void Resources::Finalize()
{
	this->_physicalResources.clear();
}

void Resources::Register(const std::string& key, const std::string& path)
{
	this->_physicalResources.emplace(key, path);
}

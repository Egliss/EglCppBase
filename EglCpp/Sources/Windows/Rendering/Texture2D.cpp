#include "pch.h"
#include <d3d12.h>
#include <Win.hpp>
#include <WICTextureLoader.h>

#include "Texture2D.hpp"
#include "DirectXManager.hpp"
#include "../../Utility/StringUtility.hpp"

using namespace Egliss;
using namespace Egliss::Rendering;

std::shared_ptr<Texture2D> Texture2D::Create(const std::string& key, const std::string& path)
{
	auto texture = std::shared_ptr<Texture2D>(new Texture2D());
	texture->_key = key;
	texture->_path = path;
	texture->Initialize(texture);
	return texture;
}

bool Texture2D::InternalInitialize()
{
	auto result = DirectX::LoadWICTextureFromFile(
		DirectXManager::GetInstance().Device(),
		StringUtility::ToWString(this->_path.data()).data(),
		this->_texture.ReleaseAndGetAddressOf(),
		this->_data,
		this->_subresourceDesc
	);
	if (FAILED(result)) 
	{
		throw std::exception(("texture initialize failed --> " + this->_path).data());
	}

	return true;
}

Texture2D::Texture2D()
{
}

Texture2D::~Texture2D()
{
}

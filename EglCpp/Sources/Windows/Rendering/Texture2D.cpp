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

	auto result = DirectX::LoadWICTextureFromFile(
		DirectXManager::GetInstance().Device(),
		StringUtility::ToWString(path.data()).data(),
		texture->_texture.ReleaseAndGetAddressOf(),
		texture->_data,
		texture->_subresourceDesc
	);

	if (FAILED(result))
	{
		throw std::exception(("texture initialize failed --> " + path).data());
	}

	texture->Register(texture);
	
	return texture;
}

Texture2D::Texture2D(): _subresourceDesc()
{
}

Texture2D::~Texture2D()
{
}

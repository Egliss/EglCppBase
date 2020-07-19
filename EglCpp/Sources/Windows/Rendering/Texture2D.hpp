#pragma once

#include <string>
#include <memory>
#include "DirectXShared.hpp"
#include "../../Math/Vector2.hpp"
#include "../../App/Components/Resources/Resource.hpp"

namespace Egliss::Rendering
{
	class Texture2D : public Resource
	{
	public:
		virtual ~Texture2D();
		static std::shared_ptr<Texture2D> Create(const std::string& key, const std::string& path);

		ComPtr<ID3D12Resource> Texture() const
		{
			return this->_texture;
		}
		Vector2 Resolution() const
		{
			return this->_resolution;
		}
	protected:

		virtual bool InternalInitialize() override;

	private:
		std::string _path;
		Vector2 _resolution;
		std::unique_ptr<unsigned char[]> _data;
		ComPtr<ID3D12Resource> _texture;

		Texture2D();
	};
}
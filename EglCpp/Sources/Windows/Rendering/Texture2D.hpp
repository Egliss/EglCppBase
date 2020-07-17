#pragma once

#include <string>

#include "../../App/Components/Resources/Resource.hpp"

#include "DirectXShared.hpp"
#include "../../Math/Vector2.hpp"

namespace Egliss::Rendering
{
	class Texture2D : public Resource
	{
	public:

		ComPtr<ID3D12Resource> Texture() const
		{
			return this->_texture;
		}
		Vector2 Resolution() const
		{
			return this->_resolution;
		}
	private:
	#ifdef _DEBUG
		std::string _path;
	#endif // _DEBUG

		Vector2 _resolution;
		ComPtr<ID3D12Resource> _texture;

	protected:

		// Resource ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
		virtual bool InternalInitialize(const std::string& path) override;
	};
}
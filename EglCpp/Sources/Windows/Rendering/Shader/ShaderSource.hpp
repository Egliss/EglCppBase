#pragma once

#include "../../../App/Components/Resources/Resource.hpp"

namespace Egliss::Rendering
{
	class Shader;
	class ShaderSource : public Resource
	{
	public:
		virtual bool Initialize(const std::string& key, const std::string& path);
		virtual void Finalize() override;
		std::shared_ptr<Shader> CreateShader();

		const std::string& Path()const
		{
			return this->_path;
		}
		const std::string Source()const
		{
			return this->_shaderSource;
		}
	private:
		std::string _path;
		std::string _shaderSource;
		//TODO Reflection
	};
}
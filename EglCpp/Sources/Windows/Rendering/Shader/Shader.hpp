#pragma once

#include <vector>
#include <string>

#include "../../../App/Components/Resources/Resource.hpp"

struct ID3D11InputLayout;
struct ID3D10Blob;

namespace Egliss::Rendering
{
	class ShaderSource;

	// _Debug then, the 
	class Shader : public Resource
	{
		class _Impl;
	public:
		enum class Type
		{
			Vertex,
			Geomerty,
			Hull,
			Domain,
			Pixel,
			Compute,
		};
		static std::string SignatureOf(Type type);
		static Type TypeOf(const std::string& type);

		virtual bool Initialize(std::shared_ptr<ShaderSource> source, Type type);
		virtual bool Initialize(const std::string& key, const std::string& path);
		virtual void Finalize() override;
		
		ID3D11InputLayout* InputLayout() const;
	private:
		std::unique_ptr<_Impl> _impl;
		void InitializeD3DResource();
	};
}
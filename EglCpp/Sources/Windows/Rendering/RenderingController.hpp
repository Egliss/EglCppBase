#pragma once

#include <memory>

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace DirectX
{
	class CommonStates;
	class IEffectFactory;
	class SpriteBatch;
	class BasicEffect;
}

namespace Egliss::Rendering
{
	class RenderingController
	{
	public:
		void Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
		void Finalize();

	private:
		std::unique_ptr<DirectX::CommonStates> _states;
		std::unique_ptr<DirectX::IEffectFactory> _effectFactory;
		std::unique_ptr<DirectX::BasicEffect> _rootBatch;
	};
}
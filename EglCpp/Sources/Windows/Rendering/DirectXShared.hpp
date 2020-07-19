#pragma once

#include <d3d12.h>
#include <wrl/client.h>

/*
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
*/

namespace Egliss::Rendering
{
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
}

enum class CommandListType
{
	Direct = 0,
	Bundle = 1,
	Compute = 2,
	Copy = 3,
	VideoDecode = 4,
	VIdeoProcess = 5,
	VideoEncode = 6
};

enum class DescriptorHeapType
{
	ConstantBuffer = 0, ShaderResource = 0, UnorderedAccess = 0,
	Sampler = 1,
	RenderTarget = 2,
	DepthStencil = 3,
};
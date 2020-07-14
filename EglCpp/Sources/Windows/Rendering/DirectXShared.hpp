#pragma once

#include <wrl/client.h>

struct IDXGIFactory4;
struct IDXGIAdapter1;
struct IDXGISwapChain1;
struct ID3D12Device6;
struct ID3D12Debug3;
struct ID3D12CommandQueue;
struct ID3D12CommandList;
struct ID3D12DescriptorHeap;
struct ID3D12Resource1;
struct ID3D12CommandAllocator;

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
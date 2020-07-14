#include "pch.h"
#include "DirectXManager.hpp"
#include <Win.hpp>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace Egliss;
using namespace Egliss::Rendering;

std::unique_ptr<DirectXManager> DirectXManager::_instance;

void DirectXManager::Initialize()
{
	auto instance = std::make_unique<DirectXManager>();
	instance->InitializeInternal();
	DirectXManager::_instance = std::move(instance);
}

void DirectXManager::Finalize()
{
}

void DirectXManager::Draw()
{
	if (!DirectXManager::_instance)
		return;
	
	DirectXManager::_instance->Rendering();
}

void DirectXManager::InitializeInternal()
{
	const auto hwnd = Application::GetImplAs<WinImpl>().GetHWND();

	this->_debug = DirectXManager::CreateDebugLayer();

	this->_dxgiFactory = DirectXManager::CreateDXGIFactory();
	this->_dxgiAdapter = DirectXManager::CreateDXGIAdapter(this->_dxgiFactory);
	this->_device = DirectXManager::CreateDevice(this->_dxgiAdapter);
	this->_renderingQueue = DirectXManager::CreateCommandQueue(this->_device, CommandListType::Direct);
	this->_computeQueue = DirectXManager::CreateCommandQueue(this->_device, CommandListType::Compute);
	this->_copyQueue = DirectXManager::CreateCommandQueue(this->_device, CommandListType::Copy);
	this->_swapChain = DirectXManager::CreateSwapChain(this->_renderingQueue, this->_dxgiFactory, hwnd);
	this->_swapChainDescriptorHeap = DirectXManager::CreateDescriptorHeap(this->_device, DescriptorHeapType::RenderTarget, SwapChainBufferCount, false);
	this->_swapChainRTV = DirectXManager::CreateSwapChainRTV(this->_device, this->_swapChain, this->_swapChainDescriptorHeap);
	this->_commandAllocator = DirectXManager::CreateCommandAllocator(this->_device, CommandListType::Direct);
	this->_graphicsCommandList = DirectXManager::CreateGraphicsCommandList(this->_device, this->_commandAllocator, CommandListType::Direct);
	this->_fenceEvenet = DirectXManager::CreateFenceEvent();
	this->_renderingQueueFence = DirectXManager::CreateCommandQueueFence(this->_device, CommandListType::Direct);
}
ComPtr<ID3D12Debug> DirectXManager::CreateDebugLayer()
{
	ComPtr<ID3D12Debug> debug;
#ifdef _DEBUG
	// TODO query in release build ?
	const auto result = D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void**)debug.ReleaseAndGetAddressOf());
	if (FAILED(result))
		throw std::exception("debug layer initialize failed.");

	debug->EnableDebugLayer();
#endif // _DEBUG
	return debug;

}
ComPtr<IDXGIFactory4> DirectXManager::CreateDXGIFactory()
{
	ComPtr<IDXGIFactory4> factory;
	const auto result = CreateDXGIFactory2(0, __uuidof(IDXGIFactory4), (void**)factory.ReleaseAndGetAddressOf());
	if (FAILED(result))
		throw std::exception("create dxgi factory was failed.");

	return factory;
}

DirectXManager::SwapChainRTVArrayT DirectXManager::CreateSwapChainRTV(ComPtr<ID3D12Device6> device, ComPtr<IDXGISwapChain3> swapchain, ComPtr<ID3D12DescriptorHeap> heap)
{
	auto handle = heap->GetCPUDescriptorHandleForHeapStart();
	const auto HeapStrid = DirectXManager::GetHeapByteSize(device, DescriptorHeapType::RenderTarget);
	SwapChainRTVArrayT array;
	for (auto L10 = 0U; L10 < DirectXManager::SwapChainBufferCount; L10++)
	{
		auto result = swapchain->GetBuffer(L10, __uuidof(ID3D12Resource), (void**)array[L10].GetAddressOf());
		if (FAILED(result))
			throw std::exception("query swapchain buffer was failed.");
		device->CreateRenderTargetView(array[L10].Get(), nullptr, handle);
		// step next loop addresss
		handle.ptr += HeapStrid;
	}
	return std::move(array);
}
ComPtr<IDXGIAdapter1> DirectXManager::CreateDXGIAdapter(ComPtr<IDXGIFactory4> factory)
{
	ComPtr<IDXGIAdapter1> adapter;
	for (auto index = 0U; DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(index, adapter.ReleaseAndGetAddressOf()); ++index)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			continue;

		// try constructable device
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_0, __uuidof(ID3D12Device6), nullptr)))
			return adapter;
	}
	throw std::exception("directX12 device adapter not found.");
}

ComPtr<ID3D12Device6> DirectXManager::CreateDevice(ComPtr<IDXGIAdapter1> adapter)
{
	ComPtr<ID3D12Device6> device6;
	ComPtr<ID3D12Device> device;

	const auto result = D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_0, __uuidof(ID3D12Device), (void**)device.ReleaseAndGetAddressOf());
	
	if (FAILED(result))
		throw std::exception("directx 12 adapter found but, device initialize failed");
	if (FAILED(device.As(&device6)))
		throw std::exception("directx 12 adapter found but, device initialize failed");

	return device6;
}

ComPtr<ID3D12CommandQueue> DirectXManager::CreateCommandQueue(ComPtr<ID3D12Device6> device, CommandListType type)
{
	ComPtr<ID3D12CommandQueue> queue;
	D3D12_COMMAND_QUEUE_DESC desc;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.Type = static_cast<D3D12_COMMAND_LIST_TYPE>(type);
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY::D3D12_COMMAND_QUEUE_PRIORITY_HIGH;

	const auto result = device->CreateCommandQueue(&desc, __uuidof(ID3D12CommandQueue), (void**)queue.ReleaseAndGetAddressOf());

	if (FAILED(result))
		throw std::exception("create command queue was failed.");

	return queue;
}

ComPtr<ID3D12Fence> DirectXManager::CreateCommandQueueFence(ComPtr<ID3D12Device6> device, CommandListType type)
{
	ComPtr<ID3D12Fence> fence;

	const auto result = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)fence.ReleaseAndGetAddressOf());

	if (FAILED(result))
		throw std::exception("create fence was failed.");

	return fence;
}

HANDLE DirectXManager::CreateFenceEvent()
{
	return CreateEventA(nullptr, false, false, nullptr);

}

ComPtr<ID3D12CommandList> DirectXManager::CreateCommandList(ComPtr<ID3D12Device6> device, ComPtr<ID3D12CommandAllocator> allocator, CommandListType type)
{
	ComPtr<ID3D12CommandList> commandList;
	const auto result =
		device->CreateCommandList(0,
								  static_cast<D3D12_COMMAND_LIST_TYPE>(type),
								  allocator.Get(),
								  nullptr,
								  __uuidof(ID3D12CommandList),
								  (void**)commandList.ReleaseAndGetAddressOf());
	if (FAILED(result))
		throw std::exception("create command list was failed.");

	return commandList;
}

ComPtr<ID3D12GraphicsCommandList> DirectXManager::CreateGraphicsCommandList(ComPtr<ID3D12Device6> device, ComPtr<ID3D12CommandAllocator> allocator, CommandListType type)
{
	ComPtr<ID3D12GraphicsCommandList> graphicsCommandList;

	const auto result =
		device->CreateCommandList(0,
								  static_cast<D3D12_COMMAND_LIST_TYPE>(type),
								  allocator.Get(),
								  nullptr,
								  __uuidof(ID3D12GraphicsCommandList),
								  (void**)graphicsCommandList.ReleaseAndGetAddressOf());
	if (FAILED(result))
		throw std::exception("create graphics command list was failed.");

	return graphicsCommandList;
}

ComPtr<ID3D12CommandAllocator> DirectXManager::CreateCommandAllocator(ComPtr<ID3D12Device6> device, CommandListType type)
{
	ComPtr<ID3D12CommandAllocator> allocator;
	const auto result = device->CreateCommandAllocator(static_cast<D3D12_COMMAND_LIST_TYPE>(type), __uuidof(ID3D12CommandAllocator), (void**)allocator.ReleaseAndGetAddressOf());

	if (FAILED(result))
		throw std::exception("create command allocator was failed.");

	return allocator;
}

ComPtr<IDXGISwapChain3> DirectXManager::CreateSwapChain(ComPtr<ID3D12CommandQueue> queue, ComPtr<IDXGIFactory4> factory, HWND hwnd)
{
	ComPtr<IDXGISwapChain1> swapChain;
	ComPtr<IDXGISwapChain3> swapChain3;

	RECT rect;
	GetClientRect(hwnd, &rect);
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE::DXGI_ALPHA_MODE_UNSPECIFIED;
	swapChainDesc.Scaling = DXGI_SCALING::DXGI_SCALING_NONE;
	swapChainDesc.BufferCount = SwapChainBufferCount;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	swapChainDesc.Stereo = false;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swapChainDesc.Height = rect.bottom - rect.top;
	swapChainDesc.Width = rect.right - rect.left;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc;
	fullScreenDesc.RefreshRate.Denominator = 60;
	fullScreenDesc.RefreshRate.Numerator = 1;
	fullScreenDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;
	fullScreenDesc.Windowed = true;
	fullScreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	const auto result = factory->CreateSwapChainForHwnd(queue.Get(), hwnd, &swapChainDesc, nullptr, nullptr/* multi monitor */, swapChain.ReleaseAndGetAddressOf());

	if (FAILED(result))
		throw std::exception("create swapchain was failed.");
	if (FAILED(swapChain.As(&swapChain3)))
		throw std::exception("create swapchain was failed.");

	return swapChain3;
}


unsigned int DirectXManager::GetHeapByteSize(ComPtr<ID3D12Device6> device, DescriptorHeapType type)
{
	return device->GetDescriptorHandleIncrementSize(static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(type));
}

ComPtr<ID3D12DescriptorHeap> DirectXManager::CreateDescriptorHeap(ComPtr<ID3D12Device6> device, DescriptorHeapType type, int elementCount, bool accessFromShader)
{
	ComPtr<ID3D12DescriptorHeap> heap;
	D3D12_DESCRIPTOR_HEAP_DESC desc;
	desc.Flags = accessFromShader ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NumDescriptors = elementCount;
	desc.Type = static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(type);
	desc.NodeMask = 0;

	const auto result = device->CreateDescriptorHeap(&desc, __uuidof(ID3D12DescriptorHeap), (void**)heap.ReleaseAndGetAddressOf());

	if (FAILED(result))
		throw std::exception("create descriptor heap was failed.");

	return heap;
}

void SetResourceBarrier(ID3D12GraphicsCommandList* commandList, ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
{
	D3D12_RESOURCE_BARRIER descBarrier;
	ZeroMemory(&descBarrier, sizeof(descBarrier));
	descBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	descBarrier.Transition.pResource = resource;
	descBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	descBarrier.Transition.StateBefore = before;
	descBarrier.Transition.StateAfter = after;
	commandList->ResourceBarrier(1, &descBarrier);
}

void WaitForCommandQueue(ComPtr<ID3D12CommandQueue> queue, ComPtr<ID3D12Fence> fence, HANDLE fenceEvent)
{
	static UINT64 frames = 0;
	fence->SetEventOnCompletion(frames, fenceEvent);
	queue->Signal(fence.Get(), frames);
	WaitForSingleObject(fenceEvent, INFINITE);
	frames++;
}

#include <ctime>
#include <cmath>
#include <chrono>

void DirectXManager::Rendering()
{
	const auto hwnd = Application::GetImplAs<WinImpl>().GetHWND();
	RECT rect;
	GetClientRect(hwnd, &rect);
	const auto now = std::chrono::system_clock::now().time_since_epoch();
	const auto miliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
	const auto seed = std::fmod((double)miliseconds, 100000.0) / 1000.0f;
	const auto r = std::sinf(seed);
	const auto g = std::cosf(seed);
	const auto b = std::tanf(seed);
	const auto a = 1.0f;

	float clearColor[4] = { r,g,b,a };
	D3D12_VIEWPORT viewport;
	viewport.TopLeftX = 0; viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(rect.right - rect.left);
	viewport.Height = static_cast<float>(rect.bottom - rect.top);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	const auto rtvIndex = this->_swapChain->GetCurrentBackBufferIndex();
	auto rtvHandle = this->_swapChainDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += DirectXManager::GetHeapByteSize(this->_device, DescriptorHeapType::RenderTarget) * rtvIndex;

	SetResourceBarrier(this->_graphicsCommandList.Get(), this->_swapChainRTV[rtvIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	this->_graphicsCommandList->RSSetViewports(1, &viewport);
	this->_graphicsCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	SetResourceBarrier(this->_graphicsCommandList.Get(), this->_swapChainRTV[rtvIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	this->_graphicsCommandList->Close();

	ID3D12CommandList* commandList = this->_graphicsCommandList.Get();
	this->_renderingQueue->ExecuteCommandLists(1, &commandList);
	this->_swapChain->Present(1, 0);

	WaitForCommandQueue(this->_renderingQueue, this->_renderingQueueFence, this->_fenceEvenet);

	this->_commandAllocator->Reset();
	this->_graphicsCommandList->Reset(this->_commandAllocator.Get(), nullptr);
}

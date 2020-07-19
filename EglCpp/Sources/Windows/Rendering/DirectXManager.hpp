#pragma once

#include "DirectXShared.hpp"
#include <dxgi1_4.h>
#include <memory>
#include <array>

namespace Egliss::Rendering
{
	class DirectXManager
	{
	public:
		static void Initialize();
		static void Finalize();
		static void Draw();

		static DirectXManager& GetInstance()
		{
			return *_instance.get();
		}
		ID3D12Device6* Device()
		{
			return this->_device.Get();
		}
	private:
		static constexpr unsigned int SwapChainBufferCount = 2;
		using SwapChainRTVArrayT = std::array<ComPtr<ID3D12Resource>, SwapChainBufferCount>;
		static std::unique_ptr<DirectXManager> _instance;

		ComPtr<IDXGIAdapter1> _dxgiAdapter;
		ComPtr<IDXGIFactory4> _dxgiFactory;
		ComPtr<IDXGISwapChain3> _swapChain;
		ComPtr<ID3D12Device6> _device;
		ComPtr<ID3D12Debug> _debug;
		ComPtr<ID3D12CommandQueue> _renderingQueue;
		ComPtr<ID3D12Fence> _renderingQueueFence;
		HANDLE _fenceEvenet;
		ComPtr<ID3D12CommandQueue> _computeQueue;
		ComPtr<ID3D12CommandQueue> _copyQueue;
		ComPtr<ID3D12CommandAllocator> _commandAllocator;
		ComPtr<ID3D12GraphicsCommandList> _graphicsCommandList;
		ComPtr<ID3D12DescriptorHeap> _swapChainDescriptorHeap;
		SwapChainRTVArrayT _swapChainRTV;

		void InitializeInternal();
		static ComPtr<ID3D12Debug> CreateDebugLayer();
		static ComPtr<IDXGIFactory4> CreateDXGIFactory();
		static SwapChainRTVArrayT CreateSwapChainRTV(ComPtr<ID3D12Device6> device, ComPtr<IDXGISwapChain3> swapchain, ComPtr<ID3D12DescriptorHeap> heap);
		static ComPtr<IDXGIAdapter1> CreateDXGIAdapter(ComPtr<IDXGIFactory4> factory);
		static ComPtr<ID3D12Device6> CreateDevice(ComPtr<IDXGIAdapter1> adapter);
		static ComPtr<ID3D12CommandQueue> CreateCommandQueue(ComPtr<ID3D12Device6> device, CommandListType type);
		static ComPtr<ID3D12Fence> CreateCommandQueueFence(ComPtr<ID3D12Device6> device);
		static HANDLE CreateFenceEvent();
		static ComPtr<ID3D12CommandList> CreateCommandList(ComPtr<ID3D12Device6> device, ComPtr<ID3D12CommandAllocator> allocator, CommandListType type);
		static ComPtr<ID3D12GraphicsCommandList> CreateGraphicsCommandList(ComPtr<ID3D12Device6> device, ComPtr<ID3D12CommandAllocator> allocator, CommandListType type);
		static ComPtr<ID3D12CommandAllocator> CreateCommandAllocator(ComPtr<ID3D12Device6> device, CommandListType type);
		static ComPtr<IDXGISwapChain3> CreateSwapChain(ComPtr<ID3D12CommandQueue> queue, ComPtr<IDXGIFactory4> factory, HWND hwnd);
		static ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(ComPtr<ID3D12Device6> device, DescriptorHeapType type, int elementCount, bool accessFromShader);

		void Rendering();

		static unsigned int GetHeapByteSize(ComPtr<ID3D12Device6> device, DescriptorHeapType type);

	};
}
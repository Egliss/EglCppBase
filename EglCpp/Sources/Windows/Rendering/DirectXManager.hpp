#pragma once

#include "DirectXShared.hpp"
#include <memory>
#include <array>

namespace Egliss::Rendering
{
	class DirectXManager
	{
	public:
		static void Initialize();
		static void Finalize();

	private:
		static std::unique_ptr<DirectXManager> _instance;

		ComPtr<IDXGIAdapter1> _dxgiAdapter;
		ComPtr<IDXGIFactory4> _dxgiFactory;
		ComPtr<IDXGISwapChain1> _swapChain;
		ComPtr<ID3D12Device6> _device;
		ComPtr<ID3D12Debug3> _debug;
		ComPtr<ID3D12CommandQueue> _renderingQueue;
		ComPtr<ID3D12CommandQueue> _computeQueue;
		ComPtr<ID3D12CommandQueue> _copyQueue;

		void InitializeInternal();
		static ComPtr<ID3D12Debug3> CreateDebugLayer();
		static ComPtr<IDXGIFactory4> CreateDXGIFactory();
		static ComPtr<IDXGIAdapter1> CreateDXGIAdapter(ComPtr<IDXGIFactory4> factory);
		static ComPtr<ID3D12Device6> CreateDevice(ComPtr<IDXGIAdapter1> adapter);
		static ComPtr<ID3D12CommandQueue> CreateCommandQueue(ComPtr<ID3D12Device6> device, CommandListType type);
		static ComPtr<IDXGISwapChain1> CreateSwapChain(ComPtr<ID3D12CommandQueue> queue, ComPtr<IDXGIFactory4> factory, HWND hwnd);
		static ComPtr<ID3D12DescriptorHeap> CreateDescriptor(ComPtr<ID3D12Device6> device, DescriptorHeapType type, int elementCount, bool accessFromShader);
		static unsigned int GetHeapByteSize(ComPtr<ID3D12Device6> device,DescriptorHeapType type);

	};
}
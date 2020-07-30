#pragma once

#include "DirectXShared.hpp"
#include <dxgi1_4.h>
#include <memory>
#include <array>
#include <wrl/wrappers/corewrappers.h>

namespace Egliss::Rendering
{
	class DirectXManager
	{
	public:
		static void Initialize();
		static void Finalize();
		void Present();
		void ClearScreen();
		void Render();
				
		static DirectXManager& GetInstance()
		{
			return *_instance.get();
		}
		ID3D12Device* Device()
		{
			return this->device.Get();
		}

	private:
		static std::unique_ptr<DirectXManager> _instance;

		// Direct3D Objects
		static constexpr unsigned int SwapBufferCount = 2;
		unsigned int                                        _backBufferIndex;
		unsigned int                                        _rtvDescriptorSize;
		Microsoft::WRL::ComPtr<ID3D12Device>                device;
		Microsoft::WRL::ComPtr<IDXGIFactory4>               _dxgiFactory;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue>          _commandQueue;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        _rtvDescriptorHeap;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        m_dsvDescriptorHeap;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator>      m_commandAllocators[SwapBufferCount];
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>   m_commandList;
		Microsoft::WRL::ComPtr<ID3D12Fence>                 _fence;
		unsigned long long                                  _fenceValues[SwapBufferCount];
		Microsoft::WRL::Wrappers::Event                     _fenceEvent;

		// Rendering resources
		Microsoft::WRL::ComPtr<IDXGISwapChain3>             _swapChain;
		Microsoft::WRL::ComPtr<ID3D12Resource>              _renderTargets[SwapBufferCount];
		Microsoft::WRL::ComPtr<ID3D12Resource>              dsv;

		void InitializeInternal();
		void MoveToNextFrame();
		void CreateDevice();
		void CreateResources();
		void WaitForGpu();
		ComPtr<IDXGIAdapter1> GetAdapter();

		void OnActivated();
		void OnDeactivated();
		void OnSuspending();
		void OnResuming();
		void OnWindowSizeChanged(float width, float height);
		void OnDeviceLosted();
	};
}
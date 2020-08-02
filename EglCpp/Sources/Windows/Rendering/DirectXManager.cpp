#include "pch.h"
#include "DirectXManager.hpp"
#include <Win.hpp>

#include <d3d12.h>
#include <dxgidebug.h>
#include <DirectXColors.h>

#include <fmt/format.h>

#include "../DirectXUtility.hpp"
#include "../../App/Components/Random.hpp"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace Egliss;
using namespace Egliss::Rendering;

std::unique_ptr<DirectXManager> DirectXManager::_instance;

const D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_0;

void DirectXManager::Initialize()
{
	auto instance = std::make_unique<DirectXManager>();
	instance->InitializeInternal();
	DirectXManager::_instance = std::move(instance);
}

void DirectXManager::InitializeInternal()
{
    CreateDevice();
    CreateResources();
}

void DirectXManager::Finalize()
{
    _instance->WaitForGpu();
}

// Draws the scene.
void DirectXManager::Render()
{
    ClearScreen();

    // TODO: Add your rendering code here.

    // Show the new frame.
    Present();
}

// Helper method to prepare the command list for rendering and clear the back buffers.
void DirectXManager::ClearScreen()
{
    // Reset command list and allocator.
    ThrowIfFailed(m_commandAllocators[_backBufferIndex]->Reset());
    ThrowIfFailed(m_commandList->Reset(m_commandAllocators[_backBufferIndex].Get(), nullptr));

    // Transition the render target into the correct state to allow for drawing into it.
    D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(_renderTargets[_backBufferIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    m_commandList->ResourceBarrier(1, &barrier);

    // Clear the views.
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvDescriptor(
        _rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
        static_cast<INT>(_backBufferIndex), _rtvDescriptorSize);
    CD3DX12_CPU_DESCRIPTOR_HANDLE dsvDescriptor(m_dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
    m_commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);
    m_commandList->ClearRenderTargetView(rtvDescriptor, DirectX::Colors::CornflowerBlue, 0, nullptr);
    m_commandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    const auto size = Application::GetImplAs<WindowsApplicationImpl>().WindowSize();
    // Set the viewport and scissor rect.
    D3D12_VIEWPORT viewport = { 0.0f, 0.0f, size.x,size.y, D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
    D3D12_RECT scissorRect = { 0, 0, static_cast<LONG>(size.x), static_cast<LONG>(size.y) };
    m_commandList->RSSetViewports(1, &viewport);
    m_commandList->RSSetScissorRects(1, &scissorRect);
}

// Submits the command list to the GPU and presents the back buffer contents to the screen.
void DirectXManager::Present()
{
    // Transition the render target to the state that allows it to be presented to the display.
    D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(_renderTargets[_backBufferIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    m_commandList->ResourceBarrier(1, &barrier);

    // Send the command list off to the GPU for processing.
    ThrowIfFailed(m_commandList->Close());
    _commandQueue->ExecuteCommandLists(1, CommandListCast(m_commandList.GetAddressOf()));

    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    HRESULT hr = _swapChain->Present(1, 0);

    // If the device was reset we must completely reinitialize the renderer.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        OnDeviceLosted();
    }
    else
    {
        ThrowIfFailed(hr);

        MoveToNextFrame();
    }
}

// Message handlers
void DirectXManager::OnActivated()
{
    // TODO: Game is becoming active window.
}

void DirectXManager::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void DirectXManager::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void DirectXManager::OnResuming()
{
    // TODO: Game is being power-resumed (or returning from minimize).
}

void DirectXManager::OnWindowSizeChanged(float width, float height)
{
    CreateResources();
}

// These are the resources that depend on the device.
void DirectXManager::CreateDevice()
{
    DWORD dxgiFactoryFlags = 0;

#if defined(_DEBUG)
    // Enable the debug layer (requires the Graphics Tools "optional feature").
    //
    // NOTE: Enabling the debug layer after device creation will invalidate the active device.
    {
        ComPtr<ID3D12Debug> debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.GetAddressOf()))))
        {
            debugController->EnableDebugLayer();
        }

        ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
        if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(dxgiInfoQueue.GetAddressOf()))))
        {
            dxgiFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;

            dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
            dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
        }
    }
    
#endif

    ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(_dxgiFactory.ReleaseAndGetAddressOf())));

    ComPtr<IDXGIAdapter1> adapter = GetAdapter();

    // Create the DX12 API device object.
    ThrowIfFailed(D3D12CreateDevice(
        adapter.Get(),
        FeatureLevel,
        IID_PPV_ARGS(device.ReleaseAndGetAddressOf())
    ));

    // Configure debug device (if active).
    ComPtr<ID3D12InfoQueue> d3dInfoQueue;
    if (SUCCEEDED(device.As(&d3dInfoQueue)))
    {
    #ifdef _DEBUG
        d3dInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
        d3dInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
    #endif
        D3D12_MESSAGE_ID hide[] =
        {
            D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,
            D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE
        };
        D3D12_INFO_QUEUE_FILTER filter = {};
        filter.DenyList.NumIDs = _countof(hide);
        filter.DenyList.pIDList = hide;
        d3dInfoQueue->AddStorageFilterEntries(&filter);
    }

    // Create the command queue.
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    ThrowIfFailed(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(_commandQueue.ReleaseAndGetAddressOf())));

    // Create descriptor heaps for render target views and depth stencil views.
    D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc = {};
    rtvDescriptorHeapDesc.NumDescriptors = SwapBufferCount;
    rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

    D3D12_DESCRIPTOR_HEAP_DESC dsvDescriptorHeapDesc = {};
    dsvDescriptorHeapDesc.NumDescriptors = 1;
    dsvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

    ThrowIfFailed(device->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(_rtvDescriptorHeap.ReleaseAndGetAddressOf())));
    ThrowIfFailed(device->CreateDescriptorHeap(&dsvDescriptorHeapDesc, IID_PPV_ARGS(m_dsvDescriptorHeap.ReleaseAndGetAddressOf())));

    _rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    // Create a command allocator for each back buffer that will be rendered to.
    for (UINT n = 0; n < SwapBufferCount; n++)
    {
        ThrowIfFailed(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_commandAllocators[n].ReleaseAndGetAddressOf())));
    }

    // Create a command list for recording graphics commands.
    ThrowIfFailed(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocators[0].Get(), nullptr, IID_PPV_ARGS(m_commandList.ReleaseAndGetAddressOf())));
    ThrowIfFailed(m_commandList->Close());

    // Create a fence for tracking GPU execution progress.
    ThrowIfFailed(device->CreateFence(_fenceValues[_backBufferIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(_fence.ReleaseAndGetAddressOf())));
    _fenceValues[_backBufferIndex]++;

    _fenceEvent.Attach(CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE));
    if (!_fenceEvent.IsValid())
    {
        throw std::exception("CreateEvent");
    }

    // TODO: Initialize device dependent objects here (independent of window size).
}

// Allocate all memory resources that change on a window SizeChanged event.
void DirectXManager::CreateResources()
{
    // Wait until all previous GPU work is complete.
    WaitForGpu();

    // Release resources that are tied to the swap chain and update fence values.
    for (auto index = 0U; index < SwapBufferCount; index++)
    {
        _renderTargets[index].Reset();
        _fenceValues[index] = _fenceValues[_backBufferIndex];
    }
    const auto size = Application::GetImplAs<WindowsApplicationImpl>().WindowSize();
    const DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
    const DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D32_FLOAT;
    const unsigned int backBufferWidth = static_cast<unsigned int>(size.x);
    const unsigned int backBufferHeight = static_cast<unsigned int>(size.y);

    // If the swap chain already exists, resize it, otherwise create one.
    if (_swapChain)
    {
        HRESULT hr = _swapChain->ResizeBuffers(SwapBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
            OnDeviceLosted();
            return;
        }
        else
        {
            ThrowIfFailed(hr);
        }
    }
    else
    {
        // Create a descriptor for the swap chain.
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.Width = backBufferWidth;
        swapChainDesc.Height = backBufferHeight;
        swapChainDesc.Format = backBufferFormat;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = SwapBufferCount;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
        fsSwapChainDesc.Windowed = TRUE;
        auto hwnd = Application::GetImplAs<WindowsApplicationImpl>().GetHWND();
        // Create a swap chain for the window.
        ComPtr<IDXGISwapChain1> swapChain;
        ThrowIfFailed(_dxgiFactory->CreateSwapChainForHwnd(
            _commandQueue.Get(),
            hwnd,
            &swapChainDesc,
            &fsSwapChainDesc,
            nullptr,
            swapChain.GetAddressOf()
        ));

        ThrowIfFailed(swapChain.As(&_swapChain));

        // This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut
        ThrowIfFailed(_dxgiFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER));
    }

    // Obtain the back buffers for this window which will be the final render targets
    // and create render target views for each of them.
    for (UINT index = 0; index < SwapBufferCount; index++)
    {
        ThrowIfFailed(_swapChain->GetBuffer(index, IID_PPV_ARGS(_renderTargets[index].GetAddressOf())));

        auto name = fmt::format(L"Swapchain RTV[%d]", index);
        _renderTargets[index]->SetName(name.data());

        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvDescriptor(
            _rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
            static_cast<INT>(index), _rtvDescriptorSize);
        device->CreateRenderTargetView(_renderTargets[index].Get(), nullptr, rtvDescriptor);
    }

    // Reset the index to the current back buffer.
    _backBufferIndex = _swapChain->GetCurrentBackBufferIndex();

    // Allocate a 2-D surface as the depth/stencil buffer and create a depth/stencil view
    // on this surface.
    CD3DX12_HEAP_PROPERTIES depthHeapProperties(D3D12_HEAP_TYPE_DEFAULT);

    D3D12_RESOURCE_DESC depthStencilDesc = CD3DX12_RESOURCE_DESC::Tex2D(
        depthBufferFormat,
        backBufferWidth,
        backBufferHeight,
        1, // This depth stencil view has only one texture.
        1  // Use a single mipmap level.
    );
    depthStencilDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
    depthOptimizedClearValue.Format = depthBufferFormat;
    depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
    depthOptimizedClearValue.DepthStencil.Stencil = 0;

    ThrowIfFailed(device->CreateCommittedResource(
        &depthHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &depthStencilDesc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &depthOptimizedClearValue,
        IID_PPV_ARGS(dsv.ReleaseAndGetAddressOf())
    ));

    dsv->SetName(L"Swapchain DSV");

    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = depthBufferFormat;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

    device->CreateDepthStencilView(dsv.Get(), &dsvDesc, m_dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

    // TODO: Initialize windows-size dependent objects here.
}

void DirectXManager::WaitForGpu()
{
    if (_commandQueue && _fence && _fenceEvent.IsValid())
    {
        // Schedule a Signal command in the GPU queue.
        UINT64 fenceValue = _fenceValues[_backBufferIndex];
        if (SUCCEEDED(_commandQueue->Signal(_fence.Get(), fenceValue)))
        {
            // Wait until the Signal has been processed.
            if (SUCCEEDED(_fence->SetEventOnCompletion(fenceValue, _fenceEvent.Get())))
            {
                WaitForSingleObjectEx(_fenceEvent.Get(), INFINITE, FALSE);

                // Increment the fence value for the current frame.
                _fenceValues[_backBufferIndex]++;
            }
        }
    }
}

void DirectXManager::MoveToNextFrame()
{
    // Schedule a Signal command in the queue.
    const UINT64 currentFenceValue = _fenceValues[_backBufferIndex];
    ThrowIfFailed(_commandQueue->Signal(_fence.Get(), currentFenceValue));

    // Update the back buffer index.
    _backBufferIndex = _swapChain->GetCurrentBackBufferIndex();

    // If the next frame is not ready to be rendered yet, wait until it is ready.
    if (_fence->GetCompletedValue() < _fenceValues[_backBufferIndex])
    {
        ThrowIfFailed(_fence->SetEventOnCompletion(_fenceValues[_backBufferIndex], _fenceEvent.Get()));
        WaitForSingleObjectEx(_fenceEvent.Get(), INFINITE, FALSE);
    }

    // Set the fence value for the next frame.
    _fenceValues[_backBufferIndex] = currentFenceValue + 1;
}

// This method acquires the first available hardware adapter that supports Direct3D 12.
// If no such adapter can be found, try WARP. Otherwise throw an exception.
ComPtr<IDXGIAdapter1> DirectXManager::GetAdapter()
{
    ComPtr<IDXGIAdapter1> adapter;
    for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != _dxgiFactory->EnumAdapters1(adapterIndex, adapter.ReleaseAndGetAddressOf()); adapterIndex++)
    {
        DXGI_ADAPTER_DESC1 desc;
        ThrowIfFailed(adapter->GetDesc1(&desc));

        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            continue;

        // Check to see if the adapter supports Direct3D 12, but don't create the actual device yet.
        if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), FeatureLevel, __uuidof(ID3D12Device), nullptr)))
            break;
    }

#if _DEBUG
    if (!adapter)
    {
        if (FAILED(_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(adapter.ReleaseAndGetAddressOf()))))
            throw std::exception("WARP12 not available. Enable the 'Graphics Tools' optional feature");
    }
#endif

    if (!adapter)
        throw std::exception("No Direct3D 12 device found");

    return adapter;
}

void DirectXManager::OnDeviceLosted()
{
    for (UINT index = 0; index < SwapBufferCount; index++)
    {
        m_commandAllocators[index].Reset();
        _renderTargets[index].Reset();
    }

    dsv.Reset();
    _fence.Reset();
    m_commandList.Reset();
    _swapChain.Reset();
    _rtvDescriptorHeap.Reset();
    m_dsvDescriptorHeap.Reset();
    _commandQueue.Reset();
    device.Reset();
    _dxgiFactory.Reset();

    CreateDevice();
    CreateResources();
}

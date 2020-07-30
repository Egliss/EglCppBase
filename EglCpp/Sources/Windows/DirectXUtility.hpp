#pragma once

#include <d3d12.h>
#include <exception>

namespace Egliss
{
	static inline void ThrowIfFailed(HRESULT result)
	{
		if (FAILED(result))
			throw std::exception("create command list was failed.");
	}
	template <class CommandListT>
	static inline const ID3D12CommandList** CommandListCast(const CommandListT** ptr) noexcept
	{
		return reinterpret_cast<const ID3D12CommandList**>(ptr);
	}
	template <class CommandListT>
	static inline ID3D12CommandList** CommandListCast(CommandListT** ptr) noexcept
	{
		return reinterpret_cast<ID3D12CommandList**>(ptr);
	}
    inline constexpr UINT D3D12CalcSubresource(UINT MipSlice, UINT ArraySlice, UINT PlaneSlice, UINT MipLevels, UINT ArraySize) noexcept
    {
        return MipSlice + ArraySlice * MipLevels + PlaneSlice * MipLevels * ArraySize;
    }
    inline UINT8 D3D12GetFormatPlaneCount(
        _In_ ID3D12Device* pDevice,
        DXGI_FORMAT Format
    ) noexcept
    {
        D3D12_FEATURE_DATA_FORMAT_INFO formatInfo = { Format, 0 };
        if (FAILED(pDevice->CheckFeatureSupport(D3D12_FEATURE_FORMAT_INFO, &formatInfo, sizeof(formatInfo))))
        {
            return 0;
        }
        return formatInfo.PlaneCount;
    }

    struct CD3DX12_DEFAULT {};

    struct CD3DX12_RESOURCE_DESC : public D3D12_RESOURCE_DESC
    {
        CD3DX12_RESOURCE_DESC() = default;
        explicit CD3DX12_RESOURCE_DESC(const D3D12_RESOURCE_DESC& o) noexcept :
            D3D12_RESOURCE_DESC(o)
        {
        }
        CD3DX12_RESOURCE_DESC(D3D12_RESOURCE_DIMENSION dimension, UINT64 alignment, UINT64 width, UINT height, UINT16 depthOrArraySize,
                              UINT16 mipLevels, DXGI_FORMAT format, UINT sampleCount, UINT sampleQuality, D3D12_TEXTURE_LAYOUT layout, D3D12_RESOURCE_FLAGS flags) noexcept
        {
            Dimension = dimension;
            Alignment = alignment;
            Width = width;
            Height = height;
            DepthOrArraySize = depthOrArraySize;
            MipLevels = mipLevels;
            Format = format;
            SampleDesc.Count = sampleCount;
            SampleDesc.Quality = sampleQuality;
            Layout = layout;
            Flags = flags;
        }
        static inline CD3DX12_RESOURCE_DESC Buffer(const D3D12_RESOURCE_ALLOCATION_INFO& resAllocInfo, D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE) noexcept
        {
            return CD3DX12_RESOURCE_DESC(D3D12_RESOURCE_DIMENSION_BUFFER, resAllocInfo.Alignment, resAllocInfo.SizeInBytes,
                                         1, 1, 1, DXGI_FORMAT_UNKNOWN, 1, 0, D3D12_TEXTURE_LAYOUT_ROW_MAJOR, flags);
        }
        static inline CD3DX12_RESOURCE_DESC Buffer(UINT64 width, D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE, UINT64 alignment = 0) noexcept
        {
            return CD3DX12_RESOURCE_DESC(D3D12_RESOURCE_DIMENSION_BUFFER, alignment, width, 1, 1, 1,
                                         DXGI_FORMAT_UNKNOWN, 1, 0, D3D12_TEXTURE_LAYOUT_ROW_MAJOR, flags);
        }
        static inline CD3DX12_RESOURCE_DESC Tex1D(DXGI_FORMAT format, UINT64 width, UINT16 arraySize = 1, UINT16 mipLevels = 0,
                                                  D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE, D3D12_TEXTURE_LAYOUT layout = D3D12_TEXTURE_LAYOUT_UNKNOWN, UINT64 alignment = 0) noexcept
        {
            return CD3DX12_RESOURCE_DESC(D3D12_RESOURCE_DIMENSION_TEXTURE1D, alignment, width, 1, arraySize, mipLevels, format, 1, 0, layout, flags);
        }
        static inline CD3DX12_RESOURCE_DESC Tex2D(DXGI_FORMAT format, UINT64 width, UINT height, UINT16 arraySize = 1, UINT16 mipLevels = 0, UINT sampleCount = 1,
                                                  UINT sampleQuality = 0, D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE, D3D12_TEXTURE_LAYOUT layout = D3D12_TEXTURE_LAYOUT_UNKNOWN, UINT64 alignment = 0) noexcept
        {
            return CD3DX12_RESOURCE_DESC(D3D12_RESOURCE_DIMENSION_TEXTURE2D, alignment, width, height, arraySize,
                                         mipLevels, format, sampleCount, sampleQuality, layout, flags);
        }
        static inline CD3DX12_RESOURCE_DESC Tex3D(DXGI_FORMAT format, UINT64 width, UINT height, UINT16 depth, UINT16 mipLevels = 0,
                                                  D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE, D3D12_TEXTURE_LAYOUT layout = D3D12_TEXTURE_LAYOUT_UNKNOWN, UINT64 alignment = 0) noexcept
        {
            return CD3DX12_RESOURCE_DESC(D3D12_RESOURCE_DIMENSION_TEXTURE3D, alignment, width, height, depth,
                                         mipLevels, format, 1, 0, layout, flags);
        }
        inline UINT16 Depth() const noexcept
        {
            return (Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE3D ? DepthOrArraySize : 1);
        }
        inline UINT16 ArraySize() const noexcept
        {
            return (Dimension != D3D12_RESOURCE_DIMENSION_TEXTURE3D ? DepthOrArraySize : 1);
        }
        inline UINT8 PlaneCount(_In_ ID3D12Device* pDevice) const noexcept
        {
            return D3D12GetFormatPlaneCount(pDevice, Format);
        }
        inline UINT Subresources(_In_ ID3D12Device* pDevice) const noexcept
        {
            return MipLevels * ArraySize() * PlaneCount(pDevice);
        }
        inline UINT CalcSubresource(UINT MipSlice, UINT ArraySlice, UINT PlaneSlice) noexcept
        {
            return D3D12CalcSubresource(MipSlice, ArraySlice, PlaneSlice, MipLevels, ArraySize());
        }
    };

    struct CD3DX12_RESOURCE_BARRIER : public D3D12_RESOURCE_BARRIER
    {
        CD3DX12_RESOURCE_BARRIER() = default;
        explicit CD3DX12_RESOURCE_BARRIER(const D3D12_RESOURCE_BARRIER& o) noexcept :
            D3D12_RESOURCE_BARRIER(o)
        {
        }
        static inline CD3DX12_RESOURCE_BARRIER Transition(
            _In_ ID3D12Resource* pResource,
            D3D12_RESOURCE_STATES stateBefore,
            D3D12_RESOURCE_STATES stateAfter,
            UINT subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
            D3D12_RESOURCE_BARRIER_FLAGS flags = D3D12_RESOURCE_BARRIER_FLAG_NONE) noexcept
        {
            CD3DX12_RESOURCE_BARRIER result = {};
            D3D12_RESOURCE_BARRIER& barrier = result;
            result.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            result.Flags = flags;
            barrier.Transition.pResource = pResource;
            barrier.Transition.StateBefore = stateBefore;
            barrier.Transition.StateAfter = stateAfter;
            barrier.Transition.Subresource = subresource;
            return result;
        }
        static inline CD3DX12_RESOURCE_BARRIER Aliasing(
            _In_ ID3D12Resource* pResourceBefore,
            _In_ ID3D12Resource* pResourceAfter) noexcept
        {
            CD3DX12_RESOURCE_BARRIER result = {};
            D3D12_RESOURCE_BARRIER& barrier = result;
            result.Type = D3D12_RESOURCE_BARRIER_TYPE_ALIASING;
            barrier.Aliasing.pResourceBefore = pResourceBefore;
            barrier.Aliasing.pResourceAfter = pResourceAfter;
            return result;
        }
        static inline CD3DX12_RESOURCE_BARRIER UAV(
            _In_ ID3D12Resource* pResource) noexcept
        {
            CD3DX12_RESOURCE_BARRIER result = {};
            D3D12_RESOURCE_BARRIER& barrier = result;
            result.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
            barrier.UAV.pResource = pResource;
            return result;
        }
    };

    struct CD3DX12_CPU_DESCRIPTOR_HANDLE : public D3D12_CPU_DESCRIPTOR_HANDLE
    {
        CD3DX12_CPU_DESCRIPTOR_HANDLE() = default;
        explicit CD3DX12_CPU_DESCRIPTOR_HANDLE(const D3D12_CPU_DESCRIPTOR_HANDLE& o) noexcept :
            D3D12_CPU_DESCRIPTOR_HANDLE(o)
        {
        }
        CD3DX12_CPU_DESCRIPTOR_HANDLE(CD3DX12_DEFAULT) noexcept { ptr = 0; }
        CD3DX12_CPU_DESCRIPTOR_HANDLE(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE& other, INT offsetScaledByIncrementSize) noexcept
        {
            InitOffsetted(other, offsetScaledByIncrementSize);
        }
        CD3DX12_CPU_DESCRIPTOR_HANDLE(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE& other, INT offsetInDescriptors, UINT descriptorIncrementSize) noexcept
        {
            InitOffsetted(other, offsetInDescriptors, descriptorIncrementSize);
        }
        CD3DX12_CPU_DESCRIPTOR_HANDLE& Offset(INT offsetInDescriptors, UINT descriptorIncrementSize) noexcept
        {
            ptr = SIZE_T(INT64(ptr) + INT64(offsetInDescriptors) * INT64(descriptorIncrementSize));
            return *this;
        }
        CD3DX12_CPU_DESCRIPTOR_HANDLE& Offset(INT offsetScaledByIncrementSize) noexcept
        {
            ptr = SIZE_T(INT64(ptr) + INT64(offsetScaledByIncrementSize));
            return *this;
        }
        bool operator==(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE& other) const noexcept
        {
            return (ptr == other.ptr);
        }
        bool operator!=(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE& other) const noexcept
        {
            return (ptr != other.ptr);
        }
        CD3DX12_CPU_DESCRIPTOR_HANDLE& operator=(const D3D12_CPU_DESCRIPTOR_HANDLE& other) noexcept
        {
            ptr = other.ptr;
            return *this;
        }

        inline void InitOffsetted(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE& base, INT offsetScaledByIncrementSize) noexcept
        {
            InitOffsetted(*this, base, offsetScaledByIncrementSize);
        }

        inline void InitOffsetted(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE& base, INT offsetInDescriptors, UINT descriptorIncrementSize) noexcept
        {
            InitOffsetted(*this, base, offsetInDescriptors, descriptorIncrementSize);
        }

        static inline void InitOffsetted(_Out_ D3D12_CPU_DESCRIPTOR_HANDLE& handle, _In_ const D3D12_CPU_DESCRIPTOR_HANDLE& base, INT offsetScaledByIncrementSize) noexcept
        {
            handle.ptr = SIZE_T(INT64(base.ptr) + INT64(offsetScaledByIncrementSize));
        }

        static inline void InitOffsetted(_Out_ D3D12_CPU_DESCRIPTOR_HANDLE& handle, _In_ const D3D12_CPU_DESCRIPTOR_HANDLE& base, INT offsetInDescriptors, UINT descriptorIncrementSize) noexcept
        {
            handle.ptr = SIZE_T(INT64(base.ptr) + INT64(offsetInDescriptors) * INT64(descriptorIncrementSize));
        }
    };

    struct CD3DX12_HEAP_PROPERTIES : public D3D12_HEAP_PROPERTIES
    {
        CD3DX12_HEAP_PROPERTIES() = default;
        explicit CD3DX12_HEAP_PROPERTIES(const D3D12_HEAP_PROPERTIES& o) noexcept :
            D3D12_HEAP_PROPERTIES(o)
        {
        }
        CD3DX12_HEAP_PROPERTIES(
            D3D12_CPU_PAGE_PROPERTY cpuPageProperty,
            D3D12_MEMORY_POOL memoryPoolPreference,
            UINT creationNodeMask = 1,
            UINT nodeMask = 1) noexcept
        {
            Type = D3D12_HEAP_TYPE_CUSTOM;
            CPUPageProperty = cpuPageProperty;
            MemoryPoolPreference = memoryPoolPreference;
            CreationNodeMask = creationNodeMask;
            VisibleNodeMask = nodeMask;
        }
        explicit CD3DX12_HEAP_PROPERTIES(
            D3D12_HEAP_TYPE type,
            UINT creationNodeMask = 1,
            UINT nodeMask = 1) noexcept
        {
            Type = type;
            CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
            CreationNodeMask = creationNodeMask;
            VisibleNodeMask = nodeMask;
        }
        bool IsCPUAccessible() const noexcept
        {
            return Type == D3D12_HEAP_TYPE_UPLOAD || Type == D3D12_HEAP_TYPE_READBACK || (Type == D3D12_HEAP_TYPE_CUSTOM &&
                                                                                          (CPUPageProperty == D3D12_CPU_PAGE_PROPERTY_WRITE_COMBINE || CPUPageProperty == D3D12_CPU_PAGE_PROPERTY_WRITE_BACK));
        }
    };
}
#include "pch.h"
#include "Shader.hpp"
#include "ShaderSource.hpp"
#include "ShaderIncludeImpl.hpp"
#include "../DirectXManager.hpp"
#include "../../DirectXUtility.hpp"

#include <SpriteFont.h>
#include <filesystem>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace Egliss;
using namespace Egliss::Rendering;

struct CD3D11_INPUT_ELEMENT_DESC : public D3D11_INPUT_ELEMENT_DESC
{
    CD3D11_INPUT_ELEMENT_DESC(LPCSTR SemanticName, UINT SemanticIndex, DXGI_FORMAT Format, UINT InputSlot, UINT AlignedByteOffset, D3D11_INPUT_CLASSIFICATION InputSlotClass, UINT InstanceDataStepRate)
    {
        this->SemanticName = SemanticName;
        this->SemanticIndex = SemanticIndex;
        this->Format = Format;
        this->InputSlot = InputSlot;
        this->AlignedByteOffset = AlignedByteOffset;
        this->InputSlotClass = InputSlotClass;
        this->InstanceDataStepRate = InstanceDataStepRate;
    }
};

namespace Egliss::Rendering
{
    class Shader::_Impl
    {
    public:
        Type type;
        std::string path;
        std::weak_ptr<ShaderSource> dependencySource;
        // Memo: ID3DBlob has no virtual destructor ?
        std::vector<unsigned char> blob;
        ComPtr<ID3D11InputLayout> layout;
        static ShaderIncludeImpl includeImpl;
    };
}

// TODO move
ShaderIncludeImpl Shader::_Impl::includeImpl = ShaderIncludeImpl();

DXGI_FORMAT GetDxgiFormat(D3D10_REGISTER_COMPONENT_TYPE type, BYTE mask)
{
    if (mask & 0x0F)
    {
        // xyzw
        switch (type)
        {
        case D3D10_REGISTER_COMPONENT_FLOAT32:
            return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case D3D10_REGISTER_COMPONENT_SINT32:
            return DXGI_FORMAT_R32G32B32A32_SINT;
        case D3D10_REGISTER_COMPONENT_UINT32:
            return DXGI_FORMAT_R32G32B32A32_UINT;
        }
    }

    if (mask & 0x07)
    {
        // xyz
        switch (type)
        {
        case D3D10_REGISTER_COMPONENT_FLOAT32:
            return DXGI_FORMAT_R32G32B32_FLOAT;
        case D3D10_REGISTER_COMPONENT_SINT32:
            return DXGI_FORMAT_R32G32B32_SINT;
        case D3D10_REGISTER_COMPONENT_UINT32:
            return DXGI_FORMAT_R32G32B32_UINT;
        }
    }

    if (mask & 0x3)
    {
        // xy
        switch (type)
        {
        case D3D10_REGISTER_COMPONENT_FLOAT32:
            return DXGI_FORMAT_R32G32_FLOAT;
        case D3D10_REGISTER_COMPONENT_SINT32:
            return DXGI_FORMAT_R32G32_SINT;
        case D3D10_REGISTER_COMPONENT_UINT32:
            return DXGI_FORMAT_R32G32_UINT;
        }
    }

    if (mask & 0x1)
    {
        // x
        switch (type)
        {
        case D3D10_REGISTER_COMPONENT_FLOAT32:
            return DXGI_FORMAT_R32_FLOAT;
        case D3D10_REGISTER_COMPONENT_SINT32:
            return DXGI_FORMAT_R32_SINT;
        case D3D10_REGISTER_COMPONENT_UINT32:
            return DXGI_FORMAT_R32_UINT;
        }
    }

    return DXGI_FORMAT_UNKNOWN;
}

std::string Shader::SignatureOf(Type type)
{
    switch (type)
    {
    case Type::Vertex:
        return "vs";
    case Type::Hull:
        return "hs";
    case Type::Domain:
        return "ds";
    case Type::Geomerty:
        return "gs";
    case Type::Pixel:
        return "ps";
    case Type::Compute:
        return "cs";
    default:
        throw std::exception("the type is incorrect.");
    }
}

Shader::Type Shader::TypeOf(const std::string& type)
{
    if (type.length() != 2 || type[1] != 's')
    {
        throw  std::exception(StringUtility::Format("the signature({0}) is invalid signature is [vs,hs,ds,gs,ps,cs]", type).data());
    }
    if (type[0] == 'v')
        return Type::Vertex;
    else if(type[0] == 'h')
        return Type::Hull;
    else if(type[0] == 'd')
        return Type::Domain;
    else if(type[0] == 'g')
        return Type::Geomerty;
    else if(type[0] == 'p')
        return Type::Pixel;
    else if(type[0] == 'c')
        return Type::Compute;
    else
    {
        throw  std::exception(StringUtility::Format("the signature({0}) is invalid signature is [vs,hs,ds,gs,ps,cs]", type).data());
    }
}

bool Shader::Initialize(std::shared_ptr<ShaderSource> source, Type type)
{
    if (source == nullptr)
    {
        throw std::exception("ShaderSource can't set nullptr");
    }
    this->_impl = std::make_unique<Shader::_Impl>();
    const auto signature = SignatureOf(type);
    this->_key = source->Key() + "_" + signature;
    this->_impl->path = source->Path();

#ifdef _DEBUG
    const char* OptionalName = this->_key.data();
#else
    const char* OptionalName = nullptr;
#endif // _DEBUG
    ID3DBlob* resultBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;
    auto result = D3DCompile2(
        source->Source().data(),
        source->Source().length(),
        OptionalName,
        nullptr,
        &Shader::_Impl::includeImpl,
        signature.data(),
        StringUtility::Format("{0}_5_0", signature.data()).data(),
        0U, 0U, 0U, nullptr, 0U, &resultBlob, &errorBlob
    );
    if (FAILED(result))
    {
        // TODO Logger
        auto message = StringUtility::Format("shader compile failed: {0} {1}", reinterpret_cast<char*>(errorBlob->GetBufferPointer()), __FILE__);
        throw std::exception(message.data());
    }

    // copy blob to vector
    const auto BufferSize = resultBlob->GetBufferSize();
    this->_impl->blob.reserve(BufferSize);
    this->_impl->blob.resize(BufferSize);
    std::memcpy(this->_impl->blob.data(), resultBlob->GetBufferPointer(), BufferSize);

    this->InitializeD3DResource();

    return true;
}

bool Shader::Initialize(const std::string& key, const std::string& path)
{
    Resource::Initialize();
    this->_key = key;
    this->_impl->path = path;

    // 2 words of key tail
    this->_impl->type = TypeOf(this->_key.substr(this->_key.length() - 2, 2));

    // compiled shader
    StreamReader reader(path, IOMode::Binary);
    this->_impl->blob = reader.ReadAllBytes();
    
    this->InitializeD3DResource();

    return true;
}

void Shader::Finalize()
{
    this->_impl->path.clear();
    this->_key.clear();
    this->_impl->blob.clear();

    Resource::Finalize();
}

ID3D11InputLayout* Shader::InputLayout() const
{
    return this->_impl->layout.Get();
}

// ready blob
void Shader::InitializeD3DResource()
{
    ComPtr<ID3D11ShaderReflection> reflection;
    auto result = D3DReflect(
        this->_impl->blob.data(),
        static_cast<unsigned int>(this->_impl->blob.size()),
        __uuidof(ID3D11ShaderReflection), 
        (void**)reflection.ReleaseAndGetAddressOf());
    
    ThrowIfFailed(result);
    
    D3D11_SHADER_DESC desc;
    reflection->GetDesc(&desc);

    // InputLayout
    std::vector<CD3D11_INPUT_ELEMENT_DESC> elements;
    for (auto L10 = 0U; L10 < desc.InputParameters; L10++) 
    {
        D3D11_SIGNATURE_PARAMETER_DESC elementDesc;
        reflection->GetInputParameterDesc(L10, &elementDesc);

        const auto format = GetDxgiFormat(elementDesc.ComponentType, elementDesc.Mask);

        elements.emplace_back(elementDesc.SemanticName,
                              elementDesc.SemanticIndex,
                              format,
                              0,
                              D3D11_APPEND_ALIGNED_ELEMENT,
                              D3D11_INPUT_PER_VERTEX_DATA,
                              0);
    }
    auto device = DirectXManager::Instance()->GetD3DDevice();
    device->CreateInputLayout(elements.data(),
                              static_cast<unsigned int>(elements.size()),
                              this->_impl->blob.data(),
                              static_cast<unsigned int>(this->_impl->blob.size()),
                              this->_impl->layout.ReleaseAndGetAddressOf()
                              );
}

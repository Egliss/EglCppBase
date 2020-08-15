#pragma once

#include <d3d11.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>

#include "../../../App/AppConfiguration.hpp"
#include "../../../Utility/StringUtility.hpp"
#include "../../../Utility/StreamReader.hpp"

namespace Egliss::Rendering
{
    class ShaderIncludeImpl : public ID3DInclude
    {
    public:
        ShaderIncludeImpl()
        {
            auto directory = std::filesystem::current_path().string() + "/" + std::string(AppConfiguration::ShaderRoot);
            directory = StringUtility::Replace(directory, "\\", "/");
            auto itr = std::filesystem::recursive_directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied);
            for (auto& entry : itr)
            {
                _includeDirectories.emplace_back(entry.path().string());
            }
        }
        static std::string ReadShaderSource(const std::string& shaderFilePath)
        {
            StreamReader reader(shaderFilePath, Egliss::IOMode::Binary);
            return reader.ReadToEnd();
        }
        const std::string& GetShaderSource(const std::string& shaderFilePath, bool discaredCache = false)
        {
            std::filesystem::path path(shaderFilePath);
            const auto key = path.filename().string();
            auto itr = this->_shaderSourceCache.find(key);
            if (itr != this->_shaderSourceCache.end())
            {
                if (discaredCache)
                {
                    const auto source = ReadShaderSource(shaderFilePath);
                    itr->second = source;
                }
                return itr->second;
            }
            // first register
            const auto source = ReadShaderSource(shaderFilePath);
            const auto kvp = this->_shaderSourceCache.emplace(key, source);
            return kvp.first->second;
        }
        virtual HRESULT __stdcall Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID, LPCVOID* ppData, UINT* pBytes) override
        {
            // TODO D3D_COMPILE_STANDARD_FILE_INCLUDE allow relative include
            try
            {
                switch (IncludeType)
                {
                case D3D_INCLUDE_LOCAL:
                case D3D_INCLUDE_SYSTEM:
                    break;
                default:
                    throw std::exception(StringUtility::Format("D3DInclude return invalid include type({0})", IncludeType).data());
                }

                auto& source = GetShaderSource(pFileName, false);
                if (!source.empty())
                {
                    *ppData = source.data();
                    *pBytes = static_cast<unsigned  int>(source.size());
                    return S_OK;
                }
                else
                    return E_FAIL;
            }
            catch (std::exception&)
            {
                return E_FAIL;
            }
        }
        virtual HRESULT __stdcall Close(LPCVOID) override
        {
            return S_OK;
        }

        void DiscaredCache(bool discaredCache)
        {
            this->_discaredCache = discaredCache;
        }
    private:
        bool _discaredCache = false;
        std::vector<std::string> _includeDirectories;
        std::unordered_map<std::string, std::string> _shaderSourceCache;
    };
}
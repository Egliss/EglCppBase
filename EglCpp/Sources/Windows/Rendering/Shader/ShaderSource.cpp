#include "pch.h"
#include "ShaderSource.hpp"
#include "../../../Utility/StreamReader.hpp"
#include "../../../Utility/StringUtility.hpp"

using namespace Egliss;
using namespace Egliss::Rendering;

bool ShaderSource::Initialize(const std::string& key, const std::string& path)
{
    Resource::Initialize();
    this->_key = key;
    this->_path = path;

    StreamReader reader(path, Egliss::IOMode::Input);
    _shaderSource = reader.ReadToEnd();
    return true;
}

void ShaderSource::Finalize()
{
    this->_path.clear();
    this->_shaderSource.clear();
}

#include "pch.h"
#include "Random.hpp"
#include "../../Reflection/StaticType.hpp"

#include <random>

namespace Egliss
{
    class Random::_Impl
    {
    public:
        std::mt19937 _engine;
        std::normal_distribution<float> _distribution;
        using ParamT = std::normal_distribution<float>::param_type;
    };
}

using namespace Egliss;
using namespace Egliss::Reflection;

void Random::Seed(unsigned int seed)
{
    this->_impl->_engine.seed(seed);
}

float Random::Range(float min, float max)
{
    _impl->_distribution.param(Random::_Impl::ParamT{ min, max});
    return _impl->_distribution(_impl->_engine);
}

void Random::Initialize()
{
    this->_impl = new Random::_Impl();
}

void Random::Finalize()
{
    delete this->_impl;
}

std::string Random::Name() const
{
    return StaticTypeDescription<Random>::Name.data();
}

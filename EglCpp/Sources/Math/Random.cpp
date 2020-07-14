#include "pch.h"
#include "Random.hpp"
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

std::unique_ptr<Random> Random::_instance;

Egliss::Random::~Random()
{
}

void Random::Initialize(unsigned int seed)
{
    _instance = std::make_unique<Random>();
    _instance->_impl = new Random::_Impl();
    _instance->_impl->_engine.seed(seed);
;}

void Random::Finalize()
{
    delete _instance->_impl;
    _instance.reset();
}

float Random::Range(float min, float max)
{
    auto& impl = *_instance->_impl;
    impl._distribution.param(Random::_Impl::ParamT{ min, max});
    return impl._distribution(impl._engine);
}

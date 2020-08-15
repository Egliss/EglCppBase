#include "pch.h"
#include "RenderingController.hpp"

#include <CommonStates.h>
#include <Effects.h>

using namespace Egliss::Rendering;
using namespace DirectX;

void RenderingController::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
	this->_states = std::make_unique<CommonStates>(device);
	this->_effectFactory = std::make_unique<EffectFactory>(device);
	this->_rootBatch = std::make_unique<BasicEffect>(device);
}

void RenderingController::Finalize()
{
	this->_effectFactory.reset();
	this->_rootBatch.reset();
	this->_states.reset();
}

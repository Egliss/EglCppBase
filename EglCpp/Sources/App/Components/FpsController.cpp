#include "pch.h"
#include "FpsController.hpp"

using namespace Egliss;

void FpsController::Initialize()
{
}

void FpsController::Finalize()
{
}

void FpsController::TargetFps(int fps)
{
	this->_fps = fps;
}


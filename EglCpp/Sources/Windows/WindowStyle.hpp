#pragma once

#include "WindowsShared.hpp"
// TODO winnt.hÇ™âÛÇÍÇÈÇÃÇ≈É_ÉÅ
// #include <WinUser.h>
namespace Egliss
{
	enum class WindowStyle : unsigned int
	{
		Windowed = WS_OVERLAPPEDWINDOW | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		AeroBorderless = WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX,
		Borderless = WS_POPUP | WS_THICKFRAME | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX,
	};
}
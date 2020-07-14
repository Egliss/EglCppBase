#pragma once

#include "../App/IApplicationInitializeArg.hpp"

struct HINSTANCE__;

namespace Egliss
{
	class WindowsInitializeArg : public IApplicationInitializeArg
	{
	public:
		virtual ~WindowsInitializeArg() = default;

		HINSTANCE__* hInstance = nullptr;
		HINSTANCE__* hPrevInstance = nullptr;
		const wchar_t* lpCmdLine = nullptr;
		int nCmdShow = 0;
	};
}
#include "pch.h"

#ifdef _WIN32
#include "App/Application.hpp"
#include "Windows/WindowsShared.hpp"
#include "Windows/WindowsApplicationImpl.hpp"
#include "Windows/WindowsInitializeArg.hpp"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	auto impl = std::make_unique<Egliss::WindowsApplicationImpl>();
	Egliss::WindowsInitializeArg arg;
	arg.hInstance = hInstance;
	arg.hPrevInstance = hPrevInstance;
	arg.lpCmdLine = lpCmdLine;
	arg.nCmdShow = nCmdShow;

	bool initialized = Egliss::Application::Initialize(std::move(impl), std::move(arg));
	if (!initialized)
		return false;

	MSG message;
	auto& app = Egliss::Application::GetImplAs<Egliss::WindowsApplicationImpl>();

	while (true)
	{
		auto ee = PeekMessageA(&message, nullptr, 0, 0, PM_REMOVE);
		if(ee > 0)
		{
			if (message.message == WM_QUIT)
				break;
			TranslateMessage(&message);
			DispatchMessageA(&message);
		}
		else 
		{
			app.Update();

		}
	}

	return static_cast<int>(message.wParam);
}
#endif
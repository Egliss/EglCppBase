#include "pch.h"

#include "../App/Application.hpp"
#include "../App/AppConfiguration.hpp"
#include "WindowsShared.hpp"
#include "WindowStyle.hpp"
#include "WindowsApplicationImpl.h"
#include "WindowsInitializeArg.h"

#include "Rendering/DirectXManager.hpp"

#include <iostream>

using namespace Egliss;

#pragma region Local

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXA wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WindowsApplicationImpl::ProcCall;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursorA(hInstance, (const char*)IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = "";
	wcex.lpszClassName = AppConfiguration::ApplicationClassName.data();
	wcex.hIcon = 0;
	wcex.hIconSm = 0;

	auto result = RegisterClassExA(&wcex);
	return result;
}

bool InitInstance(HINSTANCE handleInstance, int nCmdShow, HWND& outHWND)
{
	handleInstance = handleInstance;

	HWND hWnd = CreateWindowExA(
		0,
		AppConfiguration::ApplicationClassName.data(),
		AppConfiguration::ApplicationName.data(),
		static_cast<DWORD>(WindowStyle::Windowed),
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, handleInstance, nullptr);

	if (!hWnd)
	{
		return false;
	}
	outHWND = hWnd;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return true;
}
#pragma endregion

bool WindowsApplicationImpl::Initialize(IApplicationInitializeArg&& arg)
{
	auto&& winArg = static_cast<WindowsInitializeArg&&>(arg);

	this->_hinstance = winArg.hInstance;

	MyRegisterClass(winArg.hInstance);
	auto initialized = InitInstance(winArg.hInstance, winArg.nCmdShow, this->_hwnd);
	if (!initialized)
		return initialized;

	Rendering::DirectXManager::Initialize();

	return initialized;
}

void WindowsApplicationImpl::Finalize()
{

}

LRESULT WindowsApplicationImpl::ProcCall(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	std::cout << message << std::endl;
	switch (message)
	{
	case WM_SIZE:
	{
		break;
	}
	case WM_COMMAND:
	{
		return DefWindowProcA(hWnd, message, wParam, lParam);
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		// TODO: HDC ���g�p����`��R�[�h�������ɒǉ����Ă�������...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcA(hWnd, message, wParam, lParam);
	}
	return 0;
}
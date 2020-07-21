#include "pch.h"

#include "../App/Application.hpp"
#include "../App/AppConfiguration.hpp"
#include "WindowsShared.hpp"
#include "WindowStyle.hpp"
#include "WindowsApplicationImpl.hpp"
#include "WindowsInitializeArg.hpp"

#include "Rendering/DirectXManager.hpp"
#include "DirectXUtility.hpp"

#include <iostream>

using namespace Egliss;

#pragma region Local

ATOM RegisterWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEXA wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WindowsApplicationImpl::ProcCall;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursorA(hInstance, (const char*)IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
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
	auto style = GetWindowLongA(hWnd, GWL_EXSTYLE);
	SetWindowLongA(hWnd, GWL_EXSTYLE, style | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hWnd,RGB(0, 0, 0),0, LWA_COLORKEY);

	return true;
}
#pragma endregion

bool WindowsApplicationImpl::Initialize(IApplicationInitializeArg&& arg)
{
	auto&& winArg = static_cast<WindowsInitializeArg&&>(arg);

	this->_hinstance = winArg.hInstance;

	RegisterWindowClass(winArg.hInstance);
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
			auto& impl = Application::GetImplAs<WindowsApplicationImpl>();
			if (wParam == SIZE_MINIMIZED)
			{
				if (!impl._isMinimized)
				{
					impl._isMinimized = true;
					impl.TrySuspend();
				}
			}
			else if (impl._isMinimized)
			{
				impl._isMinimized = false;
				impl.TryResume();
			}
			else if (impl._isSizeMoving)
			{
				const auto width = static_cast<float>(LOWORD(lParam));
				const auto height = static_cast<float>(HIWORD(lParam));
				impl._onResized.Call(width, height);
			}
			break;
		}
		case WM_PAINT:
		{
			/*
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			// TODO: HDC を使用する描画コードをここに追加してください...
			EndPaint(hWnd, &ps);
			*/
			break;
		}
		case WM_ENTERSIZEMOVE:
		{
			Application::GetImplAs<WindowsApplicationImpl>()._isSizeMoving = true;
			break;
		}
		case WM_EXITSIZEMOVE:
		{
			Application::GetImplAs<WindowsApplicationImpl>()._isSizeMoving = false;
			RECT rect;
			GetClientRect(hWnd, &rect);
			const auto width = static_cast<float>(rect.right - rect.left);
			const auto height = static_cast<float>(rect.bottom - rect.top);
			Application::GetImplAs<WindowsApplicationImpl>()._onResized.Call(width, height);
			break;
		}
		case WM_ACTIVATE:
		{
			if (wParam)
				Application::GetImplAs<WindowsApplicationImpl>()._onActivated.Call();
			else
				Application::GetImplAs<WindowsApplicationImpl>()._onDeactivated.Call();
			break;
		}
		case WM_POWERBROADCAST:
		{
			auto& impl = Application::GetImplAs<WindowsApplicationImpl>();
			switch (wParam)
			{
			case PBT_APMQUERYSUSPEND:
				if (!impl._isSuspended)
				{
					impl.TrySuspend();
					return TRUE;
				}
				break;
			case PBT_APMRESUMESUSPEND:
				if (!impl._isMinimized)
				{
					impl.TryResume();
					return TRUE;
				}
			}
			break;
		}
		case WM_GETMINMAXINFO:
		if (lParam)
		{
			const auto& impl = Application::GetImplAs<WindowsApplicationImpl>();
			auto info = reinterpret_cast<MINMAXINFO*>(lParam);
			info->ptMinTrackSize.x = impl.MinimumWidth;
			info->ptMinTrackSize.y = impl.MinimumHeight;
		}
		break;
		case WM_DESTROY:
		{

			PostQuitMessage(0);
			break;
		}
	}
	return DefWindowProcA(hWnd, message, wParam, lParam);
}

void WindowsApplicationImpl::TrySuspend()
{
	auto& impl = Application::GetImplAs<WindowsApplicationImpl>();

	if (!impl._isSuspended)
		impl._onSuspend.Call();
	impl._isSuspended = true;
}

void WindowsApplicationImpl::TryResume()
{
	auto& impl = Application::GetImplAs<WindowsApplicationImpl>();
	if (impl._isSuspended)
		impl._onResumed.Call();
	impl._isSuspended = false;
}

WindowsApplicationImpl::WindowsApplicationImpl() :IApplicationImpl(), _hinstance(0), _hwnd(0),_isMinimized(false),_isSuspended(false),_isSizeMoving(false)
{
	if (OleInitialize(nullptr) >= 0)
	{
		CoUninitialize();
		DirectXUtility::ThrowIfFailed(CoInitializeEx(nullptr, COINIT_MULTITHREADED));
	}
}

WindowsApplicationImpl::~WindowsApplicationImpl()
{
	CoUninitialize();
	OleUninitialize();
}

#include "pch.h"


#include "../App/Components/FpsController.hpp"
#include "../App/Application.hpp"
#include "../App/AppConfiguration.hpp"
#include "WindowsShared.hpp"
#include "WindowsApplicationImpl.hpp"
#include "WindowsInitializeArg.hpp"

#include "Rendering/DirectXManager.hpp"
#include "DirectXUtility.hpp"

#include <Keyboard.h>
#include <Mouse.h>

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

bool InitInstance(HINSTANCE handleInstance, int nCmdShow, WindowStyle windowStyle, HWND& outHWND)
{
	handleInstance = handleInstance;
	HWND hWnd = CreateWindowExA(
		0,
		AppConfiguration::ApplicationClassName.data(),
		AppConfiguration::ApplicationName.data(),
		static_cast<DWORD>(windowStyle),
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

void WindowsApplicationImpl::Resize(Vector2 size)
{
	RECT rect;
	GetWindowRect(this->_hwnd, &rect);
	SetWindowPos(this->_hwnd, 0, rect.left, rect.top, static_cast<int>(size.x), static_cast<int>(size.y), static_cast<DWORD>(this->_style));
	// Window Proc will fire event
}

bool WindowsApplicationImpl::Initialize(IApplicationInitializeArg&& arg)
{
	auto&& winArg = static_cast<WindowsInitializeArg&&>(arg);

	this->_hinstance = winArg.hInstance;
	this->_style = WindowStyle::Windowed;

	RegisterWindowClass(winArg.hInstance);
	auto initialized = InitInstance(winArg.hInstance, winArg.nCmdShow,this->_style, this->_hwnd);
	if (!initialized)
		return initialized;

	RECT rect;
	GetClientRect(this->_hwnd,&rect);
	this->_size = Vector2(static_cast<float>(rect.right - rect.left), static_cast<float>(rect.bottom - rect.top));

	Rendering::DirectXManager::Initialize();

	return initialized;
}

void WindowsApplicationImpl::Finalize()
{

}

void WindowsApplicationImpl::Update()
{
	Application::GetAppComponent<FpsController>().Tick([]()
	{

		Rendering::DirectXManager::GetInstance().Render();
	});
	// UpdateWindow(this->_hwnd);
}

LRESULT WindowsApplicationImpl::ProcCall(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
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
			else if (wParam == SIZE_MAXSHOW)
			{
				//	auto style = GetWindowLongA(hWnd, GWL_EXSTYLE);
	            // SetWindowLongA(hWnd, GWL_EXSTYLE, style | WS_EX_LAYERED);
	            // SetLayeredWindowAttributes(hWnd,RGB(0, 0, 0),0, LWA_COLORKEY);
				if (impl._isMinimized)
				{
					impl._isMinimized = false;
					impl.TryResume();
				}
			}
			else if (impl._isSizeMoving)
			{
				impl._size.x = static_cast<float>(LOWORD(lParam));
				impl._size.y = static_cast<float>(HIWORD(lParam));
				impl._onResized.Call(impl._size);
			}
			break;
		}
		case WM_ENTERSIZEMOVE:
		{
			Application::GetImplAs<WindowsApplicationImpl>()._isSizeMoving = true;
			break;
		}
		case WM_EXITSIZEMOVE:
		{
			auto& impl = Application::GetImplAs<WindowsApplicationImpl>();
			impl._isSizeMoving = false;
			RECT rect;
			GetClientRect(hWnd, &rect);
			impl._size.x = static_cast<float>(rect.right - rect.left);
			impl._size.y = static_cast<float>(rect.bottom - rect.top);
			impl._onResized.Call(impl._size);
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
				}
				break;
			case PBT_APMRESUMESUSPEND:
				if (!impl._isMinimized)
				{
					impl.TryResume();
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
	#pragma region InputEvent
		case WM_INPUT:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_MOUSEHOVER:
			DirectX::Mouse::ProcessMessage(message, wParam, lParam);
			break;
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
			break;
	#pragma endregion

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
	{
		impl._onSuspend.Call();
	}
	impl._isSuspended = true;
}

void WindowsApplicationImpl::TryResume()
{
	auto& impl = Application::GetImplAs<WindowsApplicationImpl>();
	if (impl._isSuspended)
	{
		impl._onResumed.Call();
	}
	impl._isSuspended = false;
}

Vector2 WindowsApplicationImpl::WindowSize() const
{
	return this->_size;
}

WindowsApplicationImpl::WindowsApplicationImpl() :IApplicationImpl(), _hinstance(0), _hwnd(0),_isMinimized(false),_isSuspended(false),_isSizeMoving(false)
{
	if (OleInitialize(nullptr) >= 0)
	{
		CoUninitialize();
		ThrowIfFailed(CoInitializeEx(nullptr, COINIT_MULTITHREADED));
	}
}

WindowsApplicationImpl::~WindowsApplicationImpl()
{
	CoUninitialize();
	OleUninitialize();
}

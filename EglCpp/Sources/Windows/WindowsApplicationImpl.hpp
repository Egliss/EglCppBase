#pragma once

#include <Windows.h>
#include "WindowStyle.hpp"
#include "../Utility/Event.hpp"
#include "../App/IApplicationImpl.hpp"

namespace Egliss
{
	class WindowsApplicationImpl : public IApplicationImpl
	{
		const int MinimumWidth = 320;
		const int MinimumHeight = 200;

	public:
		WindowsApplicationImpl();
		virtual ~WindowsApplicationImpl();

		virtual bool Initialize(IApplicationInitializeArg&& arg) override;
		virtual void Finalize() override;
		virtual void Update() override;
		virtual std::string GetName() const override { return "Windows"; }
		virtual Event<void()>::IEventRefT OnSuspend() { return this->_onSuspend.AsIEvent(); }
		virtual Event<void()>::IEventRefT OnResumed() { return this->_onResumed.AsIEvent(); }
		virtual Event<void()>::IEventRefT OnActivated() { return this->_onActivated.AsIEvent(); }
		virtual Event<void()>::IEventRefT OnDeactivated() { return this->_onDeactivated.AsIEvent(); }
		virtual Event<void(Vector2)>::IEventRefT OnResized() { return this->_onResized.AsIEvent(); }
		virtual void Resize(Vector2 size) override;
		virtual Vector2 WindowSize() const override;

		HWND GetHWND() const { return this->_hwnd; }
		HINSTANCE GetHINSTANCE() const { return this->_hinstance; }

		static LRESULT __stdcall ProcCall(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	private:
		HINSTANCE _hinstance;
		HWND _hwnd;
		bool _isMinimized;
		bool _isSuspended;
		bool _isSizeMoving;
		WindowStyle _style;
		Vector2 _size;

		Event<void()> _onSuspend;
		Event<void()> _onResumed;
		Event<void()> _onActivated;
		Event<void()> _onDeactivated;
		Event<void(Vector2)> _onResized;

		void TrySuspend();
		void TryResume();

	};
	using WinImpl = WindowsApplicationImpl;
}
#pragma once

#include <Windows.h>
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
		virtual std::string GetName() const override { return "Windows"; }
		virtual IEvent<void()> OnSuspend() { return this->_onSuspend; }
		virtual IEvent<void()> OnResumed() { return this->_onResumed; }
		virtual IEvent<void()> OnActivated() { return this->_onActivated; }
		virtual IEvent<void()> OnDeactivated() { return this->_onDeactivated; }
		virtual IEvent<void(float, float)> OnResized() { return this->_onResized; }

		HWND GetHWND() const { return this->_hwnd; }
		HINSTANCE GetHINSTANCE() const { return this->_hinstance; }

		static LRESULT __stdcall ProcCall(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	private:
		HINSTANCE _hinstance;
		HWND _hwnd;
		bool _isMinimized;
		bool _isSuspended;
		bool _isSizeMoving;

		Event<void()> _onSuspend;
		Event<void()> _onResumed;
		Event<void()> _onActivated;
		Event<void()> _onDeactivated;
		Event<void(float, float)> _onResized;

		void TrySuspend();
		void TryResume();
	};
	using WinImpl = WindowsApplicationImpl;
}
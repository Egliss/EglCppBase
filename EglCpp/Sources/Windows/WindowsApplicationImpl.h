#pragma once

#include "../App/IApplicationImpl.h"
#include "MinDef.hpp"

namespace Egliss
{
	class WindowsApplicationImpl : public IApplicationImpl
	{
	public:
		virtual ~WindowsApplicationImpl() = default;

		virtual bool Initialize(IApplicationInitializeArg&& arg) override;
		virtual void Finalize() override;
		virtual std::string GetName() const override { return "Windows"; }

		HWND GetHWND() const { return this->_hwnd; }
		HINSTANCE GetHINSTANCE() const { return this->_hinstance; }

		static LRESULT __stdcall ProcCall(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	private:
		HINSTANCE _hinstance;
		HWND _hwnd;
	};
	using WinImpl = WindowsApplicationImpl;
}
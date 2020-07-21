#pragma once

#include "../Utility/Event.hpp"
#include "../App/IApplicationInitializeArg.hpp"
#include <string>

namespace Egliss
{
	class IApplicationImpl
	{
	public:

		virtual ~IApplicationImpl() {};
		IApplicationImpl(const IApplicationImpl&) = delete;
		IApplicationImpl& operator=(const IApplicationImpl&) = delete;

		virtual bool Initialize(IApplicationInitializeArg&& arg) = 0;
		virtual void Finalize() = 0;
		virtual IEvent<void()> OnSuspend() = 0;
		virtual IEvent<void()> OnResumed() = 0;
		virtual IEvent<void()> OnActivated() = 0;
		virtual IEvent<void()> OnDeactivated() = 0;
		virtual IEvent<void(float, float)> OnResized() = 0;
		virtual std::string GetName() const = 0;
	protected:
		IApplicationImpl() = default;
	};
}
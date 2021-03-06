#pragma once

#include "../Math/Vector2.hpp"
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
		virtual void Update() = 0;
		virtual void Resize(Vector2) = 0;
		virtual Event<void()>::IEventRefT OnSuspend() = 0;
		virtual Event<void()>::IEventRefT OnResumed() = 0;
		virtual Event<void()>::IEventRefT OnActivated() = 0;
		virtual Event<void()>::IEventRefT OnDeactivated() = 0;
		virtual Event<void(Vector2)>::IEventRefT OnResized() = 0;
		virtual std::string GetName() const = 0;
		virtual Vector2 WindowSize() const = 0;
	protected:
		IApplicationImpl() = default;
	};
}
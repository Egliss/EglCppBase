#pragma once

#include "../IApplicationComponent.hpp"
#include "../../Utility/Event.hpp"
#include "../../Math/Vector2.hpp"

namespace Egliss
{
	class HardwareInput : 
		public IApplicationComponent,
		public IAppComponentUpdateLister
	{
		class _Impl;
	public:
		HardwareInput() : 
			IApplicationComponent(StaticType<Resources>::Id), 
			IAppComponentUpdateLister(StaticType<Resources>::Id) {};
		~HardwareInput() = default;

		enum class InputState : int
		{
			None = 0, Down = 2, Press = 4, Up = 8
		};
		enum class MouseInputType : char
		{
			Left, Middle, Right, Button1, Button2
		};
		InputState GetKeyboardInput(int keyCode);
		InputState GetMouseButtonInput(MouseInputType type);
		float GetMouseScrollInput();
		Vector2 GetCursorPosition();

		virtual void Initialize() override;
		virtual void Finalize() override;
		virtual std::string Name() const override
		{
			return "HardwareInput";
		}

		virtual void Update() override;

	private:
		_Impl* _impl = nullptr;
	};
}

namespace Egliss
{
	inline HardwareInput::InputState operator+(HardwareInput::InputState left, HardwareInput::InputState right)
	{
		return static_cast<HardwareInput::InputState>((int)left + (int)right);
	}
	inline HardwareInput::InputState operator&(HardwareInput::InputState left, HardwareInput::InputState right)
	{
		return static_cast<HardwareInput::InputState>((int)left & (int)right);
	}
	inline HardwareInput::InputState operator|(HardwareInput::InputState left, HardwareInput::InputState right)
	{
		return static_cast<HardwareInput::InputState>((int)left | (int)right);
	}
}
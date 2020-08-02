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
		HardwareInput() = default;
		~HardwareInput() = default;

		enum class InputState : unsigned int
		{
			None = 0, Press = 2, Down = 4, Up = 8
		};
		enum class MouseInputType : unsigned int
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
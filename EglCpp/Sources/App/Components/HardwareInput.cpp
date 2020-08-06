#include "pch.h"
#include "HardwareInput.hpp"
#ifdef _WIN32

#include <Keyboard.h>
#include <Mouse.h>
#include "HardwareInput.hpp"

#include <map>
#include <windows.h>
#include <string>

#endif // _WIN32

using namespace DirectX;

namespace Egliss
{
    class HardwareInput::_Impl
    {
    public:
        HardwareInput::_Impl()
        {
            using MouseStateT = Mouse::ButtonStateTracker::ButtonState;
            using GenericStateT = HardwareInput::InputState;
            MouseStateMap.emplace(MouseStateT::RELEASED, GenericStateT::None);
            MouseStateMap.emplace(MouseStateT::HELD, GenericStateT::Down);
            MouseStateMap.emplace(MouseStateT::PRESSED, GenericStateT::Press);
            MouseStateMap.emplace(MouseStateT::UP, GenericStateT::Up);
        }
        Mouse mouse;
        Keyboard keyboard;
        Mouse::State mouseState;
        Keyboard::State keyState;
        Mouse::ButtonStateTracker mouseTracker;
        Keyboard::KeyboardStateTracker keyTracker;
        std::map<Mouse::ButtonStateTracker::ButtonState, HardwareInput::InputState> MouseStateMap;
    };
}

using namespace Egliss;
#include "../../Utility/StringUtility.hpp"
HardwareInput::InputState HardwareInput::GetKeyboardInput(int keyCode)
{
    const auto key = static_cast<Keyboard::Keys>(keyCode);
    const auto& tracker = this->_impl->keyTracker;
    const auto* statePtr = reinterpret_cast<const char*>(&this->_impl->keyState);

    InputState state = InputState::None;
    state = state + (tracker.pressed.IsKeyDown(key) ? InputState::Down : InputState::None); // OnDown
    state = state + (tracker.released.IsKeyDown(key) ? InputState::Up : InputState::None); // OnUp
    state = state + (statePtr[keyCode] ? InputState::Press : InputState::None);

    return state;
}

HardwareInput::InputState HardwareInput::GetMouseButtonInput(MouseInputType type)
{
    using InputT = HardwareInput::MouseInputType;
    const auto& impl = *this->_impl;
    const auto& tracker = impl.mouseTracker;
    const auto& converter = impl.MouseStateMap;

    switch (type)
    {
    case InputT::Left:
        return converter.at(tracker.leftButton);
    case InputT::Middle:
        return converter.at(tracker.middleButton);
    case InputT::Right:
        return converter.at(tracker.rightButton);
    case InputT::Button1:
        return converter.at(tracker.xButton1);
    case InputT::Button2:
        return converter.at(tracker.xButton2);
    default:
        return HardwareInput::InputState::None;
    }
}

float HardwareInput::GetMouseScrollInput()
{
    return static_cast<float>(Mouse::Get().GetState().scrollWheelValue);
}

Vector2 HardwareInput::GetCursorPosition()
{
    const auto& state = Mouse::Get().GetState();
    return Vector2(static_cast<float>(state.x), static_cast<float>(state.y));
}

void HardwareInput::Initialize()
{
    this->_impl = new HardwareInput::_Impl();

    Keyboard::Get();
    Mouse::Get();

    this->_impl->mouseTracker.Reset();
    this->_impl->keyTracker.Reset();
    this->Update();
}

void HardwareInput::Finalize()
{
    delete this->_impl;
}

#include <WinUser.h>
void HardwareInput::Update()
{
    // TODO twice copy 
    this->_impl->keyState = Keyboard::Get().GetState();
    this->_impl->mouseState = Mouse::Get().GetState();

    this->_impl->keyTracker.Update(this->_impl->keyState);
    this->_impl->mouseTracker.Update(this->_impl->mouseState);

    GetKeyboardInput(VK_LSHIFT);
}


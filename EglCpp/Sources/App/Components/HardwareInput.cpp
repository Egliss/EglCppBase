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
        Mouse::ButtonStateTracker mouseTracker;
        Keyboard::KeyboardStateTracker keyTracker;
        std::map<Mouse::ButtonStateTracker::ButtonState, HardwareInput::InputState> MouseStateMap;
    };
}

using namespace Egliss;
#include "../../Utility/StringUtility.hpp"
HardwareInput::InputState HardwareInput::GetKeyboardInput(int keyCode)
{
    auto key = static_cast<Keyboard::Keys>(keyCode);
    auto& tracker = this->_impl->keyTracker;
    auto e = tracker.pressed.IsKeyDown(key) ? 1 : 0;
    e |= tracker.pressed.IsKeyUp(key) ? 2 : 0;
    e |= tracker.released.IsKeyDown(key) ? 4 : 0;
    e |= tracker.released.IsKeyDown(key) ? 8 : 0;
    auto ee = StringUtility::Format("%d", e);
    OutputDebugStringA(ee.data());
    return static_cast<HardwareInput::InputState>(e);
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

void HardwareInput::Update()
{
    this->_impl->mouseTracker.Update(Mouse::Get().GetState());
    this->_impl->keyTracker.Update(Keyboard::Get().GetState());
}


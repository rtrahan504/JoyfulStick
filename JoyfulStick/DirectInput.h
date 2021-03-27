#pragma once

#include "JoyfulStick.h"
#include <deque>
#include <unordered_map>
#include <string>
#include <stdexcept>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

namespace JoyfulStick
{
    class DirectInput;

    class Joystick
    {
    public:
        JOYFULSTICKEXPORT Joystick(DirectInput& parent, DIDEVICEINSTANCEW& deviceInst);
        Joystick(const Joystick&) = delete;
        Joystick(Joystick&&) = default;
        Joystick& operator=(const Joystick&) = delete;
        Joystick& operator=(Joystick&&) = delete;
        JOYFULSTICKEXPORT ~Joystick();

        static constexpr long AxisRange = 32768;

        JOYFULSTICKEXPORT std::wstring GetName() const;

        JOYFULSTICKEXPORT void Acquire();
        JOYFULSTICKEXPORT void Release();

        class JoystickObject
        {
        public:
            JOYFULSTICKEXPORT JoystickObject(Joystick& parent, DIDEVICEOBJECTINSTANCE);

            DIDEVICEOBJECTINSTANCE m_Inst;
        private:
            Joystick& m_Parent;
        };

        JOYFULSTICKEXPORT double GetX() const;
        JOYFULSTICKEXPORT double GetY() const;
        JOYFULSTICKEXPORT double GetZ() const;
        JOYFULSTICKEXPORT double GetRX() const;
        JOYFULSTICKEXPORT double GetRY() const;
        JOYFULSTICKEXPORT double GetRZ() const;
        JOYFULSTICKEXPORT double GetSlider(unsigned int i) const;

        JOYFULSTICKEXPORT unsigned int GetNumAxes() const;
        JOYFULSTICKEXPORT double GetAxisValue(unsigned int i) const;

        JOYFULSTICKEXPORT unsigned int GetNumPOVs() const;
        JOYFULSTICKEXPORT long GetPOV(unsigned int i) const;

        JOYFULSTICKEXPORT unsigned int GetNumButtons() const;
        JOYFULSTICKEXPORT bool GetButton(unsigned int i) const;

        JOYFULSTICKEXPORT std::deque<JoystickObject*>& GetAxes();
        JOYFULSTICKEXPORT std::deque<JoystickObject*>& GetButtons();
        JOYFULSTICKEXPORT std::deque<JoystickObject*>& GetPOVs();

        //! Call this to update the state of the joystick
        JOYFULSTICKEXPORT bool Poll();

    private:

        DirectInput& m_Parent;
        LPDIRECTINPUTDEVICE8W m_Handle = nullptr;
        std::wstring m_Name;
        DIDEVCAPS m_Capabilities;
        DIJOYSTATE2 m_State;
        bool m_IsAcquired = false;

        std::deque<JoystickObject*> m_Axes;
        std::deque<JoystickObject*> m_Buttons;
        std::deque<JoystickObject*> m_POVs;

        std::deque<JoystickObject> m_Objects;
    };

    class DirectInput
    {
    public:
        JOYFULSTICKEXPORT DirectInput();
        JOYFULSTICKEXPORT ~DirectInput();

        JOYFULSTICKEXPORT void EnumerateJoysticks();

        JOYFULSTICKEXPORT size_t GetNumJoysticks() const;
        JOYFULSTICKEXPORT Joystick GetJoystick(unsigned int i);

    protected:
        friend class Joystick;
        JOYFULSTICKEXPORT IDirectInput* const GetDInputHandle();

    private:
        HINSTANCE m_ProcInst;
        IDirectInput* m_DInput = nullptr;
        std::deque<DIDEVICEINSTANCEW> m_Joysticks;
    };
}

#include "DirectInput.h"

namespace JoyfulStick
{
    DirectInput::DirectInput()
    {
        m_ProcInst = GetModuleHandle(nullptr);

        HRESULT res = DirectInput8Create(m_ProcInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)(IDirectInput**)(&m_DInput), nullptr);
        if (res != S_OK)
            throw std::runtime_error("Unable to create the direct input device.");
    }
    DirectInput::~DirectInput()
    {
        m_DInput->Release();
    }

    void DirectInput::EnumerateJoysticks()
    {
        auto EnumerateJoysticks_Callback = [](LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef) -> BOOL
        {
            auto This = (DirectInput*)pvRef;
            This->m_Joysticks.push_back(*lpddi);

            return DIENUM_CONTINUE; // DIENUM_STOP to stop enumeration now            
        };

        m_Joysticks.clear();
        m_DInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumerateJoysticks_Callback, this, 0);
    }

    IDirectInput* const DirectInput::GetDInputHandle() { return m_DInput; }
    size_t DirectInput::GetNumJoysticks() const { return m_Joysticks.size(); }
    Joystick DirectInput::GetJoystick(unsigned int i)
    {
        return Joystick(*this, m_Joysticks[i]);
    }


    Joystick::Joystick(DirectInput& parent, DIDEVICEINSTANCEW& deviceInst) :
        m_Parent(parent)
    {
        if (m_Parent.GetDInputHandle()->CreateDevice(deviceInst.guidInstance, (LPDIRECTINPUTDEVICEW*)&m_Handle, nullptr) != DI_OK)
            throw std::runtime_error("Error opening joystick connection.");

        m_Name = deviceInst.tszProductName;

        if (m_Handle->SetCooperativeLevel(nullptr, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE) != DI_OK)
            throw std::runtime_error("Error settings the cooperation level.");

        if (m_Handle->SetDataFormat(&c_dfDIJoystick2) != DI_OK)
            throw std::runtime_error("Error setting data format.");

        m_Capabilities.dwSize = sizeof(DIDEVCAPS);
        auto ret = m_Handle->GetCapabilities(&m_Capabilities);
        if (ret != DI_OK)
            throw std::runtime_error("Error getting the capabilities.");

        auto EnumAxes = [](LPCDIDEVICEOBJECTINSTANCE objectInstance, LPVOID pvRef) -> BOOL
        {
            auto This = (Joystick*)pvRef;

            auto type = objectInstance->dwType & 0xFF;
            unsigned int ObjectIndex = (objectInstance->dwType & 0xFF00) >> 8;

            auto& item = This->m_Objects.emplace_back(*This, *objectInstance);

            if (type & (DIDFT_AXIS | DIDFT_ABSAXIS))
            {
                unsigned int index = -1;

                if (objectInstance->guidType == GUID_XAxis) index = 0;
                else if (objectInstance->guidType == GUID_YAxis) index = 1;
                else if (objectInstance->guidType == GUID_ZAxis) index = 2;

                else if (objectInstance->guidType == GUID_RxAxis) index = 3;
                else if (objectInstance->guidType == GUID_RyAxis) index = 4;
                else if (objectInstance->guidType == GUID_RzAxis) index = 5;

                else if (objectInstance->guidType == GUID_Slider) index = 6;

                if (index != -1)
                {
                    while (This->m_Axes.size() < index + 1ULL) This->m_Axes.emplace_back();
                    This->m_Axes[index] = &item;
                }
            }
            else if (type & DIDFT_BUTTON)
            {
                unsigned int index = ObjectIndex;
                if (index != -1)
                {
                    while (This->m_Buttons.size() < index + 1ULL) This->m_Buttons.emplace_back();
                    This->m_Buttons[index] = &item;
                }
            }
            else if (type & DIDFT_POV)
            {
                unsigned int index = ObjectIndex;
                if (index != -1)
                {
                    while (This->m_POVs.size() < index + 1ULL) This->m_POVs.emplace_back();
                    This->m_POVs[index] = &item;
                }
            }

            return true;
        };
        m_Handle->EnumObjects(EnumAxes, this, DIDFT_ALL);
    }

    Joystick::~Joystick()
    {
        Release();
        m_Handle = nullptr;
    }

    std::wstring Joystick::GetName() const { return m_Name; }


    double Joystick::GetX() const { return (double)m_State.lX / AxisRange; }
    double Joystick::GetY() const { return (double)m_State.lY / AxisRange; }
    double Joystick::GetZ() const { return (double)m_State.lZ / AxisRange; }

    double Joystick::GetRX() const { return (double)m_State.lRx / AxisRange; }
    double Joystick::GetRY() const { return (double)m_State.lRy / AxisRange; }
    double Joystick::GetRZ() const { return (double)m_State.lRz / AxisRange; }

    double Joystick::GetSlider(unsigned int i) const
    {
        return (double)m_State.rglSlider[i] / AxisRange;
    }

    unsigned int Joystick::GetNumAxes() const { return std::min<unsigned int>(7, (unsigned int)m_Axes.size()); }
    double Joystick::GetAxisValue(unsigned int i) const
    {
        if (i == 0)
            return (double)m_State.lX / AxisRange;
        else if (i == 1)
            return (double)m_State.lY / AxisRange;
        else if (i == 2)
            return (double)m_State.lZ / AxisRange;

        else if (i == 3)
            return (double)m_State.lRx / AxisRange;
        else if (i == 4)
            return (double)m_State.lRy / AxisRange;
        else if (i == 5)
            return (double)m_State.lRz / AxisRange;

        else if (i == 6)
            return (double)m_State.rglSlider[0] / AxisRange;
        else if (i == 7)
            return (double)m_State.rglSlider[1] / AxisRange;

        return 0;
    }

    long Joystick::GetPOV(unsigned int i) const
    {
        auto val = m_State.rgdwPOV[i];

        return val == -1 ? val : val / 100;
    }

    unsigned int Joystick::GetNumPOVs() const { return (unsigned int)m_POVs.size(); }
    unsigned int Joystick::GetNumButtons() const { return (unsigned int)m_Buttons.size(); }
    bool Joystick::GetButton(unsigned int i) const { return m_State.rgbButtons[i]; }

    std::deque<Joystick::JoystickObject*>& Joystick::GetAxes() { return m_Axes; }
    std::deque<Joystick::JoystickObject*>& Joystick::GetButtons() { return m_Buttons; }
    std::deque<Joystick::JoystickObject*>& Joystick::GetPOVs() { return m_POVs; }

    void Joystick::Acquire()
    {
        Release();

        if (!m_Handle || FAILED(m_Handle->Acquire()))
            throw std::runtime_error("Failed to acquire");
        m_IsAcquired = true;
    }

    void Joystick::Release()
    {
        if (!m_IsAcquired || !m_Handle)
            return;

        m_Handle->Unacquire();
        m_Handle->Release();
        m_IsAcquired = false;
    }

    bool Joystick::Poll()
    {
        if (!m_Handle)
            return false;

        ZeroMemory(&m_State, sizeof(m_State));

        auto hr = m_Handle->Poll();

        if (FAILED(hr))
        {
            // DirectInput lost the device, try to re-acquire it
            hr = m_Handle->Acquire();
            while (hr == DIERR_INPUTLOST)
                hr = m_Handle->Acquire();

            // return if a fatal error is encountered
            if ((hr == DIERR_INVALIDPARAM) || (hr == DIERR_NOTINITIALIZED))
                return false;

            // if another application has control of this device, we have to wait for our turn
            if (hr == DIERR_OTHERAPPHASPRIO)
                return false;
        }

        // now if everything is okay, we can get the state of the device

        hr = m_Handle->GetDeviceState(sizeof(m_State), &m_State);

        if (FAILED(hr))
            return false;

        return true;
    }

    Joystick::JoystickObject::JoystickObject(Joystick& parent, DIDEVICEOBJECTINSTANCE inst) :
        m_Parent(parent),
        m_Inst(inst)
    {
        if (inst.dwType & DIDFT_AXIS)
        {
            DIPROPDWORD gameControllerDeadZone;

            // set the dead zone to 1%
            gameControllerDeadZone.dwData = 1000;

            // set the size of the structure
            gameControllerDeadZone.diph.dwSize = sizeof(DIPROPDWORD);
            gameControllerDeadZone.diph.dwHeaderSize = sizeof(DIPROPHEADER);

            // set the object that we want to change
            gameControllerDeadZone.diph.dwHow = DIPH_BYID;
            gameControllerDeadZone.diph.dwObj = m_Inst.dwType;

            // now set the dead zone for the axis
            if (FAILED(parent.m_Handle->SetProperty(DIPROP_DEADZONE, &gameControllerDeadZone.diph)))
                throw std::runtime_error("Error setting axis property.");

            DIPROPRANGE gameControllerRange;

            // set the range
            gameControllerRange.lMin = -Joystick::AxisRange;
            gameControllerRange.lMax = Joystick::AxisRange;

            // set the size of the structure
            gameControllerRange.diph.dwSize = sizeof(DIPROPRANGE);
            gameControllerRange.diph.dwHeaderSize = sizeof(DIPROPHEADER);

            // set the object that we want to change		
            gameControllerRange.diph.dwHow = DIPH_BYID;
            gameControllerRange.diph.dwObj = m_Inst.dwType;

            // now set the range for the axis		
            if (FAILED(parent.m_Handle->SetProperty(DIPROP_RANGE, &gameControllerRange.diph)))
                throw std::runtime_error("Error setting axis range.");
        }
    }
}

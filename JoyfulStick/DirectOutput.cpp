#include "DirectOutput.h"

extern "C" {
#include "Include/DirectOutput.h"
}

#include <sstream>

namespace JoyfulStick
{
    enum NativeLEDs : DWORD
    {
        LED_FIRE = 0,
        LED_FIRE_A_RED = 1,
        LED_FIRE_A_GREEN = 2,
        LED_FIRE_B_RED = 3,
        LED_FIRE_B_GREEN = 4,
        LED_FIRE_D_RED = 5,
        LED_FIRE_D_GREEN = 6,
        LED_FIRE_E_RED = 7,
        LED_FIRE_E_GREEN = 8,
        LED_TOGGLE_1_2_RED = 9,
        LED_TOGGLE_1_2_GREEN = 10,
        LED_TOGGLE_3_4_RED = 11,
        LED_TOGGLE_3_4_GREEN = 12,
        LED_TOGGLE_5_6_RED = 13,
        LED_TOGGLE_5_6_GREEN = 14,
        LED_POV_2_RED = 15,
        LED_POV_2_GREEN = 16,
        LED_CLUTCH_RED = 17,
        LED_CLUTCH_GREEN = 18,
        LED_THROTTLE = 19
    };

    enum class MFD_Line : DWORD
    {
        MFD_STRING_TOP = 0,
        MFD_STRING_MIDDLE = 1,
        MFD_STRING_BOTTOM = 2
    };

    void __stdcall DirectOutput::Device_Callback(void* hDevice, bool bAdded, void* pvContext)
    {
        auto This = (DirectOutput*)pvContext;

        if (bAdded)
            This->m_Devices.push_back(hDevice);
    }

    void __stdcall DirectOutput::Enumerate_Callback(void* hDevice, void* pvContext)
    {
        auto This = (DirectOutput*)pvContext;

        This->m_Devices.push_back(hDevice);
    }


    DirectOutput::DirectOutput()
    {
        const wchar_t* name = L"JoyfulStick";
        DirectOutput_Initialize(name);
        EnumerateDevices();
    }

    void DirectOutput::EnumerateDevices()
    {
        m_Devices.clear();
        DirectOutput_RegisterDeviceCallback(*Device_Callback, this);
        DirectOutput_Enumerate(*Enumerate_Callback, this);
    }

    DirectOutput::~DirectOutput()
    {
        DirectOutput_Deinitialize();
    }

    size_t DirectOutput::GetNumDevices() const { return m_Devices.size(); }
    DirectOutput::Device DirectOutput::GetDevice(unsigned int i)
    {
        if (i >= m_Devices.size() || !m_Devices[i])
            throw std::runtime_error("Invalid device index.");
        return Device(m_Devices[i]);
    }


    DirectOutput::Device::Device(void* ptr) : DevicePtr(ptr)
    {

    }

    bool DirectOutput::Device::AddPage(unsigned int Page)
    {
        if (!DevicePtr) return false;

        std::wstringstream ss;
        ss << Page;
        auto str = ss.str();

        return DirectOutput_AddPage(DevicePtr, Page, str.c_str(), FLAG_SET_AS_ACTIVE) == S_OK;
    }

    bool DirectOutput::Device::SetLED(unsigned int Page, LEDs led, Colors color)
    {
        if (!DevicePtr) return false;

        DWORD index;

        if (led == LEDs::Fire)
        {
            index = NativeLEDs::LED_FIRE;
            return DirectOutput_SetLed(DevicePtr, Page, index, color != Colors::Off ? 1 : 0) == S_OK;
        }
        else if (led == LEDs::Throttle)
        {
            index = NativeLEDs::LED_THROTTLE;
            return DirectOutput_SetLed(DevicePtr, Page, index, color != Colors::Off ? 1 : 0) == S_OK;
        }
        else if (led == LEDs::A)
            index = LED_FIRE_A_RED;
        else if (led == LEDs::B)
            index = LED_FIRE_B_RED;
        else if (led == LEDs::D)
            index = LED_FIRE_D_RED;
        else if (led == LEDs::E)
            index = LED_FIRE_E_RED;
        else if (led == LEDs::Toggle1)
            index = LED_TOGGLE_1_2_RED;
        else if (led == LEDs::Toggle2)
            index = LED_TOGGLE_3_4_RED;
        else if (led == LEDs::Toggle3)
            index = LED_TOGGLE_5_6_RED;
        else if (led == LEDs::POV2)
            index = LED_POV_2_RED;
        else if (led == LEDs::Clutch)
            index = LED_CLUTCH_RED;
        else
            return false;

        return DirectOutput_SetLed(DevicePtr, Page, index, color == Colors::Red || color == Colors::Amber ? 1 : 0) == S_OK &&
            DirectOutput_SetLed(DevicePtr, Page, index + 1, color == Colors::Green || color == Colors::Amber ? 1 : 0) == S_OK;
    }

    bool DirectOutput::Device::SetText(unsigned int Page, MFD_Line line, std::wstring text)
    {
        if (!DevicePtr) return false;
        return DirectOutput_SetString(DevicePtr, Page, (DWORD)line, (DWORD)text.size(), text.data()) == S_OK;
    }
}

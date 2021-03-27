#pragma once

#include "JoyfulStick.h"
#include <vector>
#include <iostream>

#include <windows.h>
#include <deque>

namespace JoyfulStick
{
    class DirectOutput
    {
    public:
        JOYFULSTICKEXPORT DirectOutput();
        JOYFULSTICKEXPORT ~DirectOutput();

        enum class LEDs
        {
            Fire,
            A,
            B,
            D,
            E,
            Toggle1,
            Toggle2,
            Toggle3,
            POV2,
            Clutch,
            Throttle,
            Count
        };
        enum class Colors
        {
            Off,
            Red,
            Green,
            Amber
        };
        enum class MFD_Line
        {
            _0,
            _1,
            _2
        };

        class Device
        {
        public:
            Device(void*);
            Device(const Device&) = delete;
            Device(Device&&) = default;
            Device& operator=(const Device&) = delete;
            Device& operator=(Device&&) = delete;

            JOYFULSTICKEXPORT bool AddPage(unsigned int Page);
            JOYFULSTICKEXPORT bool SetLED(unsigned int Page, LEDs led, Colors color);
            JOYFULSTICKEXPORT bool SetText(unsigned int Page, MFD_Line line, std::wstring text);

        private:
            void* DevicePtr = nullptr;
        };

        JOYFULSTICKEXPORT void EnumerateDevices();
        JOYFULSTICKEXPORT size_t GetNumDevices() const;
        JOYFULSTICKEXPORT Device GetDevice(unsigned int i);


    private:
        std::deque<void*> m_Devices;

        static void __stdcall Device_Callback(void* hDevice, bool bAdded, void* pvContext);
        static void __stdcall Enumerate_Callback(void* hDevice, void* pvContext);
    };
}

#pragma once

#include "DirectOutput.h"

namespace JoyfulStickWrapper
{
    ref class DirectOutput;

	public ref class DirectOutputDevice
	{
	protected:
		DirectOutputDevice(JoyfulStick::DirectOutput::Device& native);
    public:
        ~DirectOutputDevice();

        enum class LEDs
        {
            Fire = (int)JoyfulStick::DirectOutput::LEDs::Fire,
            A = (int)JoyfulStick::DirectOutput::LEDs::A,
            B = (int)JoyfulStick::DirectOutput::LEDs::B,
            D = (int)JoyfulStick::DirectOutput::LEDs::D,
            E = (int)JoyfulStick::DirectOutput::LEDs::E,
            Toggle1 = (int)JoyfulStick::DirectOutput::LEDs::Toggle1,
            Toggle2 = (int)JoyfulStick::DirectOutput::LEDs::Toggle2,
            Toggle3 = (int)JoyfulStick::DirectOutput::LEDs::Toggle3,
            POV2 = (int)JoyfulStick::DirectOutput::LEDs::POV2,
            Clutch = (int)JoyfulStick::DirectOutput::LEDs::Clutch,
            Throttle = (int)JoyfulStick::DirectOutput::LEDs::Throttle,
            Count = (int)JoyfulStick::DirectOutput::LEDs::Count
        };
        enum class Colors
        {
            Off = (int)JoyfulStick::DirectOutput::Colors::Off,
            Red = (int)JoyfulStick::DirectOutput::Colors::Red,
            Green = (int)JoyfulStick::DirectOutput::Colors::Green,
            Amber = (int)JoyfulStick::DirectOutput::Colors::Amber
        };
        enum class MFD_Line
        {
            _0 = (int)JoyfulStick::DirectOutput::MFD_Line::_0,
            _1 = (int)JoyfulStick::DirectOutput::MFD_Line::_1,
            _2 = (int)JoyfulStick::DirectOutput::MFD_Line::_2
        };

        bool AddPage(unsigned int Page);
		bool SetLED(unsigned int Page, LEDs led, Colors color);
        bool SetText(unsigned int Page, MFD_Line line, System::String^ text);

	private:
        JoyfulStick::DirectOutput::Device* m_Imp = nullptr;
	};

	public ref class DirectOutput
	{
	public:
		DirectOutput();
        ~DirectOutput();

        void EnumerateDevices();
		size_t GetNumDevices();
		DirectOutputDevice^ GetDevice(unsigned int i);

	private:
        JoyfulStick::DirectOutput* m_Imp;
	};
}
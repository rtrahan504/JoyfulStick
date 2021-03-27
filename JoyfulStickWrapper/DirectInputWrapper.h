#pragma once

#include "DirectInput.h"
#include <memory>

namespace JoyfulStickWrapper
{
	ref class DirectInput;

	public ref class Joystick
	{
	public:
		Joystick(JoyfulStick::Joystick& native, DirectInput^ parent);
		~Joystick();

		System::String^ GetName();
		void Acquire();
		void Release();

		unsigned int GetNumAxes();
		unsigned int GetNumPOVs();
		unsigned int GetNumButtons();

		System::String^ GetAxisName(unsigned int i);
		System::String^ GetPOVName(unsigned int i);
		System::String^ GetButtonName(unsigned int i);

		double GetAxisValue(unsigned int i);
		long GetPOVValue(unsigned int i);
		bool GetButtonValue(unsigned int i);

		bool Poll();

	private:
		JoyfulStick::Joystick* m_Imp = nullptr;
		DirectInput^ m_Parent;
	};

	public ref class DirectInput
	{
	public:
		DirectInput();
		~DirectInput();

		void EnumerateJoysticks();
		size_t GetNumJoysticks();
		Joystick^ GetJoystick(unsigned int i);

	private:
		JoyfulStick::DirectInput* m_Imp;
	};
}
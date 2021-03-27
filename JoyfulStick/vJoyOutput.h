#pragma once

#include "JoyfulStick.h"
#include <stdio.h>
#include <tchar.h>

#include <windows.h>
#include <basetyps.h>
#include <cfgmgr32.h>
#include <Setupapi.h>
#include <strsafe.h>
#include <Newdev.h>
#include <INITGUID.H>

#include "Include/public.h"
#include <unordered_map>
#include <deque>

namespace JoyfulStick
{
	class VJoy;

	class VJoyDevice
	{
	public:
		JOYFULSTICKEXPORT static unsigned int GetNumDevices();

		VJoyDevice(unsigned int Index);
		VJoyDevice(VJoyDevice&&) = default;
		VJoyDevice(const VJoyDevice&) = delete;
		VJoyDevice& operator=(VJoyDevice&&) = delete;
		VJoyDevice& operator=(const VJoyDevice&) = delete;
		~VJoyDevice();

		JOYFULSTICKEXPORT void SetX(double val);
		JOYFULSTICKEXPORT void SetY(double val);
		JOYFULSTICKEXPORT void SetZ(double val);

		JOYFULSTICKEXPORT void SetRX(double val);
		JOYFULSTICKEXPORT void SetRY(double val);
		JOYFULSTICKEXPORT void SetRZ(double val);

		JOYFULSTICKEXPORT void SetSlider(double val);

		JOYFULSTICKEXPORT void SetAxisValue(double val, unsigned int i);
		JOYFULSTICKEXPORT void SetPOVValue(long val, unsigned int i);
		JOYFULSTICKEXPORT void SetButtonValue(bool val, unsigned int i);


		JOYFULSTICKEXPORT double GetX() const;
		JOYFULSTICKEXPORT double GetY() const;
		JOYFULSTICKEXPORT double GetZ() const;

		JOYFULSTICKEXPORT double GetRX() const;
		JOYFULSTICKEXPORT double GetRY() const;
		JOYFULSTICKEXPORT double GetRZ() const;

		JOYFULSTICKEXPORT double GetSlider() const;

		JOYFULSTICKEXPORT double GetAxisValue(unsigned int i) const;
		JOYFULSTICKEXPORT long GetPOVValue(unsigned int i) const;
		JOYFULSTICKEXPORT bool GetButtonValue(unsigned int i) const;

		JOYFULSTICKEXPORT unsigned int GetNumAxes() const;
		JOYFULSTICKEXPORT unsigned int GetNumPOVs() const;
		JOYFULSTICKEXPORT unsigned int GetNumButtons() const;

		JOYFULSTICKEXPORT void CommitChanges();

	private:
		const unsigned int Index;
		JOYSTICK_POSITION_V2 m_State;
	};
}

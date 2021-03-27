#pragma once

#include "vJoyOutput.h"

namespace JoyfulStickWrapper
{
	public ref class VJoyDevice
	{
	public:
		static unsigned int GetNumDevices();

		VJoyDevice(unsigned int i);
		~VJoyDevice();

		void SetX(double val);
		void SetY(double val);
		void SetZ(double val);

		void SetRX(double val);
		void SetRY(double val);
		void SetRZ(double val);

		void SetSlider(double val);

		void SetAxisValue(double val, unsigned int i);
		void SetPOVValue(long val, unsigned int i);
		void SetButtonValue(bool val, unsigned int i);


		double GetX();
		double GetY();
		double GetZ();

		double GetRX();
		double GetRY();
		double GetRZ();

		double GetSlider();

		double GetAxisValue(unsigned int i);
		long GetPOVValue(unsigned int i);
		bool GetButtonValue(unsigned int i);

		unsigned int GetNumAxes();
		unsigned int GetNumButtons();
		unsigned int GetNumPOVs();

		void CommitChanges();

	private:
		JoyfulStick::VJoyDevice* m_Imp = nullptr;
	};
}
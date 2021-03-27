#include "vJoyOutput.h"
#include "Include/vjoyinterface.h"
#include <stdexcept>


namespace JoyfulStick
{
	unsigned int VJoyDevice::GetNumDevices()
	{
		int ret = 0;
		GetNumberExistingVJD(&ret);
		return (unsigned int)ret;
	}

	VJoyDevice::VJoyDevice(unsigned int Index) :
		Index(Index)
	{
		memset(&m_State, 0, sizeof(m_State));

		m_State.bHats = -1;
		m_State.bHatsEx1 = -1;
		m_State.bHatsEx2 = -1;
		m_State.bHatsEx3 = -1;

		struct Enabler
		{
			Enabler()
			{
				if (!vJoyEnabled())
					throw std::runtime_error("VJoy is not enabled.");
			}
		};

		static Enabler enabler;

		if (!AcquireVJD(Index))
			throw std::runtime_error("An error occurred acquiring the VJoy device.");
	}
	VJoyDevice::~VJoyDevice()
	{
		RelinquishVJD(Index);
	}

	void VJoyDevice::SetX(double val) { m_State.wAxisX = (long)std::round((val + 1.0) * 16384.0); }
	void VJoyDevice::SetY(double val) { m_State.wAxisY = (long)std::round((val + 1.0) * 16384.0); }
	void VJoyDevice::SetZ(double val) { m_State.wAxisZ = (long)std::round((val + 1.0) * 16384.0); }

	void VJoyDevice::SetRX(double val) { m_State.wAxisXRot = (long)std::round((val + 1.0) * 16384.0); }
	void VJoyDevice::SetRY(double val) { m_State.wAxisYRot = (long)std::round((val + 1.0) * 16384.0); }
	void VJoyDevice::SetRZ(double val) { m_State.wAxisZRot = (long)std::round((val + 1.0) * 16384.0); }

	void VJoyDevice::SetSlider(double val) { m_State.wSlider = (long)std::round((val + 1.0) * 16384.0); }


	void VJoyDevice::SetAxisValue(double val, unsigned int i)
	{
		if (i == 0) SetX(val);
		else if (i == 1) SetY(val);
		else if (i == 2) SetZ(val);
		else if (i == 3) SetRX(val);
		else if (i == 4) SetRY(val);
		else if (i == 5) SetRZ(val);
		else if (i == 6) SetSlider(val);
	}

	void VJoyDevice::SetPOVValue(long val, unsigned int i)
	{
		if (val != -1)
			val = val * 100;

		if (i == 0)
			m_State.bHats = val;
		else if (i == 1)
			m_State.bHatsEx1 = val;
		else if (i == 2)
			m_State.bHatsEx2 = val;
		else if (i == 3)
			m_State.bHatsEx3 = val;
	}
	void VJoyDevice::SetButtonValue(bool val, unsigned int i)
	{
		if (val)
		{
			if (i < 32)
				m_State.lButtons |= 1 << i;
			else if (i < 64)
				m_State.lButtonsEx1 |= 1 << (i - 32);
			else if (i < 96)
				m_State.lButtonsEx2 |= 1 << (i - 64);
			else if (i < 128)
				m_State.lButtonsEx3 |= 1 << (i - 96);
		}
		else
		{
			if (i < 32)
				m_State.lButtons &= ~(1 << i);
			else if (i < 64)
				m_State.lButtonsEx1 &= ~(1 << (i - 32));
			else if (i < 96)
				m_State.lButtonsEx2 &= ~(1 << (i - 64));
			else if (i < 128)
				m_State.lButtonsEx3 &= ~(1 << (i - 96));
		}
	}


	double VJoyDevice::GetX() const { return (double)m_State.wAxisX / 16384.0 - 1.0; }
	double VJoyDevice::GetY() const { return (double)m_State.wAxisY / 16384.0 - 1.0; }
	double VJoyDevice::GetZ() const { return (double)m_State.wAxisZ / 16384.0 - 1.0; }

	double VJoyDevice::GetRX() const { return  (double)m_State.wAxisXRot / 16384.0 - 1.0; }
	double VJoyDevice::GetRY() const { return (double)m_State.wAxisYRot / 16384.0 - 1.0; }
	double VJoyDevice::GetRZ() const { return  (double)m_State.wAxisZRot / 16384.0 - 1.0; }

	double VJoyDevice::GetSlider() const { return  (double)m_State.wSlider / 16384.0 - 1.0; }

	double VJoyDevice::GetAxisValue(unsigned int i) const
	{
		if (i == 0) return GetX();
		else if (i == 1) return GetY();
		else if (i == 2) return GetZ();
		else if (i == 3) return GetRX();
		else if (i == 4) return GetRY();
		else if (i == 5) return GetRZ();
		else if (i == 6) return GetSlider();
		else return 0;
	}

	bool VJoyDevice::GetButtonValue(unsigned int i) const
	{
		if (i < 32)
			return m_State.lButtons & (1 << i);
		else if (i < 64)
			return m_State.lButtonsEx1 & (1 << (i - 32));
		else if (i < 96)
			return m_State.lButtonsEx2 & (1 << (i - 64));
		else if (i < 128)
			return m_State.lButtonsEx3 & (1 << (i - 96));
		else
			return false;
	}
	long VJoyDevice::GetPOVValue(unsigned int i) const
	{
		long val = -1;

		if (i == 0)
			val = m_State.bHats;
		else if (i == 1)
			val = m_State.bHatsEx1;
		else if (i == 2)
			val = m_State.bHatsEx2;
		else if (i == 3)
			val = m_State.bHatsEx3;

		return val == -1 ? val : val / 100;
	}

	unsigned int VJoyDevice::GetNumAxes() const
	{
		return 7;
	}
	unsigned int VJoyDevice::GetNumButtons() const
	{
		return GetVJDButtonNumber(Index);
	}

	unsigned int VJoyDevice::GetNumPOVs() const
	{
		return GetVJDContPovNumber(Index);
	}

	void VJoyDevice::CommitChanges()
	{
		if (!UpdateVJD(Index, &m_State))
			throw std::runtime_error("Error updating VJoy device.");
	}
}

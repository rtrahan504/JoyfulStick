#include "vJoyOutputWrapper.h"

namespace JoyfulStickWrapper
{
	VJoyDevice::VJoyDevice(unsigned int i) :
		m_Imp(new JoyfulStick::VJoyDevice(i))
	{}
	VJoyDevice::~VJoyDevice()
	{
		if (m_Imp) delete m_Imp;
	}

	void VJoyDevice::SetX(double val) { m_Imp->SetX(val); }
	void VJoyDevice::SetY(double val) { m_Imp->SetY(val); }
	void VJoyDevice::SetZ(double val) { m_Imp->SetZ(val); }

	void VJoyDevice::SetRX(double val) { m_Imp->SetRX(val); }
	void VJoyDevice::SetRY(double val) { m_Imp->SetRY(val); }
	void VJoyDevice::SetRZ(double val) { m_Imp->SetRZ(val); }

	void VJoyDevice::SetSlider(double val) { m_Imp->SetSlider(val); }

	void VJoyDevice::SetAxisValue(double val, unsigned int i) { m_Imp->SetAxisValue(val, i); }
	void VJoyDevice::SetPOVValue(long val, unsigned int i) { m_Imp->SetPOVValue(val, i); }
	void VJoyDevice::SetButtonValue(bool val, unsigned int i) { m_Imp->SetButtonValue(val, i); }


	double VJoyDevice::GetX() { return m_Imp->GetX(); }
	double VJoyDevice::GetY() { return m_Imp->GetY(); }
	double VJoyDevice::GetZ() { return m_Imp->GetZ(); }

	double VJoyDevice::GetRX() { return m_Imp->GetRX(); }
	double VJoyDevice::GetRY() { return m_Imp->GetRY(); }
	double VJoyDevice::GetRZ() { return m_Imp->GetRZ(); }

	double VJoyDevice::GetSlider() { return m_Imp->GetX(); }

	double VJoyDevice::GetAxisValue(unsigned int i) { return m_Imp->GetAxisValue(i); }
	long VJoyDevice::GetPOVValue(unsigned int i) { return m_Imp->GetPOVValue(i); }
	bool VJoyDevice::GetButtonValue(unsigned int i) { return m_Imp->GetButtonValue(i); }

	unsigned int VJoyDevice::GetNumAxes() { return m_Imp->GetNumAxes(); }
	unsigned int VJoyDevice::GetNumPOVs() { return m_Imp->GetNumPOVs(); }
	unsigned int VJoyDevice::GetNumButtons() { return m_Imp->GetNumButtons(); }

	void VJoyDevice::CommitChanges() { m_Imp->CommitChanges(); }

	unsigned int VJoyDevice::GetNumDevices()
	{
		return JoyfulStick::VJoyDevice::GetNumDevices();
	}
}
#include "DirectInputWrapper.h"

using namespace JoyfulStick;

namespace JoyfulStickWrapper
{
	Joystick::Joystick(JoyfulStick::Joystick& native, DirectInput^ parent) :
		m_Imp(new JoyfulStick::Joystick(std::move(native))), m_Parent(parent)
	{}
	Joystick::~Joystick()
	{
		if (m_Imp) delete m_Imp;
	}

	System::String^ Joystick::GetName() { return gcnew System::String(m_Imp->GetName().c_str()); }
	void Joystick::Acquire() { m_Imp->Acquire(); }
	void Joystick::Release() { m_Imp->Release(); }

	unsigned int Joystick::GetNumAxes() { return m_Imp->GetNumAxes(); }
	unsigned int Joystick::GetNumPOVs() { return m_Imp->GetNumPOVs(); }
	unsigned int Joystick::GetNumButtons() { return m_Imp->GetNumButtons(); }

	System::String^ Joystick::GetAxisName(unsigned int i) { return gcnew System::String(m_Imp->GetAxes()[i]->m_Inst.tszName); }
	System::String^ Joystick::GetPOVName(unsigned int i) { return gcnew System::String(m_Imp->GetPOVs()[i]->m_Inst.tszName); }
	System::String^ Joystick::GetButtonName(unsigned int i) { return gcnew System::String(m_Imp->GetButtons()[i]->m_Inst.tszName); }

	double Joystick::GetAxisValue(unsigned int i) { return m_Imp->GetAxisValue(i); }
	long Joystick::GetPOVValue(unsigned int i) { return m_Imp->GetPOV(i); }
	bool Joystick::GetButtonValue(unsigned int i) { return m_Imp->GetButton(i); }

	bool Joystick::Poll() { return m_Imp->Poll(); }

	DirectInput::DirectInput()
	{
		m_Imp = new ::DirectInput();
	}
	DirectInput::~DirectInput()
	{
		if(m_Imp) delete m_Imp;
	}

	void DirectInput::EnumerateJoysticks()
	{
		if (m_Imp) m_Imp->EnumerateJoysticks();
	}
	size_t DirectInput::GetNumJoysticks()
	{
		return m_Imp ? m_Imp->GetNumJoysticks() : 0;
	}
	Joystick^ DirectInput::GetJoystick(unsigned int i)
	{
		if (m_Imp && i < m_Imp->GetNumJoysticks())
			return gcnew Joystick(m_Imp->GetJoystick(i), this);
		else
			return nullptr;
	}
}
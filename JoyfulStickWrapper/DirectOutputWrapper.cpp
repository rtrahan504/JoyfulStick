#include "DirectOutputWrapper.h"
#include <msclr\marshal.h>
#include <msclr\marshal_cppstd.h>

namespace JoyfulStickWrapper
{
	DirectOutputDevice::DirectOutputDevice(JoyfulStick::DirectOutput::Device& native) :
		m_Imp(new JoyfulStick::DirectOutput::Device(std::move(native)))
	{}
	DirectOutputDevice::~DirectOutputDevice()
	{
		if (m_Imp) delete m_Imp;
	}

	bool DirectOutputDevice::AddPage(unsigned int Page)
	{
		return m_Imp->AddPage(Page);
	}
	bool DirectOutputDevice::SetLED(unsigned int Page, LEDs led, Colors color)
	{
		return m_Imp->SetLED(Page, static_cast<JoyfulStick::DirectOutput::LEDs>(led), static_cast<JoyfulStick::DirectOutput::Colors>(color));
	}
	bool DirectOutputDevice::SetText(unsigned int Page, DirectOutputDevice::MFD_Line line, System::String^ text)
	{
		return m_Imp->SetText(Page, static_cast<JoyfulStick::DirectOutput::MFD_Line>(line), msclr::interop::marshal_as<std::wstring>(text));
	}

	DirectOutput::DirectOutput() : m_Imp(new JoyfulStick::DirectOutput())
	{}
	DirectOutput::~DirectOutput()
	{
		if (m_Imp) delete m_Imp;
	}
	void DirectOutput::EnumerateDevices() { m_Imp->EnumerateDevices(); }
	size_t DirectOutput::GetNumDevices() { return m_Imp->GetNumDevices(); }

	public ref class DirectOutputDevice2 : DirectOutputDevice
	{
	public:
		DirectOutputDevice2(JoyfulStick::DirectOutput::Device& native) : DirectOutputDevice(native)
		{}
	};

	DirectOutputDevice^ DirectOutput::GetDevice(unsigned int i)
	{
		return gcnew DirectOutputDevice2(m_Imp->GetDevice(i));
	}
}
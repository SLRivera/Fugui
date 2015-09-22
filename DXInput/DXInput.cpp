#include "DXInput.h"
#include "D3DUtils.h"

using namespace Fugui;

DXInput		input;
DXInput* g_input = &input;

DXInput::DXInput() :
m_keyboardBuffSize(0),
m_mouseBuffSize(0),
mDInput(NULL),
mKeyDevice(NULL),
mMouseDevice(NULL),
m_x(0),
m_y(0),
m_fSensitivity(1.0f)
{
}

DXInput::~DXInput()
{
}

void DXInput::init(HWND hwnd, HINSTANCE hinst, DWORD keyCoopFlags, DWORD mouseCoopFlags)
{
	HR(DirectInput8Create(hinst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mDInput, NULL));

	HR(mDInput->CreateDevice(GUID_SysKeyboard, &mKeyDevice, NULL));
	HR(mKeyDevice->SetDataFormat(&c_dfDIKeyboard));
	HR(mKeyDevice->SetCooperativeLevel(hwnd, keyCoopFlags));

	HR(mDInput->CreateDevice(GUID_SysMouse, &mMouseDevice, NULL));
	HR(mMouseDevice->SetDataFormat(&c_dfDIMouse2));
	HR(mMouseDevice->SetCooperativeLevel(hwnd, mouseCoopFlags));

	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = 10;
	HR(mKeyDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph));
	HR(mMouseDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph));

	HR(mKeyDevice->Acquire());
	HR(mMouseDevice->Acquire());

	RECT winSize;
	GetWindowRect(hwnd, &winSize);
	m_windowHeight = 768;// winSize.bottom - 100;
	m_windowWidth = 1024;// winSize.right - 100;

	return;
}

void DXInput::shutdown()
{
	mMouseDevice->Unacquire();
	mKeyDevice->Unacquire();
	SafeRelease(mMouseDevice);
	SafeRelease(mKeyDevice);
	SafeRelease(mDInput);
}

void DXInput::pollInstant()
{
	if (FAILED(mKeyDevice->GetDeviceState(sizeof(mKeyState), (LPVOID)&mKeyState))){
		ZeroMemory(mKeyState, sizeof(mKeyState));
		mKeyDevice->Acquire();
	}

	if (FAILED(mMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &mMouseState))) {
		ZeroMemory(&mMouseState, sizeof(mMouseState));
		mMouseDevice->Acquire();
	}

	m_x += int(mMouseState.lX/* * m_fSensitivity*/);
	m_y += int(mMouseState.lY /** m_fSensitivity*/);

	// TODO: HACK, clamp to the actual window size
	if (m_x < 0)
		m_x = 0;
	if (m_x > 992)
		m_x = 992;
	if (m_y < 0)
		m_y = 0;
	if (m_y > 736)
		m_y = 736;

	return;
}

void DXInput::pollBuffered()
{
	m_keyboardBuffSize = 10;
	if (FAILED(mKeyDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_keyboardDataBuffer, &m_keyboardBuffSize, NULL))) {
		ZeroMemory(m_keyboardDataBuffer, sizeof(m_keyboardDataBuffer));
		mKeyDevice->Acquire();
	}
	m_mouseBuffSize = 10;
	if (FAILED(mMouseDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_mouseDataBuffer, &m_mouseBuffSize, NULL))) {
		ZeroMemory(&m_mouseDataBuffer, sizeof(m_mouseDataBuffer));
		mMouseDevice->Acquire();
	}

	m_x += int(mMouseState.lX /** m_fSensitivity*/);
	m_y += int(mMouseState.lY /** m_fSensitivity*/);

	// TODO: HACK, clamp to the actual window size
	if (m_x < 0)
		m_x = 0;
	if (m_x > 992)
		m_x = 992;
	if (m_y < 0)
		m_y = 0;
	if (m_y > 736)
		m_y = 736;

	return;
}

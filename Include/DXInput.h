#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#define LT_MOUSE_BTN	0
#define RT_MOUSE_BTN	1
#define	MD_MOUSE_BTN	2
#define ALT1_MOUSE_BTN	3
#define ALT2_MOUSE_BTN	4

namespace Fugui
{
	class DXInput
	{
	public:
		void	init(HWND hwnd, HINSTANCE hinst, DWORD keyCoopFlags, DWORD mouseCoopFlags);
		void	pollInstant();
		void	pollBuffered();
		inline void	pollBothTypes();
		void	shutdown();

		inline void getMousPos(int& x, int& y);
		inline int getMousePosX();
		inline int getMousePosY();
		inline void setSensitivity(float s);
		inline bool keyDown(int key);
		inline bool keyDownOnce(DWORD key);
		inline bool mouseBtnDown(int btn);
		inline bool mouseBtnDownOnce(int btn);
		inline float mouseMoveX();
		inline float mouseMoveY();
		inline float mouseMoveZ();

		DXInput();
		~DXInput();

	private:
		DIMOUSESTATE2				mMouseState;
		DIDEVICEOBJECTDATA		m_keyboardDataBuffer[10];
		DIDEVICEOBJECTDATA		m_mouseDataBuffer[10];

		DWORD		m_keyboardBuffSize;
		DWORD		m_mouseBuffSize;

		char					mKeyState[256];

		float	m_fSensitivity;

		int		m_x;
		int		m_y;
		int		m_windowWidth;
		int		m_windowHeight;
		
		IDirectInput8*		mDInput;

		IDirectInputDevice8*	mKeyDevice;
		IDirectInputDevice8*	mMouseDevice;

		// singleton, sort of...
		DXInput(const DXInput&);
		DXInput& operator=(const DXInput&);
	};

	void DXInput::getMousPos(int& x, int& y) { x = m_x; y = m_y; }
	void DXInput::setSensitivity(float s)	{ m_fSensitivity = s; }
	int DXInput::getMousePosX()	{ return m_x; }
	int DXInput::getMousePosY()	{ return m_y; }	

	bool DXInput::keyDown(int key)
	{
		return 0 != (mKeyState[key] & 0x80);
	}

	bool DXInput::mouseBtnDown(int btn)
	{
		return 0 != (mMouseState.rgbButtons[btn] & 0x80);
	}

	float DXInput::mouseMoveX()
	{
		return (float)mMouseState.lX; 
	}

	float DXInput::mouseMoveY()
	{
		return (float)mMouseState.lY;
	}

	float DXInput::mouseMoveZ()
	{
		return (float)mMouseState.lZ;
	}

	void DXInput::pollBothTypes()
	{
		pollInstant();
		pollBuffered();
	}

	inline bool DXInput::keyDownOnce(DWORD key)
	{
		for (DWORD i = 0; i < m_keyboardBuffSize; ++i)
			if (m_keyboardDataBuffer[i].dwOfs == key)
				if ((m_keyboardDataBuffer[i].dwData & 0x80) != 0)
					return true;

		return false;
	}

	inline bool DXInput::mouseBtnDownOnce(int btn)
	{
		for (DWORD i = 0; i < m_mouseBuffSize; ++i)
			if (m_mouseDataBuffer[i].dwOfs == btn)
				if ((m_mouseDataBuffer[i].dwData & 0x80) != 0)
					return true;

		return false;
	}

} // end namespace Fugui

extern Fugui::DXInput* g_input;
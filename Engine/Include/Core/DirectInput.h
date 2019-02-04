#pragma once

#include "../PGEngine.h"
#include <dinput.h>

PG_BEGIN

class PG_DLL CDirectInput
{
public:
	bool Init(HINSTANCE hinstance, HWND hWnd, int screenWidth, int screenHeight, bool bOnMouseRenderer = true);
	void Shutdown();
	bool Update(float fTime);

	bool IsEscapePressed();
	const POINT& GetMouseLocation()
	{
		return m_mousePos;
	}

private:
	bool UpdateKeyboard();
	bool UpdateMouse();
	void ProcessInput();

private:
	IDirectInput8*			m_directInput = nullptr;
	IDirectInputDevice8*	m_keyboard = nullptr;
	IDirectInputDevice8*	m_mouse = nullptr;

private:
	unsigned char m_keyboardState[256];
	DIMOUSESTATE  m_mouseState;

	int m_screenWidth, m_screenHeight = 0;
	POINT m_mousePos = { 0, 0 };

	DECLARE_SINGLE(CDirectInput)
};

PG_END
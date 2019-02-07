#include "DirectInput.h"

PG_USING

DEFINITION_SINGLE(CDirectInput)

CDirectInput::CDirectInput() :
	m_mousePos{ 0, 0 },
	m_screenWidth(0),
	m_screenHeight(0)
{
	m_directInput = nullptr;
	m_keyboard = nullptr;
	m_mouse = nullptr;
}


CDirectInput::~CDirectInput()
{
	Shutdown();
}

bool CDirectInput::Init(HINSTANCE hinstance, HWND hWnd, int screenWidth, int screenHeight, bool bOnMouseRenderer)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Direct Input 초기화
	if (FAILED(DirectInput8Create(hinstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_directInput, NULL)))
	{
		return false;
	}

	// 키보드 인터페이스 생성
	if (FAILED(m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL)))
	{
		return false;
	}

	// 키보드 데이터 형식 설정
	if (FAILED(m_keyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return false;
	}

	// 다른 프로그램과 공유하지 않도록 키보드 협조 수준 설정
	if (FAILED(m_keyboard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
	{
		return false;
	}

	// 키보드 할당
	if (FAILED(m_keyboard->Acquire()))
	{
		return false;
	}
	
	// 마우스 인터페이스 생성
	if (FAILED(m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL)))
	{
		return false;
	}

	// 마우스 데이터 형식 설정
	if (FAILED(m_mouse->SetDataFormat(&c_dfDIMouse)))
	{
		return false;
	}

	// 다른 프로그램과 공유하지 않도록 마우스 협조 수준 설정
	if (FAILED(m_mouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
	{
		return false;
	}

	if (FAILED(m_mouse->Acquire()))
	{
		return false;
	}

	return true;
}

void CDirectInput::Shutdown()
{
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = nullptr;
	}

	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = nullptr;
	}

	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = nullptr;
	}
}

bool CDirectInput::Update(float fTime)
{
	if (!UpdateKeyboard())
	{
		return false;
	}

	if (!UpdateMouse())
	{
		return false;
	}

	ProcessInput();

	return true;
}

bool CDirectInput::UpdateKeyboard()
{
	HRESULT result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// 키보드 컨트롤을 잃으면 다시 가져옴
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool CDirectInput::UpdateMouse()
{
	HRESULT result = m_keyboard->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		// 키보드 컨트롤을 잃으면 다시 가져옴
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool CDirectInput::IsEscapePressed()
{
	// 종료 함수
	// return false;

	return true;
}


void CDirectInput::ProcessInput()
{
	m_mousePos.x += m_mouseState.lX;
	m_mousePos.y += m_mouseState.lY;

	if (m_mousePos.x < 0) { m_mousePos.x = 0; }
	if (m_mousePos.y < 0) { m_mousePos.y = 0; }

	if (m_mousePos.x > m_screenWidth) { m_mousePos.x = m_screenWidth; }
	if (m_mousePos.y > m_screenHeight) { m_mousePos.y = m_screenHeight; }
}

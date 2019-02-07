#pragma once

#include "PGEngine.h"

PG_BEGIN

class PG_DLL CCore
{
private:
	static bool	m_bLoop;

private:
	HINSTANCE		m_hInst;
	HWND			m_hWnd;
	RESOLUTION		m_tResolution;

public:
	HWND GetWindowHandle()	const;

public:
	bool Init(HINSTANCE hInst, TCHAR* pTitle, TCHAR* pClass, int iIconID,
		UINT iWidth, UINT iHeight, bool bWindowMode, bool bOnMouseRenderer = true);
	bool Init(HINSTANCE hInst, HWND hWnd, UINT iWidth,
		UINT iHeight, bool bWindowMode, bool bOnMouseRenderer = true, bool bDirectInput = true);
	int Run();
	int RunTool();

private:
	void Logic();
	void Input(float fTime);
	int Update(float fTime);
	int LateUpdate(float fTime);
	void Collision(float fTime);
	void Render(float fTime);

private:
	ATOM WindowRegisterClass(TCHAR* pClass, int iIconID);
	BOOL InitWindow(TCHAR* pClass, TCHAR* pTitle, UINT iWidth, UINT iHeight);

public:
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	DECLARE_SINGLE(CCore)
};

PG_END

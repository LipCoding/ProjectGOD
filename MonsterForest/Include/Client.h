
#pragma once

#include "resource.h"
#include "PGEngine.h"

#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib, "Engine64_Debug")
#else
#pragma comment(lib, "Engine64")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "Engine_Debug")
#else
#pragma comment(lib, "Engine")
#endif
#endif // _DEBUG

#include "SceneScript/MainScene.h"
#include "SceneScript/LoginScene.h"

static LoginScene*	pLoginScene = NULL;
static CMainScene* pMainScene = NULL;

LRESULT clientProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
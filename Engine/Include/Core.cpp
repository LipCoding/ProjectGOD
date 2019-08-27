#include "Core.h"
#include "Device.h"
#include "Resources\ResourcesManager.h"
#include "Rendering\RenderManager.h"
#include "Core\PathManager.h"
#include "Resources\Mesh.h"
#include "Rendering\Shader.h"
#include "Rendering\ShaderManager.h"
#include "Core\Timer.h"
#include "Core\TimerManager.h"
#include "Scene\SceneManager.h"
#include "Core\Input.h"
#include "Core\DirectInput.h"
#include "Core\CollisionManager.h"
#include "GameObject\GameObject.h"
#include "Core\Scheduler.h"
#include "Core\NavigationManager.h"
#include "Core\QuadTreeManager.h"
#include "Core/SoundManager.h"
#include "Core\NaviManager.h"
#include "Core\NaviMesh.h"

PG_USING

DEFINITION_SINGLE(CCore)

bool CCore::m_bLoop = true;
bool CCore::isFocus = true;

CCore::CCore()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// 번호를 입력하면 릭이 남는 곳에서 터진다.
	//_CrtSetBreakAlloc(179728);

#ifdef _DEBUG
	//AllocConsole();
#endif
	AllocConsole();
}

CCore::~CCore()
{
	DESTROY_SINGLE(CScheduler);
	DESTROY_SINGLE(CSceneManager);
	DESTROY_SINGLE(CNavigationManager);
	DESTROY_SINGLE(CRenderManager);
	DESTROY_SINGLE(CResourcesManager);
	DESTROY_SINGLE(CInput);
	DESTROY_SINGLE(CDirectInput);
	DESTROY_SINGLE(CCollisionManager);
	DESTROY_SINGLE(CTimerManager);
	DESTROY_SINGLE(CPathManager);
	DESTROY_SINGLE(CDevice);
	DESTROY_SINGLE(CQuadTreeManager);
	DESTROY_SINGLE(CNaviManager);

//#ifdef _DEBUG
	FreeConsole();
//#endif // _DEBUG
}

HWND CCore::GetWindowHandle() const
{
	return m_hWnd;
}

bool CCore::Init(HINSTANCE hInst, TCHAR * pTitle, TCHAR * pClass, int iIconID,
	UINT iWidth, UINT iHeight, bool bWindowMode, bool bOnMouseRenderer, bool bDirectInput)
{
	m_hInst = hInst;

	WindowRegisterClass(pClass, iIconID);

	InitWindow(pClass, pTitle, iWidth, iHeight);

	return Init(hInst, m_hWnd, iWidth, iHeight, bWindowMode, bOnMouseRenderer, bDirectInput);
}

bool CCore::Init(HINSTANCE hInst, HWND hWnd, UINT iWidth, 
	UINT iHeight, bool bWindowMode, bool bOnMouseRenderer, bool bDirectInput)
{
	m_tResolution.iWidth = iWidth;
	m_tResolution.iHeight = iHeight;
	m_hInst = hInst;
	m_hWnd = hWnd;

	if (!GET_SINGLE(CDevice)->Init(m_hWnd, iWidth, iHeight, bWindowMode))
		return false;

	if (!GET_SINGLE(CScheduler)->Init())
		return false;

	if (!GET_SINGLE(CPathManager)->Init())
		return false;

	if (!GET_SINGLE(CResourcesManager)->Init())
		return false;

	if (!GET_SINGLE(CRenderManager)->Init())
		return false;

	// Direct Input으로 수정
	if (bDirectInput)
	{
		if (!GET_SINGLE(CDirectInput)->Init(m_hInst, m_hWnd, iWidth, iHeight))
			return false;
	}
	else
	{
		if (!GET_SINGLE(CInput)->Init(m_hWnd, bOnMouseRenderer))
			return false;
	}

	if (!GET_SINGLE(CTimerManager)->Init())
		return false;

	if (!GET_SINGLE(CCollisionManager)->Init())
		return false;

	if (!GET_SINGLE(CSceneManager)->Init())
		return false;

	if (!GET_SINGLE(SoundManager)->Init())
		return false;

	if (isFocus)
	{
		CGameObject*	pMouseObj = GET_SINGLE(CInput)->GetMouseObj();
		pMouseObj->SetScene(GET_SINGLE(CSceneManager)->GetCurrentScene());
		SAFE_RELEASE(pMouseObj);
	}

	ShowCursor(FALSE);

	return true;
}

int CCore::Run()
{
	GET_SINGLE(CSceneManager)->Start();
	MSG msg;

	while (m_bLoop)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// 윈도우 데드타임일 때는 else  구문으로 들어온다.
		else
		{
			Logic();
		}
	}

	return (int)msg.wParam;
}

int CCore::RunTool()
{
	Logic();

	return 0;
}

void CCore::Logic()
{
	CTimer*	pTimer = GET_SINGLE(CTimerManager)->FindTimer("MainThread");

	pTimer->Update();

	float fTime = pTimer->GetDeltaTime();

	SAFE_RELEASE(pTimer);

	GET_SINGLE(CScheduler)->Update(fTime);

	if(isFocus)
		Input(fTime);
	if (Update(fTime) == SC_CHANGE)
	{
		if(isFocus)
			GET_SINGLE(CInput)->ClearWheel();
		return;
	}
	if (LateUpdate(fTime) == SC_CHANGE)
	{
		if(isFocus)
			GET_SINGLE(CInput)->ClearWheel();
		return;
	}
	Collision(fTime);
	Render(fTime);
	
	/*CShader*	pShader = GET_SINGLE(CShaderManager)->FindShader(STANDARD_COLOR_SHADER);
	CMesh*	pMesh = GET_SINGLE(CResourcesManager)->FindMesh("TriangleColor");

	GET_SINGLE(CShaderManager)->SetInputLayout("VertexColor");
	pShader->SetShader();
	pMesh->Render(0.f);

	SAFE_RELEASE(pMesh);
	SAFE_RELEASE(pShader);*/
}

// 수정
void CCore::Input(float fTime)
{
	if (isFocus)
	{
		GET_SINGLE(CInput)->Update(fTime);
		CGameObject*	pMouseObj = GET_SINGLE(CInput)->GetMouseObj();
		pMouseObj->Input(fTime);
		SAFE_RELEASE(pMouseObj);
		GET_SINGLE(CSceneManager)->Input(fTime);
	}
}

// 수정
int CCore::Update(float fTime)
{
	if (isFocus)
	{
		CGameObject*	pMouseObj = GET_SINGLE(CInput)->GetMouseObj();
		pMouseObj->Update(fTime);
		SAFE_RELEASE(pMouseObj);
	}
	GET_SINGLE(CQuadTreeManager)->CheckRenderingChild();
	return GET_SINGLE(CSceneManager)->Update(fTime);
}

// 수정
int CCore::LateUpdate(float fTime)
{
	if (isFocus)
	{
		CGameObject*	pMouseObj = GET_SINGLE(CInput)->GetMouseObj();
		pMouseObj->LateUpdate(fTime);
		SAFE_RELEASE(pMouseObj);
	}
	return GET_SINGLE(CSceneManager)->LateUpdate(fTime);
}

// 수정
void CCore::Collision(float fTime)
{
	/*CGameObject*	pMouseObj = GET_SINGLE(CInput)->GetMouseObj();
	pMouseObj->Collision(fTime);
	SAFE_RELEASE(pMouseObj);*/

	GET_SINGLE(CSceneManager)->Collision(fTime);

	GET_SINGLE(CCollisionManager)->Collision(fTime);

	// 절두체 컬링
}

// 수정
void CCore::Render(float fTime)
{
	GET_SINGLE(CDevice)->ClearTarget();

	GET_SINGLE(CSceneManager)->Render(fTime);

	GET_SINGLE(CRenderManager)->Render(fTime);
	
	GET_SINGLE(CNaviManager)->Render(fTime);
	if (isFocus)
	{
		CGameObject*	pMouseObj = GET_SINGLE(CInput)->GetMouseObj();
		pMouseObj->Render(fTime);
		SAFE_RELEASE(pMouseObj);
	}
	GET_SINGLE(CDevice)->Present();

	if (isFocus)
	{
		GET_SINGLE(CInput)->ClearWheel();
	}
}
#ifdef _QUEST_TOOL_
HWND CCore::getQuestToolWindowHandle()
{
	return quest_tool_hwnd;
}
#endif

HINSTANCE CCore::getWindowInstance()
{
	return m_hInst;
}

ATOM CCore::WindowRegisterClass(TCHAR * pClass, int iIconID)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = CCore::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInst;
	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(iIconID));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;// MAKEINTRESOURCEW(IDC_MY170825);
	wcex.lpszClassName = pClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(iIconID));

	return RegisterClassExW(&wcex);
}

BOOL CCore::InitWindow(TCHAR * pClass, TCHAR * pTitle, UINT iWidth, UINT iHeight)
{
	HWND hWnd = CreateWindowW(pClass, pTitle, WS_OVERLAPPEDWINDOW,
		300, 0, iWidth, iHeight, nullptr, nullptr, m_hInst, nullptr);
#ifdef _QUEST_TOOL_
	quest_tool_hwnd = CreateWindowW(pClass, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
		WS_CLIPCHILDREN,
		10, 10, 610, 610, hWnd, (HMENU)NULL, m_hInst, nullptr);
#endif
	if (!hWnd)
	{
		return FALSE;
	}

	// 윈도우의 클라이언트 영역을 원하는 크기로 설정하는 방법
	RECT	rc = { 0, 0, iWidth, iHeight };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	SetWindowPos(hWnd, HWND_TOPMOST, 300, 0, rc.right - rc.left, rc.bottom - rc.top,
		SWP_NOMOVE | SWP_NOZORDER);

	m_hWnd = hWnd;

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CCore::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_KEYDOWN:	// 키를 누를때 들어오는 메세지이다.
		switch (wParam)
		{
		}
		break;
	case WM_SETFOCUS:
		isFocus = true;
		break;
	case WM_KILLFOCUS:
		//isFocus = false;
		break;

	case WM_MOUSEWHEEL:
		if(isFocus)
			GET_SINGLE(CInput)->SetWheel(HIWORD(wParam));
		break;
	case WM_DESTROY:
		m_bLoop = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
void CCore::setWindowProc(LRESULT(*wndProc)(HWND, UINT, WPARAM, LPARAM))
{

	oldProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(this->m_hWnd, GWLP_WNDPROC, (LONG_PTR)wndProc));
}
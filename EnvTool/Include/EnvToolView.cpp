
// EnvToolView.cpp: CEnvToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "EnvTool.h"
#endif

#include "EnvToolDoc.h"
#include "EnvToolView.h"
#include "Core.h"
#include "GameObject/GameObject.h"
#include "Core/Input.h"
#include "Component/Transform.h"
#include "Device.h"
#include "Scene/Scene.h"
#include "Scene/Layer.h"
#include "Scene/SceneManager.h"
#include "Component/Terrain2D.h"
#include "Component/Tile.h"
#include "Core/Timer.h"
#include "Core/TimerManager.h"
#include "MainFrm.h"

//#include "EditForm.h"
//#include "TarrainEdit.h"

PG_USING

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEnvToolView

IMPLEMENT_DYNCREATE(CEnvToolView, CView)

BEGIN_MESSAGE_MAP(CEnvToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CEnvToolView 생성/소멸

CEnvToolView::CEnvToolView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CEnvToolView::~CEnvToolView()
{
	DESTROY_SINGLE(CCore);
}

BOOL CEnvToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CEnvToolView 그리기

void CEnvToolView::OnDraw(CDC* /*pDC*/)
{
	CEnvToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CEnvToolView 인쇄

BOOL CEnvToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CEnvToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CEnvToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CEnvToolView 진단

#ifdef _DEBUG
void CEnvToolView::AssertValid() const
{
	CView::AssertValid();
}

void CEnvToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEnvToolDoc* CEnvToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEnvToolDoc)));
	return (CEnvToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CEnvToolView 메시지 처리기


void CEnvToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CWnd * pWnd = AfxGetMainWnd();
	HWND hWnd = pWnd->m_hWnd;

		// 엔진 초기화
	if (!GET_SINGLE(CCore)->Init(AfxGetInstanceHandle(), hWnd, 1400, 800, true, false))
		return;
}


void CEnvToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	POINT	ptMouse = GET_SINGLE(CInput)->GetMousePos();
	CTimer* pTimer = GET_SINGLE(CTimerManager)->FindTimer("MainThread");

	float	fTime = pTimer->GetDeltaTime();

	SAFE_RELEASE(pTimer);

	// 타일의 색상을 바꿔준다.
	//SetTileColor(ptMouse.x, ptMouse.y);

	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();

	//if(ptMouse.x)

	SAFE_RELEASE(pScene);

	CView::OnMouseMove(nFlags, point);
}


void CEnvToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonDown(nFlags, point);
}

void CEnvToolView::SetTileColor(float x, float y)
{

}


int CEnvToolView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}

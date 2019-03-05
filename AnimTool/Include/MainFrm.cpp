﻿
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "AnimTool.h"

#include "MainFrm.h"
#include "AnimToolView.h"
#include "EditForm.h"

PG_USING

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame() noexcept
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
	m_pView = nullptr;
	m_pEdit = nullptr;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	//{
	//	TRACE0("도구 모음을 만들지 못했습니다.\n");
	//	return -1;      // 만들지 못했습니다.
	//}

	//if (!m_wndStatusBar.Create(this))
	//{
	//	TRACE0("상태 표시줄을 만들지 못했습니다.\n");
	//	return -1;      // 만들지 못했습니다.
	//}
	//m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
	/*m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);*/


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	// 창 크기 조정
	cs.cx = 1800;
	cs.cy = 900;

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	// 윈도우 스플릿 1행 2열로
	if (!m_SplitWnd.CreateStatic(this, 1, 2))
	{
		return FALSE;
	}

	// View의 사이즈에 맞게 생성
	if (!m_SplitWnd.CreateView(0, 0, RUNTIME_CLASS(CAnimToolView),
		CSize(1400, 900), pContext))
	{
		return FALSE;
	}

	// Form의 사이즈에 맞게 생성
	if (!m_SplitWnd.CreateView(0, 1, RUNTIME_CLASS(CEditForm),
		CSize(400, 900), pContext))
	{
		return FALSE;
	}

	m_pView = (CAnimToolView*)m_SplitWnd.GetPane(0, 0);
	m_pEdit = (CEditForm*)m_SplitWnd.GetPane(0, 1);

	return TRUE;
}

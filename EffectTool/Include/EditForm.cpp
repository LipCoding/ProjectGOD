// EditForm.cpp: 구현 파일
//

#include "stdafx.h"
#include "EffectTool.h"
#include "EditForm.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "EffectToolView.h"

// Tab
#include "EffectTab.h"

// EditForm


// CEditForm 대화 상자

IMPLEMENT_DYNCREATE(CEditForm, CFormView)

CEditForm::CEditForm()
	: CFormView(IDD_DIALOG_FORM)
	, m_pView(nullptr)
	, m_eTabType(TAB_END)
{
}

CEditForm::~CEditForm()
{
	m_pView = nullptr;
}

BEGIN_MESSAGE_MAP(CEditForm, CView)
	ON_WM_VSCROLL()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_EFFECT, &CEditForm::OnTcnSelchangeTabEffect)
END_MESSAGE_MAP()


void CEditForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_EFFECT, m_Tab);
	DDX_Control(pDX, IDC_LIST_EFFECT_CONTAINER, m_listEffectList);
}

#ifdef _DEBUG
void CEditForm::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CEditForm::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEditForm 메시지 처리기


void CEditForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	// 스크롤바
	this->EnableScrollBarCtrl(SB_VERT, TRUE);
	SCROLLINFO si;
	si.fMask = SIF_PAGE | SIF_RANGE;
	si.nPage  = 10;
	si.nMin = 0;
	si.nMax = 500;
	this->SetScrollInfo(SB_VERT, &si, TRUE);

	m_eTabType = TAB_EFFECT;

	m_Tab.InsertItem(0, _T("Effect1"));
	m_Tab.InsertItem(1, _T("Effect2"));
	m_Tab.InsertItem(2, _T("Effect3"));

	CRect rect;
	m_Tab.GetWindowRect(&rect);

	m_Tab.SetCurSel(0);

	m_pEffectDlg = new CEffectTab;
	m_pEffectDlg->Create(IDD_DIALOG1, &m_Tab);
	m_pEffectDlg->MoveWindow(0, 20, rect.Width(), rect.Height());
	m_pEffectDlg->ShowWindow(SW_SHOW);

	m_pEffect1Dlg = new CEffectTab;
	m_pEffect1Dlg->Create(IDD_DIALOG2, &m_Tab);
	m_pEffect1Dlg->MoveWindow(0, 20, rect.Width(), rect.Height());
	m_pEffect1Dlg->ShowWindow(SW_HIDE);

	// 메인 프레임을 받아온다.
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	m_pView = (CEnvToolView*)pMain->GetActiveView();

	m_eTabType = (TOOLTAB_TYPE)m_Tab.GetCurSel();
}

void CEditForm::OnTcnSelchangeTabEffect(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int sel = m_Tab.GetCurSel();

	switch (sel)
	{
	case 0:
		m_eTabType = TAB_EFFECT;

		m_pEffectDlg->ShowWindow(SW_SHOW);
		m_pEffect1Dlg->ShowWindow(SW_HIDE);
		
		break;
	case 1:
		m_eTabType = TAB_EFFECT1;

		m_pEffectDlg->ShowWindow(SW_HIDE);
		m_pEffect1Dlg->ShowWindow(SW_SHOW);

		break;
	}

	*pResult = 0;
}

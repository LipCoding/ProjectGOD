// EditForm.cpp: 구현 파일
//

#include "stdafx.h"
#include "AnimTool.h"
#include "EditForm.h"

#include "MainFrm.h"
#include "AnimToolView.h"

#include "AnimSpeedTab.h"

// CEditForm 대화 상자

IMPLEMENT_DYNCREATE(CEditForm, CFormView)

CEditForm::CEditForm()
	: CFormView(IDD_DIALOG_FORM)
{

}

CEditForm::~CEditForm()
{
	delete m_pAnimSpeedDlg;
	m_pAnimSpeedDlg = nullptr;
}

BEGIN_MESSAGE_MAP(CEditForm, CView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_ANIM, &CEditForm::OnTcnSelchangeTabAnim)
END_MESSAGE_MAP()

// EditForm 진단

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

void CEditForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_ANIM, m_Tab);
	DDX_Control(pDX, IDC_CHECK_PLAY, m_checkPlay);
}



void CEditForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_Tab.InsertItem(0, _T("Speed"));
	m_Tab.InsertItem(1, _T("Movement"));
	m_Tab.InsertItem(2, _T("Collider"));
	m_Tab.InsertItem(3, _T("OnOff"));
	m_Tab.InsertItem(4, _T("CamShake"));

	CRect rect;
	m_Tab.GetWindowRect(&rect);

	m_Tab.SetCurSel(0);

	m_pAnimSpeedDlg = new CAnimSpeedTab;
	m_pAnimSpeedDlg->Create(IDD_DIALOG_ANIM_SPEED, &m_Tab);
	m_pAnimSpeedDlg->MoveWindow(0, 20, rect.Width(), rect.Height());
	m_pAnimSpeedDlg->ShowWindow(SW_SHOW);

	// 메인 프레임을 받아온다.
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	m_pView = (CAnimToolView*)pMain->GetActiveView();
}


void CEditForm::OnTcnSelchangeTabAnim(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int sel = m_Tab.GetCurSel();

	switch (sel)
	{
	case 0:
		m_pAnimSpeedDlg->ShowWindow(SW_SHOW);
		break;
	case 1:
		m_pAnimSpeedDlg->ShowWindow(SW_HIDE);
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	}

	*pResult = 0;
}

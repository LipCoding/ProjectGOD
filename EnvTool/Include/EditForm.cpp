// EditForm.cpp: 구현 파일
//

#include "stdafx.h"
#include "EnvTool.h"
#include "EditForm.h"

#include "MainFrm.h"
#include "EnvToolView.h"

// Tab
#include "TerrainTab.h"
#include "ObjTab.h"

// EditForm

#include "Component/LandScape.h"
#include "Component/Collider.h"
#include "Component/ColliderAABB.h"

IMPLEMENT_DYNCREATE(CEditForm, CFormView)

CEditForm::CEditForm()
	: CFormView(IDD_DIALOG_FORM)
	, m_pTerrainDlg(nullptr)
	, m_pObjDlg(nullptr)
	, m_pView(nullptr)
	, m_eTabType(TAB_END)
{

}

CEditForm::~CEditForm()
{
	delete m_pTerrainDlg;
	m_pTerrainDlg = nullptr;
	delete m_pObjDlg;
	m_pObjDlg = nullptr;
	m_pView = nullptr;
	
}

BEGIN_MESSAGE_MAP(CEditForm, CView)
//	ON_WM_CREATE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CEditForm::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON_CAM_MAIN, &CEditForm::OnBnClickedButtonCamMain)
	ON_BN_CLICKED(IDC_BUTTON_CAM_LIGHT, &CEditForm::OnBnClickedButtonCamLight)
	ON_BN_CLICKED(IDC_CHECK_COLLIDER, &CEditForm::OnBnClickedCheckCollider)
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


// EditForm 메시지 처리기

void CEditForm::DoDataExchange(CDataExchange * pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
	DDX_Control(pDX, IDC_CHECK_COLLIDER, m_checkCollider);
}

//int CEditForm::OnCreate(LPCREATESTRUCT lpCreateStruct)
//{
//	if (CFormView::OnCreate(lpCreateStruct) == -1)
//		return -1;
//
//	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
//
//	return 0;
//}


void CEditForm::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	// Tab의 현재 위치를 가져온다.
	int sel = m_Tab.GetCurSel();

	switch (sel)
	{
	case 0:
		m_eTabType = TAB_TERRAIN;

		m_pTerrainDlg->Process_ShowTab();
		m_pTerrainDlg->ShowWindow(SW_SHOW);

		m_pObjDlg->Process_ChangeTab();
		m_pObjDlg->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_eTabType = TAB_OBJECT;
		m_pTerrainDlg->Process_ChangeTab();
		m_pTerrainDlg->ShowWindow(SW_HIDE);

		m_pObjDlg->Process_ShowTab();
		m_pObjDlg->ShowWindow(SW_SHOW);
		break;
	}

	*pResult = 0;
}


void CEditForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	// Tab Control에 사용할 탭을 추가
	m_Tab.InsertItem(0, _T("Terrain"));
	m_Tab.InsertItem(1, _T("Object"));
	m_Tab.InsertItem(2, _T("Light"));
	m_Tab.InsertItem(3, _T("Sky"));

	CRect rect;
	m_Tab.GetWindowRect(&rect);

	// 탭을 첫번째로 포커스 해놓음
	m_Tab.SetCurSel(0);

	// 탭 항목에 들어갈 대화상자를 동적 생성하여, 부모를 Tab Ctrl로 지정
	m_pTerrainDlg = new CTerrainTab;
	m_pTerrainDlg->Create(IDD_DIALOG1, &m_Tab);
	m_pTerrainDlg->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pTerrainDlg->ShowWindow(SW_SHOW);

	m_pObjDlg = new CObjTab;
	m_pObjDlg->Create(IDD_DIALOG2, &m_Tab);
	m_pObjDlg->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pObjDlg->ShowWindow(SW_HIDE);

	// 메인 프레임을 받아온다.
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	m_pView = (CEnvToolView*)pMain->GetActiveView();

	m_eTabType = (TOOLTAB_TYPE)m_Tab.GetCurSel();

	m_checkCollider.SetCheck(1);
}


void CEditForm::OnBnClickedButtonCamMain()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pView->SetMainCamera();
}


void CEditForm::OnBnClickedButtonCamLight()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pView->SetLightCamera();
}

void CEditForm::OnBnClickedCheckCollider()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int check = m_checkCollider.GetCheck();

	CGameObject* pLandScapeObj = CGameObject::FindObject("LandScape");

	if (pLandScapeObj)
	{
		CLandScape* pLandScape = pLandScapeObj->FindComponentFromTag<CLandScape>("LandScape");
		list<QUADTREENODE*>* nodes = pLandScape->GetAllNodes();
		if (check)
		{
			for (auto& iter : *nodes)
			{
				CColliderAABB *pCollider = iter->pGameObject->FindComponentFromTag<CColliderAABB>("Collider");
				pCollider->SetColliderRenderCheck(true);
				SAFE_RELEASE(pCollider);
			}
		}
		else
		{
			for (auto& iter : *nodes)
			{
				CColliderAABB *pCollider = iter->pGameObject->FindComponentFromTag<CColliderAABB>("Collider");
				pCollider->SetColliderRenderCheck(false);
				SAFE_RELEASE(pCollider);
			}
		}
		SAFE_RELEASE(pLandScape);
		SAFE_RELEASE(pLandScapeObj);
	}
}

// NaviTab.cpp: 구현 파일
//

#include "stdafx.h"
#include "EnvTool.h"
#include "NaviTab.h"
#include "afxdialogex.h"

#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Scene/Layer.h"
#include "Scene/SceneManager.h"
#include "Component/Transform.h"
#include "Component/Picking.h"

#include "BrushTool.h"

#include "Core/NaviManager.h"
#include "Core/NaviMesh.h"

PG_USING

// CNaviTab 대화 상자

IMPLEMENT_DYNAMIC(CNaviTab, CDialogEx)

CNaviTab::CNaviTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

CNaviTab::~CNaviTab()
{
}

void CNaviTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_NAVI_POINT, m_listNaviPoint);
	DDX_Control(pDX, IDC_LIST_NAVI_CELL, m_listNaviCell);
	DDX_Control(pDX, IDC_CHECK_NAVI, m_checkBoxNaviOn);
}


BEGIN_MESSAGE_MAP(CNaviTab, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_NAVI, &CNaviTab::OnBnClickedCheckNavi)
END_MESSAGE_MAP()


// CNaviTab 메시지 처리기


BOOL CNaviTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	GET_SINGLE(CNaviManager)->CreateNaviMesh("test");
	GET_SINGLE(CNaviManager)->SetRenderCheck(true);

	m_checkBoxNaviOn.SetCheck(1);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CNaviTab::Process_ChangeTab()
{
	CGameObject* pBrushObj = CGameObject::FindObject("Brush");
	CBrushTool* pBrushTool = pBrushObj->FindComponentFromTag<CBrushTool>("BrushTool");

	pBrushTool->SetBrushCheck(false);
	// 한번 Update위한 함수(이렇게 해야 Brush모양이 사라짐)
	pBrushTool->SetBrushInformation(Vector3{ 0.f, 0.f, 0.f });

	SAFE_RELEASE(pBrushTool);
	SAFE_RELEASE(pBrushObj);
}

void CNaviTab::Process_ShowTab()
{
	CGameObject* pBrushObj = CGameObject::FindObject("Brush");
	CBrushTool* pBrushTool = pBrushObj->FindComponentFromTag<CBrushTool>("BrushTool");

	pBrushTool->SetBrushCheck(true);
	pBrushTool->SetBrushInformation(1.f);

	SAFE_RELEASE(pBrushTool);
	SAFE_RELEASE(pBrushObj);
}

void CNaviTab::UpdateForm()
{
}

void CNaviTab::Add_Point(const Vector3 & vPoint)
{
	// Vector
	NAVIPOINT tPoint(vPoint);

	// listBox
	m_vecNaviPoint.push_back(tPoint);

	if (m_vecNaviPoint.size() >= 3)
	{	
		CNaviMesh* pNavi = GET_SINGLE(CNaviManager)->FindNaviMesh("test");

		pNavi->AddCell(m_vecNaviPoint);

		m_vecNaviPoint.clear();
		m_listNaviPoint.ResetContent();
		m_listNaviCell.AddString(CString(to_string(m_NumCell).c_str()));
		m_NumCell++;
	}	
	else
	{
		m_listNaviPoint.AddString(L"Point");
	}
}


void CNaviTab::OnBnClickedCheckNavi()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int check = m_checkBoxNaviOn.GetCheck();

	if (check == 1)
	{
		GET_SINGLE(CNaviManager)->SetRenderCheck(true);
	}
	else
	{
		GET_SINGLE(CNaviManager)->SetRenderCheck(false);
	}
}

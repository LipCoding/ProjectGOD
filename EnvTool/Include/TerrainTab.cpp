// TerrainTab.cpp: 구현 파일
//

#include "stdafx.h"
#include "EnvTool.h"
#include "TerrainTab.h"
#include "afxdialogex.h"

#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Scene/Layer.h"
#include "Scene/SceneManager.h"
#include "Component/Transform.h"
#include "Component/Renderer2D.h"
#include "Component/Material.h"
#include "Component/Terrain2D.h"
#include "Component/LandScape.h"
#include "Component/Picking.h"

#include "BrushTool.h"

PG_USING

// CTerrainTab 대화 상자

IMPLEMENT_DYNAMIC(CTerrainTab, CDialogEx)

CTerrainTab::CTerrainTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_iSizeX(0)
	, m_iSizeZ(0)
	, m_iRadio(0)
	, m_iRadio1(0)
	, m_iRadio2(0)
	, m_iRadio_Texture(0)
{

}

CTerrainTab::~CTerrainTab()
{
}

void CTerrainTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TERRAIN_SIZEX, m_editTerrainSizeX);
	DDX_Control(pDX, IDC_EDIT_TERRIAN_SIZEY, m_editTerrainSizeZ);
	DDX_Control(pDX, IDC_CHECK_HEIGHT, m_checkBoxHeightCheck);
	DDX_Control(pDX, IDC_SLIDER_RANGE, m_ctrSliderBrushRange);
	DDX_Control(pDX, IDC_EDIT_BRUSH_RANGE, m_editBrushRange);
	DDX_Control(pDX, IDC_SLIDER_HEIGHTPOWER, m_ctrSliderHeightPower);
	DDX_Control(pDX, IDC_EDIT_HEIGHTPOWER, m_editHeightPower);
	DDX_Radio(pDX, IDC_RADIO_HEIGHT_DIR_1, (int&)m_iRadio);
	DDX_Radio(pDX, IDC_RADIO_UPTYPE_HEIGHT_1, (int&)m_iRadio1);
	DDX_Radio(pDX, IDC_RADIO_MODE_1, (int&)m_iRadio2);
	DDX_Radio(pDX, IDC_RADIO_DETAIL_1, (int&)m_iRadio_Texture);
	DDX_Control(pDX, IDC_SLIDER_DETAIL_DEFAULT, m_ctrSliderDetail_Default);
}


BEGIN_MESSAGE_MAP(CTerrainTab, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADJ_SIZE, &CTerrainTab::OnBnClickedButtonAdjSize)
	ON_BN_CLICKED(IDC_CHECK_HEIGHT, &CTerrainTab::OnBnClickedCheckBrush)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_HEIGHT_DIR_1, IDC_RADIO_HEIGHT_DIR_2, OnRadioCheck)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_UPTYPE_HEIGHT_1, IDC_RADIO_UPTYPE_HEIGHT_3, OnRadioCheck1)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_MODE_1, IDC_RADIO_MODE_2, OnRadioCheck2)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_DETAIL_1, IDC_RADIO_DETAIL_5, OnRadioCheck_Texture)

//	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_RANGE, &CTerrainTab::OnNMReleasedcaptureSliderRange)
ON_WM_HSCROLL()
ON_BN_CLICKED(IDC_BUTTON_HEIGHT_RESET, &CTerrainTab::OnBnClickedButtonHeightReset)
ON_BN_CLICKED(IDC_BUTTON_TEX_LOAD, &CTerrainTab::OnBnClickedButtonTexLoad)
END_MESSAGE_MAP()

// CTerrainTab 메시지 처리기

void CTerrainTab::OnBnClickedButtonAdjSize()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString sizeX, sizeZ;

	// 에디트 컨트롤을 가져온다.
	m_editTerrainSizeX.GetWindowTextA(sizeX);
	m_editTerrainSizeZ.GetWindowTextA(sizeZ);

	// string을 숫자로 변환
	m_iSizeX = _ttoi(sizeX);
	m_iSizeZ = _ttoi(sizeZ);

	// terrain 생성
	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();

	CLayer* pLayer = pScene->GetLayer("Default");
	CGameObject* pLandScapeObj = CGameObject::CreateObject("LandScape", pLayer);
	CTransform* pLandTr = pLandScapeObj->GetTransform();
	SAFE_RELEASE(pLandTr);

	CLandScape* pLandScape = pLandScapeObj->AddComponent<CLandScape>("LandScape");

	pLandScape->CreateLandScapeQuadTree("LandScapeMesh", m_iSizeX, m_iSizeZ, false, "LandScape",
		L"LandScape/GRASS_00+SAND.dds",
		L"LandScape/GRASS_00+SAND_NRM.png",
		L"LandScape/GRASS_00+SAND_SPEC.png");

	CPicking* pPicking = pLandScapeObj->AddComponent<CPicking>("Picking");

	SAFE_RELEASE(pPicking);
	SAFE_RELEASE(pLandScape);

	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pScene);

	_cprintf("Terrain Create Success!\n");
}


void CTerrainTab::OnBnClickedCheckBrush()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iCheckStatus = m_checkBoxHeightCheck.GetCheck();

	CGameObject* pBrushObj = CGameObject::FindObject("Brush");
	CBrushTool* pBrushTool = pBrushObj->FindComponentFromTag<CBrushTool>("BrushTool");

	if (iCheckStatus == 1)
	{
		pBrushTool->SetBrushCheck(true);
	}
	else
	{
		pBrushTool->SetBrushCheck(false);
	}

	SAFE_RELEASE(pBrushTool);
	SAFE_RELEASE(pBrushObj);
}


BOOL CTerrainTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrSliderBrushRange.SetRange(1, 50);
	m_ctrSliderBrushRange.SetPos(1);
	m_editBrushRange.SetWindowTextA(to_string(1).c_str());

	// 눈금
	m_ctrSliderBrushRange.SetTicFreq(5);

	// 키보드 커서키 조작시 증감 크기
	m_ctrSliderBrushRange.SetLineSize(1);

	//
	m_ctrSliderHeightPower.SetRange(1, 100);
	m_ctrSliderHeightPower.SetPos(10);
	m_editHeightPower.SetWindowTextA(to_string(10 / 10.f).c_str());

	// 눈금
	m_ctrSliderHeightPower.SetTicFreq(10);

	// 키보드 커서키 조작시 증감 크기
	m_ctrSliderHeightPower.SetLineSize(1);

	//
	m_ctrSliderDetail_Default.SetRange(1, 50);
	m_ctrSliderDetail_Default.SetPos(1);
	
	// 눈금
	m_ctrSliderDetail_Default.SetTicFreq(5);

	// 키보드 커서키 조작시 증감 크기
	m_ctrSliderDetail_Default.SetLineSize(1);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


//void CTerrainTab::OnNMReleasedcaptureSliderRange(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	*pResult = 0;
//}


void CTerrainTab::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다

	if (pScrollBar)
	{
		// 어떤 슬라이더인지 검사
		if (pScrollBar == (CScrollBar*)&m_ctrSliderBrushRange)
		{
			int iPos = m_ctrSliderBrushRange.GetPos();

			CGameObject* pBrushObj = CGameObject::FindObject("Brush");
			CBrushTool* pBrushTool = pBrushObj->FindComponentFromTag<CBrushTool>("BrushTool");

			pBrushTool->SetBrushInformation((float)iPos);
			m_editBrushRange.SetWindowTextA(to_string(iPos).c_str());

			SAFE_RELEASE(pBrushTool);
			SAFE_RELEASE(pBrushObj);
		}
		else if (pScrollBar == (CScrollBar*)&m_ctrSliderHeightPower)
		{
			int iPos = m_ctrSliderHeightPower.GetPos();

			CGameObject* pBrushObj = CGameObject::FindObject("Brush");
			CBrushTool* pBrushTool = pBrushObj->FindComponentFromTag<CBrushTool>("BrushTool");

			pBrushTool->SetSpeed((float)iPos / 10.f);
			m_editHeightPower.SetWindowTextA(to_string((float)iPos / 10.f).c_str());

			SAFE_RELEASE(pBrushTool);
			SAFE_RELEASE(pBrushObj);
		}
		else if (pScrollBar == (CScrollBar*)&m_ctrSliderDetail_Default)
		{
			int iPos = m_ctrSliderDetail_Default.GetPos();

			CGameObject* pLandScapeObj = CGameObject::FindObject("LandScape");
			if (pLandScapeObj)
			{
				CLandScape* pLandScape = pLandScapeObj->FindComponentFromTag<CLandScape>("LandScape");

				pLandScape->SetDetailLevel(iPos);

				SAFE_RELEASE(pLandScape);
				SAFE_RELEASE(pLandScapeObj);
			}
		}
	}
	else
	{
		// CScrollView를 상속받은 뷰의 경우 프레임의 스크롤 동작시 pScrollBar가 
		// NULL이 된다.
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CTerrainTab::OnRadioCheck(UINT id)
{
	UpdateData(TRUE);

	CGameObject* pBrushObj = CGameObject::FindObject("Brush");
	CBrushTool* pBrushTool = pBrushObj->FindComponentFromTag<CBrushTool>("BrushTool");

	switch (m_iRadio)
	{
	case 0:
		pBrushTool->SetSpeedDir(true);
		break;
	case 1:
		pBrushTool->SetSpeedDir(false);
		break;
	}

	SAFE_RELEASE(pBrushTool);
	SAFE_RELEASE(pBrushObj);
}

void CTerrainTab::OnRadioCheck1(UINT id)
{
	UpdateData(TRUE);

	CGameObject* pBrushObj = CGameObject::FindObject("Brush");
	CBrushTool* pBrushTool = pBrushObj->FindComponentFromTag<CBrushTool>("BrushTool");

	switch (m_iRadio1)
	{
	case 0:
		pBrushTool->SetUpType(UP_TYPE_FLAT);
		break;
	case 1:
		pBrushTool->SetUpType(UP_TYPE_ROUND);
		break;
	case 2:
		pBrushTool->SetUpType(UP_TYPE_SMOOTH);
		break;
	}

	SAFE_RELEASE(pBrushTool);
	SAFE_RELEASE(pBrushObj);
}

void CTerrainTab::OnRadioCheck2(UINT id)
{
	UpdateData(TRUE);

	CGameObject* pBrushObj = CGameObject::FindObject("Brush");
	CBrushTool* pBrushTool = pBrushObj->FindComponentFromTag<CBrushTool>("BrushTool");

	switch (m_iRadio2)
	{
	case 0:
		// Height
		pBrushTool->SetHeightCheck(true);
		pBrushTool->SetSlattingCheck(false);
		break;
	case 1:
		// Splatting
		pBrushTool->SetHeightCheck(false);
		pBrushTool->SetSlattingCheck(true);
		break;
	}

	SAFE_RELEASE(pBrushTool);
	SAFE_RELEASE(pBrushObj);
}

void CTerrainTab::OnRadioCheck_Texture(UINT id)
{
	UpdateData(TRUE);

	switch (m_iRadio_Texture)
	{
	case 0:
		// Default
		break;
	case 1:
		// Tex
		break;
	case 2:

		break;
	case 3:

		break;
	case 4:

		break;
	}
}


void CTerrainTab::OnBnClickedButtonHeightReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CGameObject* pBrushObj = CGameObject::FindObject("Brush");
	CBrushTool* pBrushTool = pBrushObj->FindComponentFromTag<CBrushTool>("BrushTool");

	pBrushTool->ResetHeight();

	SAFE_RELEASE(pBrushTool);
	SAFE_RELEASE(pBrushObj);
}


void CTerrainTab::OnBnClickedButtonTexLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

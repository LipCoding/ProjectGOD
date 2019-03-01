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
	DDX_Control(pDX, IDC_EDIT3, m_editDetail_Default);
	DDX_Control(pDX, IDC_EDIT5, m_editDetail_Tex1);
	DDX_Control(pDX, IDC_EDIT6, m_editDetail_Tex2);
	DDX_Control(pDX, IDC_EDIT7, m_editDetail_Tex3);
	DDX_Control(pDX, IDC_EDIT4, m_editDetail_Tex4);
	DDX_Control(pDX, IDC_SLIDER_SPLAT_POWER, m_ctrSlideSplatPower);
	DDX_Control(pDX, IDC_EDIT_SPLAT_POWER, m_editSplatPower);
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
ON_BN_CLICKED(IDC_BUTTON_SPLAT_LOAD, &CTerrainTab::OnBnClickedButtonSplatLoad)
ON_WM_VSCROLL()
ON_BN_CLICKED(IDC_BUTTON_TERRAIN_SAVE, &CTerrainTab::OnBnClickedButtonTerrainSave)
ON_BN_CLICKED(IDC_BUTTON_TERRAIN_LOAD, &CTerrainTab::OnBnClickedButtonTerrainLoad)
END_MESSAGE_MAP()

// CTerrainTab 메시지 처리기

void CTerrainTab::OnBnClickedButtonAdjSize()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString sizeX, sizeZ;

	// 에디트 컨트롤을 가져온다.
	m_editTerrainSizeX.GetWindowTextW(sizeX);
	m_editTerrainSizeZ.GetWindowTextW(sizeZ);

	// string을 숫자로 변환
	m_iSizeX = _ttoi(sizeX);
	m_iSizeZ = _ttoi(sizeZ);

	CGameObject* pLandScapeFind = CGameObject::FindObject("LandScape");

	if (pLandScapeFind)
	{
		AfxMessageBox(L"Error : You already have terrain!");
		SAFE_RELEASE(pLandScapeFind);
		return;
	}

	// terrain 생성
	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();

	CLayer* pLayer = pScene->GetLayer("Default");
	CGameObject* pLandScapeObj = CGameObject::CreateObject("LandScape", pLayer);
	CTransform* pLandTr = pLandScapeObj->GetTransform();
	SAFE_RELEASE(pLandTr);

	CLandScape* pLandScape = pLandScapeObj->AddComponent<CLandScape>("LandScape");

	pLandScape->CreateLandScapeQuadTree("LandScapeMesh", m_iSizeX, m_iSizeZ, false, "LandScape",
		L"Terrain/TerrainTex0_D.tga", 
		L"Terrain/TerrainTex0_N.tga",
		L"Terrain/TerrainTex0_S.tga");

	vector<wstring>	vecSplatting;

	wchar_t	strSplatPath[MAX_PATH] = {};

	// splatting

	// Diffuse
	wsprintf(strSplatPath, L"LandScape/BD_Terrain_Cliff05.dds");
	vecSplatting.push_back(strSplatPath);

	//vecSplatting.clear();
	memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);
	wsprintf(strSplatPath, L"LandScape/Terrain_Cliff_15_Large.dds");
	vecSplatting.push_back(strSplatPath);


	//pLandScape->SetDiffuseSplattingQuadTree("Linear", "SplatDif", &vecSplatting);


	// Normal
	vecSplatting.clear();
	memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);
	wsprintf(strSplatPath, L"LandScape/BD_Terrain_Cliff05_NRM.bmp");
	vecSplatting.push_back(strSplatPath);

	memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);
	wsprintf(strSplatPath, L"LandScape/Terrain_Cliff_15_Large_NRM.bmp");
	vecSplatting.push_back(strSplatPath);


	//pLandScape->SetNormalSplattingQuadTree("Linear", "SplatNormal", &vecSplatting);


	// Specular
	vecSplatting.clear();
	memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);
	wsprintf(strSplatPath, L"LandScape/BD_Terrain_Cliff05_SPEC.bmp");
	vecSplatting.push_back(strSplatPath);

	memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);
	wsprintf(strSplatPath, L"LandScape/Terrain_Cliff_15_Large_SPEC.bmp");
	vecSplatting.push_back(strSplatPath);

	//pLandScape->SetSpecularSplattingQuadTree("Linear", "SplatSpecular", &vecSplatting);

	// File
	vecSplatting.clear();
	memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);
	wsprintf(strSplatPath, L"LandScape/Splat.bmp");
	vecSplatting.push_back(strSplatPath);

	memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);
	wsprintf(strSplatPath, L"LandScape/Splat2.bmp");
	vecSplatting.push_back(strSplatPath);

	//pLandScape->SetSplattingAlphaQuadTree("Linear", "SplatAlpha", &vecSplatting);

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
	CString num;

	// Range
	m_ctrSliderBrushRange.SetRange(1, 50);
	m_ctrSliderBrushRange.SetPos(1);
	num.Format(_T("%d"), 1);
	m_editBrushRange.SetWindowTextW(num);

	// 눈금
	m_ctrSliderBrushRange.SetTicFreq(5);

	// 키보드 커서키 조작시 증감 크기
	m_ctrSliderBrushRange.SetLineSize(1);

	// Height Power
	m_ctrSliderHeightPower.SetRange(1, 100);
	m_ctrSliderHeightPower.SetPos(10);
	num.Format(_T("%f"), 10 / 10.f);
	m_editHeightPower.SetWindowTextW(num);

	m_ctrSliderHeightPower.SetTicFreq(10);
	m_ctrSliderHeightPower.SetLineSize(1);

	// Splat Power
	m_ctrSlideSplatPower.SetRange(50, 500);
	m_ctrSlideSplatPower.SetPos(50);
	num.Format(_T("%d"), 50);
	m_editSplatPower.SetWindowTextW(num);

	m_ctrSlideSplatPower.SetTicFreq(50);
	m_ctrSlideSplatPower.SetLineSize(50);

	m_ctrSliderDetail_Default.SetRange(1, 50);
	m_ctrSliderDetail_Default.SetPos(1);
	
	m_ctrSliderDetail_Default.SetTicFreq(5);
	m_ctrSliderDetail_Default.SetLineSize(1);

	// 스플래팅 정보를 담는 벡터 크기 조절
	m_vecSplattingDiffuse.resize(4);
	m_vecSplattingNormal.resize(4);
	m_vecSplattingSpecular.resize(4);
	m_vecSplattingAlpha.resize(4);


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
		CString num;
		// 어떤 슬라이더인지 검사
		if (pScrollBar == (CScrollBar*)&m_ctrSliderBrushRange)
		{
			int iPos = m_ctrSliderBrushRange.GetPos();

			CGameObject* pBrushObj = CGameObject::FindObject("Brush");
			CBrushTool* pBrushTool = pBrushObj->FindComponentFromTag<CBrushTool>("BrushTool");

			pBrushTool->SetBrushInformation((float)iPos);
			num.Format(_T("%d"), iPos);
			m_editBrushRange.SetWindowTextW(num);

			SAFE_RELEASE(pBrushTool);
			SAFE_RELEASE(pBrushObj);
		}
		else if (pScrollBar == (CScrollBar*)&m_ctrSliderHeightPower)
		{
			int iPos = m_ctrSliderHeightPower.GetPos();

			CGameObject* pBrushObj = CGameObject::FindObject("Brush");
			CBrushTool* pBrushTool = pBrushObj->FindComponentFromTag<CBrushTool>("BrushTool");

			pBrushTool->SetSpeed((float)iPos / 10.f);
			num.Format(_T("%f"), (float)iPos / 10.f);
			m_editHeightPower.SetWindowTextW(num);

			SAFE_RELEASE(pBrushTool);
			SAFE_RELEASE(pBrushObj);
		}
		else if (pScrollBar == (CScrollBar*)&m_ctrSliderDetail_Default)
		{
			int iPos = m_ctrSliderDetail_Default.GetPos();

			switch (m_iRadio_Texture)
			{
			case 0:
			{
				CGameObject* pLandScapeObj = CGameObject::FindObject("LandScape");
				if (pLandScapeObj)
				{
					CLandScape* pLandScape = pLandScapeObj->FindComponentFromTag<CLandScape>("LandScape");

					pLandScape->SetDetailLevel(iPos);

					SAFE_RELEASE(pLandScape);
					SAFE_RELEASE(pLandScapeObj);
				}
				break;
			}
			case 1:
			{
				CGameObject* pLandScapeObj = CGameObject::FindObject("LandScape");
				if (pLandScapeObj)
				{
					CLandScape* pLandScape = pLandScapeObj->FindComponentFromTag<CLandScape>("LandScape");

					pLandScape->SetDetailLevel_Splat(0, iPos);

					SAFE_RELEASE(pLandScape);
					SAFE_RELEASE(pLandScapeObj);
				}
				break;
			}
			case 2:
			{
				CGameObject* pLandScapeObj = CGameObject::FindObject("LandScape");
				if (pLandScapeObj)
				{
					CLandScape* pLandScape = pLandScapeObj->FindComponentFromTag<CLandScape>("LandScape");

					pLandScape->SetDetailLevel_Splat(1, iPos);

					SAFE_RELEASE(pLandScape);
					SAFE_RELEASE(pLandScapeObj);
				}
				break;
			}
			case 3:
			{
				CGameObject* pLandScapeObj = CGameObject::FindObject("LandScape");
				if (pLandScapeObj)
				{
					CLandScape* pLandScape = pLandScapeObj->FindComponentFromTag<CLandScape>("LandScape");

					pLandScape->SetDetailLevel_Splat(2, iPos);

					SAFE_RELEASE(pLandScape);
					SAFE_RELEASE(pLandScapeObj);
				}
				break;
			}
			case 4:
			{
				CGameObject* pLandScapeObj = CGameObject::FindObject("LandScape");
				if (pLandScapeObj)
				{
					CLandScape* pLandScape = pLandScapeObj->FindComponentFromTag<CLandScape>("LandScape");

					pLandScape->SetDetailLevel_Splat(3, iPos);

					SAFE_RELEASE(pLandScape);
					SAFE_RELEASE(pLandScapeObj);
				}
				break;
			}
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

	CGameObject* pBrushObj = CGameObject::FindObject("Brush");
	CBrushTool* pBrushTool = pBrushObj->FindComponentFromTag<CBrushTool>("BrushTool");

	switch (m_iRadio_Texture)
	{
	case 0:
	{
		// Default
		CGameObject* pLandScapeObj = CGameObject::FindObject("LandScape");
		if (pLandScapeObj)
		{
			CLandScape* pLandScape = pLandScapeObj->FindComponentFromTag<CLandScape>("LandScape");

			m_ctrSliderDetail_Default.SetPos(pLandScape->GetDetailLevel());

			SAFE_RELEASE(pLandScape);
			SAFE_RELEASE(pLandScapeObj);
		}
		break;
	}
	case 1:
	{
		// Tex
		CGameObject* pLandScapeObj = CGameObject::FindObject("LandScape");
		if (pLandScapeObj)
		{
			CLandScape* pLandScape = pLandScapeObj->FindComponentFromTag<CLandScape>("LandScape");

			m_ctrSliderDetail_Default.SetPos((pLandScape->GetDetailLevel_Splat())[0]);

			SAFE_RELEASE(pLandScape);
			SAFE_RELEASE(pLandScapeObj);
		}
		pBrushTool->SetTexType(0);
		break;
	}
	case 2:
	{
		CGameObject* pLandScapeObj = CGameObject::FindObject("LandScape");
		if (pLandScapeObj)
		{
			CLandScape* pLandScape = pLandScapeObj->FindComponentFromTag<CLandScape>("LandScape");

			m_ctrSliderDetail_Default.SetPos((pLandScape->GetDetailLevel_Splat())[1]);

			SAFE_RELEASE(pLandScape);
			SAFE_RELEASE(pLandScapeObj);
		}
		pBrushTool->SetTexType(1);
		break;
	}
	case 3:
	{
		CGameObject* pLandScapeObj = CGameObject::FindObject("LandScape");
		if (pLandScapeObj)
		{
			CLandScape* pLandScape = pLandScapeObj->FindComponentFromTag<CLandScape>("LandScape");

			m_ctrSliderDetail_Default.SetPos((pLandScape->GetDetailLevel_Splat())[2]);

			SAFE_RELEASE(pLandScape);
			SAFE_RELEASE(pLandScapeObj);
		}
		pBrushTool->SetTexType(2);
		break;
	}
	case 4:
	{
		CGameObject* pLandScapeObj = CGameObject::FindObject("LandScape");
		if (pLandScapeObj)
		{
			CLandScape* pLandScape = pLandScapeObj->FindComponentFromTag<CLandScape>("LandScape");

			m_ctrSliderDetail_Default.SetPos((pLandScape->GetDetailLevel_Splat())[3]);

			SAFE_RELEASE(pLandScape);
			SAFE_RELEASE(pLandScapeObj);
		}
		pBrushTool->SetTexType(3);
		break;
	}
	}

	SAFE_RELEASE(pBrushTool);
	SAFE_RELEASE(pBrushObj);
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
	static TCHAR BASED_CODE szFilter[] =
		_T("이미지 파일(*.BMP,*.GIF,*.JPG, *.TGA, *.DDS) | *.BMP,*.GIF,*.JPG,*.TGA,*.DDS;*.bmp;*.gif;*.jpg;*.tga;*.dds|모든파일(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);

	// 경로 지정
	wchar_t strPath[MAX_PATH] = {};
	wcscpy_s(strPath, MAX_PATH, GET_SINGLE(CPathManager)->FindPath(TEXTURE_PATH));

	CString originPath = strPath;

	dlg.m_ofn.lpstrInitialDir = strPath;

	// do modal error 해결
	if (dlg.DoModal() != IDOK)
		return;

	//if (IDOK == dlg.DoModal())
	{
		// 절대경로 하위 경로
		CString fileDir = dlg.GetPathName();

		fileDir.Delete(0, int(lstrlen(strPath) - 1));

		for (int i = lstrlen(fileDir) - 1; i >= 0; i--)
		{
			if (fileDir[i] == '\\')
			{
				fileDir.Delete(i + 1, lstrlen(fileDir) - 1);
				fileDir.Delete(0, 1);
				break;
			}		
		}

		// 파일 이름 + 확장자
		CString fileName = dlg.GetFileName();

		// 파일 확장자
		CString fileType = dlg.GetFileExt();

		// 파일 이름 (_D, _N, _S를 제외한)
		CString fileTitleName = dlg.GetFileTitle();
		fileTitleName.Delete((lstrlen(fileTitleName) - 1) - 1, (lstrlen(fileTitleName) - 1));

		// 경로를 담을 변수
		CString diffuseName;
		CString normalName;
		CString specularName;

		CGameObject* pLandScapeObj = CGameObject::FindObject("LandScape");
		CGameObject* pBrushObj = CGameObject::FindObject("Brush");
		CBrushTool* pBrushTool = pBrushObj->FindComponentFromTag<CBrushTool>("BrushTool");


		diffuseName = fileDir + fileTitleName + L"_D";
		normalName = fileDir + fileTitleName + L"_N";
		specularName = fileDir + fileTitleName + L"_S";

		fileType = L"." + fileType;

		diffuseName += fileType;
		normalName += fileType;
		specularName += fileType;

		wchar_t	strSplatPath[MAX_PATH] = {};

		if (pLandScapeObj)
		{
			CLandScape* pLandScape = pLandScapeObj->FindComponentFromTag<CLandScape>("LandScape");

			switch (m_iRadio_Texture)
			{
			case 0:
				// Default
				pLandScape->SetMaterial_DNS_Default(T2W(diffuseName.GetBuffer(0)), T2W(normalName.GetBuffer(0)), T2W(specularName.GetBuffer(0)));
				m_editDetail_Default.SetWindowTextW(fileName);
				break;
			case 1:
			{
				// Tex
				// D
				wsprintf(strSplatPath, diffuseName);
				m_vecSplattingDiffuse[0] = strSplatPath;
				memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);
				// N
				wsprintf(strSplatPath, normalName);
				m_vecSplattingNormal[0] = strSplatPath;
				memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);
				// S
				wsprintf(strSplatPath, specularName);
				m_vecSplattingSpecular[0] = strSplatPath;
				memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);
				// A
				wsprintf(strSplatPath, L"Tool\\Tex1.bmp");
				m_vecSplattingAlpha[0] = strSplatPath;
				memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);

				pLandScape->SetMaterial_Splatting(
					m_vecSplattingDiffuse,
					m_vecSplattingNormal,
					m_vecSplattingSpecular,
					m_vecSplattingAlpha);

				pBrushTool->SetArrPixel(0);

				m_editDetail_Tex1.SetWindowTextW(fileName);
				break;
			}
			case 2:
			{
				if (m_vecSplattingAlpha[0] == L"")
				{
					AfxMessageBox(L"Error : Add Texture 1 First!");
					break;
				}

				wsprintf(strSplatPath, diffuseName);
				m_vecSplattingDiffuse[1] = strSplatPath;
				memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);

				wsprintf(strSplatPath, normalName);
				m_vecSplattingNormal[1] = strSplatPath;
				memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);

				wsprintf(strSplatPath, specularName);
				m_vecSplattingSpecular[1] = strSplatPath;
				memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);

				wsprintf(strSplatPath, L"Tool\\Tex2.bmp");
				m_vecSplattingAlpha[1] = strSplatPath;
				memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);

				pLandScape->SetMaterial_Splatting(m_vecSplattingDiffuse,
					m_vecSplattingNormal,
					m_vecSplattingSpecular,
					m_vecSplattingAlpha);

				pBrushTool->SetArrPixel(1);

				m_editDetail_Tex2.SetWindowTextW(fileName);

				break;
			}
			case 3:
			{
				if (m_vecSplattingAlpha[1] == L"")
				{
					AfxMessageBox(L"Error : Add Texture 2 First!");
					break;
				}

				wsprintf(strSplatPath, diffuseName);
				m_vecSplattingDiffuse[2] = strSplatPath;
				memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);

				wsprintf(strSplatPath, normalName);
				m_vecSplattingNormal[2] = strSplatPath;
				memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);

				wsprintf(strSplatPath, specularName);
				m_vecSplattingSpecular[2] = strSplatPath;
				memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);

				wsprintf(strSplatPath, L"Tool\\Tex3.bmp");
				m_vecSplattingAlpha[2] = strSplatPath;
				memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);

				pLandScape->SetMaterial_Splatting(m_vecSplattingDiffuse,
					m_vecSplattingNormal,
					m_vecSplattingSpecular,
					m_vecSplattingAlpha);

				m_editDetail_Tex3.SetWindowTextW(fileName);

				pBrushTool->SetArrPixel(2);

				break;
			}
			case 4:
			{
				if (m_vecSplattingAlpha[0] == L"")
				{
					AfxMessageBox(L"Error : Add Texture 3 First!");
					break;
				}

				wsprintf(strSplatPath, diffuseName);
				m_vecSplattingDiffuse[3] = strSplatPath;
				memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);
		
				wsprintf(strSplatPath, normalName);
				m_vecSplattingNormal[3] = strSplatPath;
				memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);
				
				wsprintf(strSplatPath, specularName);
				m_vecSplattingSpecular[3] = strSplatPath;
				memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);

				wsprintf(strSplatPath, L"Tool\\Tex4.bmp");
				m_vecSplattingAlpha[3] = strSplatPath;
				memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);

				// process
				pLandScape->SetMaterial_Splatting(m_vecSplattingDiffuse,
					m_vecSplattingNormal,
					m_vecSplattingSpecular,
					m_vecSplattingAlpha);

				m_editDetail_Tex4.SetWindowTextW(fileName);

				pBrushTool->SetArrPixel(3);

				break;
			}
			}
			SAFE_RELEASE(pLandScape);
			SAFE_RELEASE(pLandScapeObj);
		}
		else
		{
			AfxMessageBox(L"Error : No terrain! Make terrain first!");
		}

		for (int i = 0; i < 4; i++)
		{
			// 기존 픽셀 정보를 다시 Set
			pBrushTool->SettingOriginPixelToTexture(i);
		}

		SAFE_RELEASE(pBrushTool);
		SAFE_RELEASE(pBrushObj);
	}
}


void CTerrainTab::OnBnClickedButtonSplatLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
}

int CTerrainTab::SaveTextureName(string fileName)
{
	ofstream file;

	file.open(fileName + ".dat", ios::out | ios::trunc /*| ios::binary*/);

	int iFileCount = 0;

	for (const auto iter : m_vecSplattingDiffuse)
	{
		if (iter != L"")
			++iFileCount;
	}

	if (iFileCount == 0)
		return iFileCount;

	// 스플레팅 텍스쳐 갯수
	file << iFileCount << endl;

	// wchar_t* 저장
	for (int i = 0; i < iFileCount; i++)
	{
		string diffuse;
		string normal;
		string specular;

		diffuse.assign(m_vecSplattingDiffuse[i].begin(), (m_vecSplattingDiffuse[i].end()));
		normal.assign(m_vecSplattingNormal[i].begin(), (m_vecSplattingNormal[i].end()));
		specular.assign(m_vecSplattingSpecular[i].begin(), (m_vecSplattingSpecular[i].end()));

		file << diffuse << endl;
		file << normal << endl;
		file << specular << endl;
	}

	return iFileCount;
}


void CTerrainTab::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (pScrollBar)
	{
		CString num;

		if (pScrollBar == (CScrollBar*)&m_ctrSlideSplatPower)
		{
			int iPos = m_ctrSlideSplatPower.GetPos();

			CGameObject* pBrushObj = CGameObject::FindObject("Brush");
			CBrushTool* pBrushTool = pBrushObj->FindComponentFromTag<CBrushTool>("BrushTool");

			pBrushTool->SetPower((float)iPos);
			num.Format(_T("%d"), iPos);
			m_editSplatPower.SetWindowTextW(num);

			SAFE_RELEASE(pBrushTool);
			SAFE_RELEASE(pBrushObj);

		}
	}
	else
	{

	}

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CTerrainTab::OnBnClickedButtonTerrainSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static TCHAR BASED_CODE szFilter[] =
		_T("데이터 파일(*.bin) | *.bin;|모든파일(*.*)|*.*||");
	CFileDialog dlg(FALSE, NULL, NULL, OFN_OVERWRITEPROMPT, szFilter);

	// 경로 지정
	wchar_t strPath[MAX_PATH] = {};
	wcscpy_s(strPath, MAX_PATH, GET_SINGLE(CPathManager)->FindPath(DATA_PATH));

	CString originPath = strPath;

	dlg.m_ofn.lpstrInitialDir = strPath;

	// do modal error 해결
	if (dlg.DoModal() != IDOK)
		return;

	CString path = dlg.GetPathName();
	CString fileName = dlg.GetFileName();

	for (int i = lstrlen(path) - 1; i >= 0; i--)
	{
		if (path[i] == '\\')
		{
			path.Delete(i + 1, lstrlen(path) - 1);
			break;
		}
	}

	for (int i = lstrlen(fileName) - 1; i >= 0; i--)
	{
		if (fileName[i] == '.')
		{
			fileName.Delete(i, lstrlen(fileName) - 1);
			break;
		}
	}
	
	CT2CA pszConvertAnsiStringPathName(path);
	string strFilePath(pszConvertAnsiStringPathName);
	CT2CA pszConvertAnsiStringFileName(fileName);
	string strFileName(pszConvertAnsiStringFileName);
	string heightFileName = "Height_" + strFileName;
	string textureFileName = "Texture_" + strFileName;
	string alphaFileName = "Bitmap_" + strFileName + "_0";
	// 저장
	CGameObject* pLandScapeObj = CGameObject::FindObject("LandScape");
	if (pLandScapeObj)
	{
		CLandScape* pLandScape = pLandScapeObj->FindComponentFromTag<CLandScape>("LandScape");
		// 노드 정보 저장
		ofstream* nodeFile = nullptr;
		pLandScape->Save_QuadTree(strFilePath + heightFileName);

		// 텍스쳐이름, Splatting 개수 저장
		int iCount = SaveTextureName(strFilePath + textureFileName);

		// 알파스플래팅 bmp 저장
		CGameObject* pBrushObj = CGameObject::FindObject("Brush");
		CBrushTool* pBrushTool = pBrushObj->FindComponentFromTag<CBrushTool>("BrushTool");

		pBrushTool->Save_AlphaSplat_Bitmap(strFilePath + alphaFileName);

		SAFE_RELEASE(pBrushTool);
		SAFE_RELEASE(pBrushObj);
		SAFE_RELEASE(pLandScape);
		SAFE_RELEASE(pLandScapeObj);
	}
}


void CTerrainTab::OnBnClickedButtonTerrainLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

}

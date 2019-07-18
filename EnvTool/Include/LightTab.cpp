// CLightTab.cpp: 구현 파일
//

#include "stdafx.h"
#include "EnvTool.h"
#include "LightTab.h"
#include "afxdialogex.h"

#include "Component/Transform.h"
#include "Component/AxisLine.h"
#include "Component/Sphere.h"
#include "Component/Renderer.h"

#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/Layer.h"

#include "Core/PathManager.h"

// CLightTab 대화 상자
PG_USING

IMPLEMENT_DYNAMIC(CLightTab, CDialogEx)

CLightTab::CLightTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{

}

CLightTab::~CLightTab()
{
	SAFE_RELEASE(m_pGlobalLight);
	SAFE_RELEASE(m_pLightCam);
	SAFE_RELEASE(m_pLight);
}

void CLightTab::Process_ChangeTab()
{

}

void CLightTab::Process_ShowTab()
{

	
}

void CLightTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_POSX, m_ctrSliderGlobLightPosX);
	DDX_Control(pDX, IDC_SLIDER_POSY, m_ctrSliderGlobLightPosY);
	DDX_Control(pDX, IDC_SLIDER_POSZ, m_ctrSliderGlobLightPosZ);
	DDX_Control(pDX, IDC_EDIT_POSX, m_editGlobLightPosX);
	DDX_Control(pDX, IDC_EDIT_POSY, m_editGlobLightPosY);
	DDX_Control(pDX, IDC_EDIT9_POSZ, m_editGlobLightPosZ);
	DDX_Control(pDX, IDC_EDIT_GLOBLIGHT_RANGE, m_editGlobLightRange);
	DDX_Control(pDX, IDC_SLIDER_DIR_POSX, m_ctrSliderGlobLightLookPosX);
	DDX_Control(pDX, IDC_SLIDER_DIR_POSY, m_ctrSliderGlobLightLookPosY);
	DDX_Control(pDX, IDC_SLIDER_DIR_POSZ, m_ctrSliderGlobLightLookPosZ);
	DDX_Control(pDX, IDC_EDIT_DIR_POSX, m_editGlobLightLookPosX);
	DDX_Control(pDX, IDC_EDIT_DIR_POSY, m_editGlobLightLookPosY);
	DDX_Control(pDX, IDC_EDIT_DIR_POSZ, m_editGlobLightLookPosZ);
	DDX_Control(pDX, IDC_EDIT_GLOBLIGHT_R, m_editGlobLightR);
	DDX_Control(pDX, IDC_EDIT_GLOBLIGHT_G, m_editGlobLightG);
	DDX_Control(pDX, IDC_EDIT_GLOBLIGHT_B, m_editGlobLightB);
}


BEGIN_MESSAGE_MAP(CLightTab, CDialogEx)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_GLOBLIGHT_RANGE_ADJ, &CLightTab::OnBnClickedButtonGloblightRangeAdj)
	ON_BN_CLICKED(IDC_BUTTON_GLOBLIGHT_COLOR_ADJ, &CLightTab::OnBnClickedButtonGloblightColorAdj)
	ON_BN_CLICKED(IDC_BUTTON_GLOBLIGHT_RESET, &CLightTab::OnBnClickedButtonGloblightReset)
	ON_BN_CLICKED(IDC_BUTTON_GLOBLIGHT_SAVE, &CLightTab::OnBnClickedButtonGloblightSave)
	ON_BN_CLICKED(IDC_BUTTON_GLOBLIGHT_LOAD, &CLightTab::OnBnClickedButtonGloblightLoad)
END_MESSAGE_MAP()


// CLightTab 메시지 처리기

BOOL CLightTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer*	pLayer = pScene->GetLayer("Default");

	m_pGlobalLight = CGameObject::CreateObject("GlobalLightObject", pLayer);

	//?
	CRenderer* pRenderer = m_pGlobalLight->AddComponent<CRenderer>("Renderer");
	pRenderer->SetMesh("Sphere");
	pRenderer->SetShader(STANDARD_COLOR_SHADER);
	pRenderer->SetInputLayout("VertexColor");

	CSphere* pSphere = m_pGlobalLight->AddComponent<CSphere>("Sphere");
	m_pLightCam = pScene->GetLightCamera();
	CTransform *pLightCamTr = pScene->GetLightCameraTr();
	pSphere->SetObjMatrix(pLightCamTr->GetWorldMatrixPointer());

	SAFE_RELEASE(pSphere);
	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pLayer);
	
	CString num;

	// RGB
	m_pLight = pScene->GetGlobalLight("GlobalLight");
	m_vLightColor = m_pLight->GetLightColorDiffuse();
	num.Format(_T("%d"), int(m_vLightColor.x * 255));
	m_editGlobLightR.SetWindowTextW(num);
	num.Format(_T("%d"), int(m_vLightColor.y * 255));
	m_editGlobLightG.SetWindowTextW(num);
	num.Format(_T("%d"), int(m_vLightColor.z * 255));
	m_editGlobLightB.SetWindowTextW(num);

	// Range
	m_fRange = m_pLightCam->GetLightRange();
	num.Format(_T("%.2f"), m_fRange);
	m_editGlobLightRange.SetWindowTextW(num);
	
	// Eye
	m_ctrSliderGlobLightPosX.SetRange(-10000, 10000);
	m_ctrSliderGlobLightPosX.SetPos(0);
	m_ctrSliderGlobLightPosX.SetTicFreq(1);
	m_ctrSliderGlobLightPosX.SetLineSize(1);

	m_ctrSliderGlobLightPosY.SetRange(0, 10000);
	m_ctrSliderGlobLightPosY.SetPos(0);
	m_ctrSliderGlobLightPosY.SetTicFreq(1);
	m_ctrSliderGlobLightPosY.SetLineSize(1);

	m_ctrSliderGlobLightPosZ.SetRange(-10000, 10000);
	m_ctrSliderGlobLightPosZ.SetPos(0);
	m_ctrSliderGlobLightPosZ.SetTicFreq(1);
	m_ctrSliderGlobLightPosZ.SetLineSize(1);

	m_vCamPos = pLightCamTr->GetWorldPos();
	
	num.Format(_T("%.2f"), m_vCamPos.x);
	m_editGlobLightPosX.SetWindowTextW(num);
	m_ctrSliderGlobLightPosX.SetPos(int(m_vCamPos.x * 10.f));
	num.Format(_T("%.2f"), m_vCamPos.y);
	m_editGlobLightPosY.SetWindowTextW(num);
	m_ctrSliderGlobLightPosY.SetPos(int(m_vCamPos.y * 10.f));
	num.Format(_T("%.2f"), m_vCamPos.z);
	m_editGlobLightPosZ.SetWindowTextW(num);
	m_ctrSliderGlobLightPosZ.SetPos(int(m_vCamPos.z * 10.f));

	// Look
	m_ctrSliderGlobLightLookPosX.SetRange(-10000, 10000);
	m_ctrSliderGlobLightLookPosX.SetPos(0);
	m_ctrSliderGlobLightLookPosX.SetTicFreq(1);
	m_ctrSliderGlobLightLookPosX.SetLineSize(1);

	m_ctrSliderGlobLightLookPosY.SetRange(-10000, 10000);
	m_ctrSliderGlobLightLookPosY.SetPos(0);
	m_ctrSliderGlobLightLookPosY.SetTicFreq(1);
	m_ctrSliderGlobLightLookPosY.SetLineSize(1);

	m_ctrSliderGlobLightLookPosZ.SetRange(-10000, 10000);
	m_ctrSliderGlobLightLookPosZ.SetPos(0);
	m_ctrSliderGlobLightLookPosZ.SetTicFreq(1);
	m_ctrSliderGlobLightLookPosZ.SetLineSize(1);

	m_vCamLookPos = m_pLightCam->GetLightCenterPos();

	num.Format(_T("%.2f"), m_vCamLookPos.x);
	m_editGlobLightLookPosX.SetWindowTextW(num);
	m_ctrSliderGlobLightLookPosX.SetPos(int(m_vCamLookPos.x * 10.f));
	num.Format(_T("%.2f"), m_vCamLookPos.y);
	m_editGlobLightLookPosY.SetWindowTextW(num);
	m_ctrSliderGlobLightLookPosY.SetPos(int(m_vCamLookPos.y * 10.f));
	num.Format(_T("%.2f"), m_vCamLookPos.z);
	m_editGlobLightLookPosZ.SetWindowTextW(num);
	m_ctrSliderGlobLightLookPosZ.SetPos(int(m_vCamLookPos.z * 10.f));


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CLightTab::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_vCamPos.x = m_ctrSliderGlobLightPosX.GetPos() / 10.f;
	m_vCamPos.y = m_ctrSliderGlobLightPosY.GetPos() / 10.f;
	m_vCamPos.z = m_ctrSliderGlobLightPosZ.GetPos() / 10.f;

	CTransform *pTr = m_pLightCam->GetTransform();
	pTr->SetWorldPos(m_vCamPos);
	SAFE_RELEASE(pTr);

	CString num;

	num.Format(_T("%.2f"), m_vCamPos.x);
	m_editGlobLightPosX.SetWindowTextW(num);
	num.Format(_T("%.2f"), m_vCamPos.y);
	m_editGlobLightPosY.SetWindowTextW(num);
	num.Format(_T("%.2f"), m_vCamPos.z);
	m_editGlobLightPosZ.SetWindowTextW(num);

	m_vCamLookPos.x = m_ctrSliderGlobLightLookPosX.GetPos() / 10.f;
	m_vCamLookPos.y = m_ctrSliderGlobLightLookPosY.GetPos() / 10.f;
	m_vCamLookPos.z = m_ctrSliderGlobLightLookPosZ.GetPos() / 10.f;

	num.Format(_T("%.2f"), m_vCamLookPos.x);
	m_editGlobLightLookPosX.SetWindowTextW(num);
	num.Format(_T("%.2f"), m_vCamLookPos.y);
	m_editGlobLightLookPosY.SetWindowTextW(num);
	num.Format(_T("%.2f"), m_vCamLookPos.z);
	m_editGlobLightLookPosZ.SetWindowTextW(num);

	m_pLightCam->SetLightCenterPos(m_vCamLookPos);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CLightTab::OnBnClickedButtonGloblightRangeAdj()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString range;
	m_editGlobLightRange.GetWindowTextW(range);
	m_fRange = (float)_wtof(range);

	m_pLightCam->SetLightRange(m_fRange);
}


void CLightTab::OnBnClickedButtonGloblightColorAdj()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString num;
	m_editGlobLightR.GetWindowTextW(num);
	m_vLightColor.x = _wtoi(num) / 255.f;
	m_editGlobLightG.GetWindowTextW(num);
	m_vLightColor.y = _wtoi(num) / 255.f;
	m_editGlobLightB.GetWindowTextW(num);
	m_vLightColor.z = _wtoi(num) / 255.f;

	m_pLight->SetLightColor(m_vLightColor);
}

void CLightTab::SetAllByMemberValue()
{
	// range
	CString num;
	num.Format(_T("%.2f"), m_fRange);
	m_editGlobLightRange.SetWindowTextW(num);
	m_pLightCam->SetLightRange(m_fRange);
	// color
	num.Format(_T("%d"), int(m_vLightColor.x * 255.f));
	m_editGlobLightR.SetWindowTextW(num);
	num.Format(_T("%d"), int(m_vLightColor.y * 255.f));
	m_editGlobLightG.SetWindowTextW(num);
	num.Format(_T("%d"), int(m_vLightColor.z * 255.f));
	m_editGlobLightB.SetWindowTextW(num);
	m_pLight->SetLightColor(m_vLightColor);
	// campos
	num.Format(_T("%.2f"), m_vCamPos.x);
	m_editGlobLightPosX.SetWindowTextW(num);
	m_ctrSliderGlobLightPosX.SetPos(int(m_vCamPos.x * 10.f));
	num.Format(_T("%.2f"), m_vCamPos.y);
	m_editGlobLightPosY.SetWindowTextW(num);
	m_ctrSliderGlobLightPosY.SetPos(int(m_vCamPos.y * 10.f));
	num.Format(_T("%.2f"), m_vCamPos.z);
	m_editGlobLightPosZ.SetWindowTextW(num);
	m_ctrSliderGlobLightPosZ.SetPos(int(m_vCamPos.z * 10.f));
	CTransform *pTr = m_pLightCam->GetTransform();
	pTr->SetWorldPos(m_vCamPos);
	SAFE_RELEASE(pTr);
	// camlookpos
	num.Format(_T("%.2f"), m_vCamLookPos.x);
	m_editGlobLightLookPosX.SetWindowTextW(num);
	m_ctrSliderGlobLightLookPosX.SetPos(int(m_vCamLookPos.x * 10.f));
	num.Format(_T("%.2f"), m_vCamLookPos.y);
	m_editGlobLightLookPosY.SetWindowTextW(num);
	m_ctrSliderGlobLightLookPosY.SetPos(int(m_vCamLookPos.y * 10.f));
	num.Format(_T("%.2f"), m_vCamLookPos.z);
	m_editGlobLightLookPosZ.SetWindowTextW(num);
	m_ctrSliderGlobLightLookPosZ.SetPos(int(m_vCamLookPos.z * 10.f));
	m_pLightCam->SetLightCenterPos(m_vCamLookPos);
}


void CLightTab::OnBnClickedButtonGloblightReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// range
	m_fRange = 425.f;
	// color
	m_vLightColor = Vector4(1.f, 1.f, 1.f, 1.f);
	// campos
	m_vCamPos = Vector3(-512.f / 8.f, 400.f, -512.f / 8.f);
	// camlookpos
	m_vCamLookPos = Vector3{ 512.f / 2.f, 0.f, 512.f / 2.f };

	SetAllByMemberValue();
}


void CLightTab::OnBnClickedButtonGloblightSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static TCHAR BASED_CODE szFilter[] =
		_T("데이터 파일(*.bin) | *.bin;|모든파일(*.*)|*.*||");
	CFileDialog dlg(FALSE, NULL, NULL, OFN_OVERWRITEPROMPT, szFilter);

	// 경로 지정
	wchar_t strPath[MAX_PATH] = {};
	wchar_t strDir[MAX_PATH] = {};
	wcscpy_s(strPath, MAX_PATH, GET_SINGLE(CPathManager)->FindPath(DATA_PATH));
	wcscpy_s(strDir, MAX_PATH, GET_SINGLE(CPathManager)->FindPath(DATA_PATH));
	wcscat_s(strPath, MAX_PATH, L"Light\\Glob\\");

	CString originPath = strPath;

	dlg.m_ofn.lpstrInitialDir = strPath;

	// do modal error 해결
	if (dlg.DoModal() != IDOK)
		return;

	CString path = dlg.GetPathName();
	CString fileName = dlg.GetFileName();
	CString flexiblePath = L"Light\\Glob\\";

	// 파일 이름 제거
	for (int i = lstrlen(path) - 1; i >= 0; i--)
	{
		if (path[i] == '\\')
		{
			path.Delete(i + 1, lstrlen(path) - 1);
			break;
		}
	}

	// 확장자명 제거(만약 확장자명이 세이브 이름으로 들어갈시)
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

	ofstream mainFile;
	mainFile.open(strFilePath + strFileName + ".bin", ios::out | ios::trunc);

	mainFile << m_fRange << endl;
	mainFile << m_vLightColor.x << ' ' << m_vLightColor.y << ' ' << m_vLightColor.z << endl;
	mainFile << m_vCamPos.x << ' ' << m_vCamPos.y << ' ' << m_vCamPos.z << endl;
	mainFile << m_vCamLookPos.x << ' ' << m_vCamLookPos.y << ' ' << m_vCamLookPos.z << endl;
	
	mainFile.close();
}


void CLightTab::OnBnClickedButtonGloblightLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static TCHAR BASED_CODE szFilter[] =
		_T("데이터 파일(*.bin) | *.bin;|모든파일(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_READONLY | OFN_OVERWRITEPROMPT, szFilter);

	// 경로 지정
	wchar_t strPath[MAX_PATH] = {};
	wcscpy_s(strPath, MAX_PATH, GET_SINGLE(CPathManager)->FindPath(DATA_PATH));
	wcscat_s(strPath, MAX_PATH, L"Light\\Glob\\");

	CString originPath = strPath;

	dlg.m_ofn.lpstrInitialDir = strPath;

	// do modal error 해결
	if (dlg.DoModal() != IDOK)
		return;

	CString filePath = dlg.GetPathName();

	ifstream mainFile;
	mainFile.open(filePath, ios::in);

	if (!mainFile.is_open())
		return;

	mainFile >> m_fRange;
	mainFile >> m_vLightColor.x >> m_vLightColor.y >> m_vLightColor.z;
	mainFile >> m_vCamPos.x >> m_vCamPos.y >> m_vCamPos.z;
	mainFile >> m_vCamLookPos.x >> m_vCamLookPos.y >> m_vCamLookPos.z;

	SetAllByMemberValue();

	mainFile.close();
}

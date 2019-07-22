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
	SAFE_RELEASE(m_pLight_point);
	SAFE_RELEASE(m_pPointLight);
}

void CLightTab::Process_ChangeTab()
{
	CSphere* pSphere = m_pGlobalLight->FindComponentFromTag<CSphere>("Sphere");
	pSphere->SetRenderCheck(false);
	SAFE_RELEASE(pSphere);

	pSphere = m_pPointLight->FindComponentFromTag<CSphere>("Sphere");
	pSphere->SetRenderCheck(false);
	SAFE_RELEASE(pSphere);

	m_pLight_point->SetLightRange(0.f);
}

void CLightTab::Process_ShowTab()
{
	CSphere* pSphere = m_pGlobalLight->FindComponentFromTag<CSphere>("Sphere");
	pSphere->SetRenderCheck(true);
	SAFE_RELEASE(pSphere);

	pSphere = m_pPointLight->FindComponentFromTag<CSphere>("Sphere");
	pSphere->SetRenderCheck(true);
	SAFE_RELEASE(pSphere);

	m_pLight_point->SetLightRange(m_fPointRange);
}

void CLightTab::AddPointLight()
{
	CScene *pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer *pLayer = pScene->GetLayer("Default");

	CString lightName;
	lightName.Format(_T("%d"), m_iPointNum);

	lightName = L"Light_" + lightName;
	m_listPointLightList.AddString(lightName);

	CLight *pPointLight = pScene->CreateLight(string(CT2CA(lightName)), LT_POINT);
	pPointLight->SetLightRange(m_fPointRange);
	pPointLight->SetLightColor(m_vPointLightColor);
	CTransform *pTempPointLightTr = m_pLight_point->GetTransform();
	CTransform *pPointLightTr = pPointLight->GetTransform();
	pPointLightTr->SetWorldPos(pTempPointLightTr->GetWorldPos());

	CGameObject *pLightObj = CGameObject::CreateObject(string(CT2CA(lightName)), pLayer);
	
	//?
	CRenderer* pRenderer = pLightObj->AddComponent<CRenderer>("Renderer");
	pRenderer->SetMesh("Sphere");
	pRenderer->SetShader(STANDARD_COLOR_SHADER);
	pRenderer->SetInputLayout("VertexColor");

	CSphere *pSphere = pLightObj->AddComponent<CSphere>("Sphere");
	pSphere->SetRenderCheck(true);
	pSphere->SetSize(Vector3(1.f, 1.f, 1.f));
	pSphere->SetObjMatrix(pPointLightTr->GetWorldMatrixPointer());
	pSphere->SetColor(m_vPointLightColor);

	PLINFO PointLightInfo;
	PointLightInfo.strLightName = string(CT2CA(lightName));
	PointLightInfo.vPos = pTempPointLightTr->GetWorldPos();
	PointLightInfo.fRange = m_fPointRange;
	PointLightInfo.vColor = m_vPointLightColor;

	m_PointLightInfos.push_back(PointLightInfo);

	SAFE_RELEASE(pTempPointLightTr);
	SAFE_RELEASE(pPointLightTr);
	SAFE_RELEASE(pPointLight);
	SAFE_RELEASE(pLightObj);
	SAFE_RELEASE(pSphere);
	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pScene);
	
	++m_iPointNum;
}

void CLightTab::UndoPointLight()
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
	DDX_Control(pDX, IDC_LIST_POINTLIGHT, m_listPointLightList);
	DDX_Control(pDX, IDC_EDIT_POINT_RANGE, m_editPointLightRange);
	DDX_Control(pDX, IDC_EDIT_POINT_R, m_editPointLightR);
	DDX_Control(pDX, IDC_EDIT_POINT_G, m_editPointLightG);
	DDX_Control(pDX, IDC_EDIT_POINT_B, m_editPointLightB);
}


BEGIN_MESSAGE_MAP(CLightTab, CDialogEx)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_GLOBLIGHT_RANGE_ADJ, &CLightTab::OnBnClickedButtonGloblightRangeAdj)
	ON_BN_CLICKED(IDC_BUTTON_GLOBLIGHT_COLOR_ADJ, &CLightTab::OnBnClickedButtonGloblightColorAdj)
	ON_BN_CLICKED(IDC_BUTTON_GLOBLIGHT_RESET, &CLightTab::OnBnClickedButtonGloblightReset)
	ON_BN_CLICKED(IDC_BUTTON_GLOBLIGHT_SAVE, &CLightTab::OnBnClickedButtonGloblightSave)
	ON_BN_CLICKED(IDC_BUTTON_GLOBLIGHT_LOAD, &CLightTab::OnBnClickedButtonGloblightLoad)
	ON_BN_CLICKED(IDC_BUTTON_POINT_RANGE_ADJ, &CLightTab::OnBnClickedButtonPointRangeAdj)
	ON_BN_CLICKED(IDC_EDIT_POINT_COLOR_ADJ, &CLightTab::OnBnClickedEditPointColorAdj)
	ON_BN_CLICKED(IDC_BUTTON_POINT_LIST_CLEARALL, &CLightTab::OnBnClickedButtonPointListClearall)
	ON_BN_CLICKED(IDC_BUTTON_POINT_SAVE, &CLightTab::OnBnClickedButtonPointSave)
	ON_BN_CLICKED(IDC_BUTTON_POINT_LOAD, &CLightTab::OnBnClickedButtonPointLoad)
END_MESSAGE_MAP()


// CLightTab 메시지 처리기

BOOL CLightTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer*	pLayer = pScene->GetLayer("Default");

	// GlobLight
	m_pGlobalLight = CGameObject::CreateObject("GlobalLightObject", pLayer);

	//? 필요 없는데...
	CRenderer* pRenderer = m_pGlobalLight->AddComponent<CRenderer>("Renderer");
	pRenderer->SetMesh("Sphere");
	pRenderer->SetShader(STANDARD_COLOR_SHADER);
	pRenderer->SetInputLayout("VertexColor");

	CSphere* pSphere = m_pGlobalLight->AddComponent<CSphere>("Sphere");
	pSphere->SetSize(Vector3(100.f, 100.f, 100.f));
	m_pLightCam = pScene->GetLightCamera();
	CTransform *pLightCamTr = pScene->GetLightCameraTr();
	pSphere->SetObjMatrix(pLightCamTr->GetWorldMatrixPointer());

	SAFE_RELEASE(pSphere);
	SAFE_RELEASE(pRenderer);
	
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

	SAFE_RELEASE(pLightCamTr);

	// PointLight
	m_pLight_point = pScene->CreateLight("TempPointLight", LT_POINT);
	m_pLight_point->SetLightRange(10.f);
	
	CTransform* pLightTr = m_pLight_point->GetTransform();

	m_pPointLight = CGameObject::CreateObject("TempPointLight", pLayer);

	pRenderer = m_pPointLight->AddComponent<CRenderer>("Renderer");
	pRenderer->SetMesh("Sphere");
	pRenderer->SetShader(STANDARD_COLOR_SHADER);
	pRenderer->SetInputLayout("VertexColor");
	SAFE_RELEASE(pRenderer);

	pSphere = m_pPointLight->AddComponent<CSphere>("Sphere");
	pSphere->SetSize(Vector3(0.5f, 0.5f, 0.5f));
	pSphere->SetObjMatrix(pLightTr->GetWorldMatrixPointer());

	SAFE_RELEASE(pLightTr);


	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pScene);

	num.Format(_T("%.2f"), m_fPointRange);
	m_editPointLightRange.SetWindowTextW(num);

	m_vPointLightColor = m_pLight_point->GetLightColorDiffuse();
	num.Format(_T("%d"), int(m_vPointLightColor.x * 255));
	m_editPointLightR.SetWindowTextW(num);
	num.Format(_T("%d"), int(m_vPointLightColor.y * 255));
	m_editPointLightG.SetWindowTextW(num);
	num.Format(_T("%d"), int(m_vPointLightColor.z * 255));
	m_editPointLightB.SetWindowTextW(num);

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

	CSphere *pSphere = m_pGlobalLight->FindComponentFromTag<CSphere>("Sphere");
	pSphere->SetColor(m_vLightColor);
	SAFE_RELEASE(pSphere);
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

	CSphere *pSphere = m_pGlobalLight->FindComponentFromTag<CSphere>("Sphere");
	pSphere->SetColor(m_vLightColor);
	SAFE_RELEASE(pSphere);
}

void CLightTab::SetPointLightPosXZ(const Vector3 & pos)
{
	CTransform* pLightTr = m_pLight_point->GetTransform();
	pLightTr->SetWorldPos(pos.x, m_fHeight, pos.z);
	SAFE_RELEASE(pLightTr);
}

void CLightTab::SetPointLightHeight(const float & height)
{
	m_fHeight += height;

	CTransform* pLightTr = m_pLight_point->GetTransform();
	Vector3 vLightPos = pLightTr->GetWorldPos();

	pLightTr->SetWorldPos(vLightPos.x, m_fHeight, vLightPos.z);
	SAFE_RELEASE(pLightTr);
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


void CLightTab::OnBnClickedButtonPointRangeAdj()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString range;

	m_editPointLightRange.GetWindowTextW(range);
	m_fPointRange = (float)_wtof(range);
	m_pLight_point->SetLightRange(m_fPointRange);
}


void CLightTab::OnBnClickedEditPointColorAdj()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString R, G, B;

	m_editPointLightR.GetWindowTextW(R);
	m_editPointLightG.GetWindowTextW(G);
	m_editPointLightB.GetWindowTextW(B);
	m_vPointLightColor.x = (float)_wtof(R) / 255.f;
	m_vPointLightColor.y = (float)_wtof(G) / 255.f;
	m_vPointLightColor.z = (float)_wtof(B) / 255.f;
	m_pLight_point->SetLightColor(m_vPointLightColor);

	CSphere *pSphere = m_pPointLight->FindComponentFromTag<CSphere>("Sphere");
	pSphere->SetColor(m_vLightColor);
	SAFE_RELEASE(pSphere);
}


void CLightTab::OnBnClickedButtonPointListClearall()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int count = m_listPointLightList.GetCount();

	CScene *pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();

	for (int i = 0; i < count; ++i)
	{
		CString lightName;
		m_listPointLightList.GetText(i, lightName);
		pScene->DeleteLight((string)CT2CA(lightName.GetString()));

		CGameObject *pLightObj = CGameObject::FindObject((string)CT2CA(lightName.GetString()));
		pLightObj->Die();
		CGameObject::EraseObj(pLightObj);
	}

	m_listPointLightList.ResetContent();
	m_PointLightInfos.clear();
	m_iPointNum = 0;

	SAFE_RELEASE(pScene);
}


void CLightTab::OnBnClickedButtonPointSave()
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
	wcscat_s(strPath, MAX_PATH, L"Light\\Point\\");

	CString originPath = strPath;

	dlg.m_ofn.lpstrInitialDir = strPath;

	// do modal error 해결
	if (dlg.DoModal() != IDOK)
		return;

	CString path = dlg.GetPathName();
	CString fileName = dlg.GetFileName();
	CString flexiblePath = L"Light\\Point\\";

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

	// 라이트 갯수
	int count = m_PointLightInfos.size();
	mainFile << count << endl;

	for (auto& iter : m_PointLightInfos)
	{
		// 라이트 이름
		mainFile << iter.strLightName << endl;
		// 라이트 위치
		mainFile << iter.vPos.x << ' ' << iter.vPos.y << ' ' << iter.vPos.z << endl;
		// 라이트 레인지
		mainFile << iter.fRange << endl;
		// 라이트 색깔
		mainFile << iter.vColor.x << ' ' << iter.vColor.y << ' ' << iter.vColor.z << ' ' << iter.vColor.w << endl;
	}
}


void CLightTab::OnBnClickedButtonPointLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static TCHAR BASED_CODE szFilter[] =
		_T("데이터 파일(*.bin) | *.bin;|모든파일(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_READONLY | OFN_OVERWRITEPROMPT, szFilter);

	// 경로 지정
	wchar_t strPath[MAX_PATH] = {};
	wcscpy_s(strPath, MAX_PATH, GET_SINGLE(CPathManager)->FindPath(DATA_PATH));
	wcscat_s(strPath, MAX_PATH, L"Light\\Point\\");

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

	// 라이트 갯수
	int count = 0;
	mainFile >> count;

	// 다 지우고
	OnBnClickedButtonPointListClearall();

	CScene *pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer *pLayer = pScene->GetLayer("Default");

	for (int i = 0; i < count; ++i)
	{
		PLINFO PointLightInfo;
		mainFile >> PointLightInfo.strLightName;
		mainFile >> PointLightInfo.vPos.x >> PointLightInfo.vPos.y >> PointLightInfo.vPos.z;
		mainFile >> PointLightInfo.fRange;
		mainFile >> PointLightInfo.vColor.x >> PointLightInfo.vColor.y >> PointLightInfo.vColor.z >> PointLightInfo.vColor.w;

		CString lightName;
		lightName = PointLightInfo.strLightName.c_str();
		m_listPointLightList.AddString(lightName);

		CLight *pPointLight = pScene->CreateLight(string(CT2CA(lightName)), LT_POINT);
		pPointLight->SetLightRange(PointLightInfo.fRange);
		pPointLight->SetLightColor(PointLightInfo.vColor);
		CTransform *pPointLightTr = pPointLight->GetTransform();
		pPointLightTr->SetWorldPos(PointLightInfo.vPos);

		CGameObject *pLightObj = CGameObject::CreateObject(string(CT2CA(lightName)), pLayer);
		//? 필요 없는데...
		CRenderer* pRenderer = pLightObj->AddComponent<CRenderer>("Renderer");
		pRenderer->SetMesh("Sphere");
		pRenderer->SetShader(STANDARD_COLOR_SHADER);
		pRenderer->SetInputLayout("VertexColor");

		CSphere *pSphere = pLightObj->AddComponent<CSphere>("Sphere");
		pSphere->SetRenderCheck(true);
		pSphere->SetSize(Vector3(1.5f, 1.5f, 1.5f));
		pSphere->SetObjMatrix(pPointLightTr->GetWorldMatrixPointer());
		pSphere->SetColor(PointLightInfo.vColor);

		m_PointLightInfos.push_back(PointLightInfo);

		SAFE_RELEASE(pPointLightTr);
		SAFE_RELEASE(pPointLight);
		SAFE_RELEASE(pLightObj);
		SAFE_RELEASE(pSphere);

		++m_iPointNum;
	}

	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pScene);
	
}

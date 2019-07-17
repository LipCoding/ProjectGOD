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
}


BEGIN_MESSAGE_MAP(CLightTab, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CLightTab 메시지 처리기

BOOL CLightTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CScene *pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer*	pLayer = pScene->GetLayer("Default");

	m_pGlobalLight = CGameObject::CreateObject("GlobalLightObject", pLayer);

	//?
	CRenderer* pRenderer = m_pGlobalLight->AddComponent<CRenderer>("Renderer");
	pRenderer->SetMesh("Sphere");
	pRenderer->SetShader(STANDARD_COLOR_SHADER);
	pRenderer->SetInputLayout("VertexColor");

	CSphere *pSphere = m_pGlobalLight->AddComponent<CSphere>("Sphere");
	m_pLightCam = pScene->GetLightCamera();
	CTransform *pLightCamTr = pScene->GetLightCameraTr();
	pSphere->SetObjMatrix(pLightCamTr->GetWorldMatrixPointer());

	SAFE_RELEASE(pSphere);
	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pLayer);
	
	//

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

	Vector3 vLightCamPos = pLightCamTr->GetWorldPos();
	
	CString num;

	num.Format(_T("%f"), vLightCamPos.x);
	m_editGlobLightPosX.SetWindowTextW(num);
	
	m_ctrSliderGlobLightPosX.SetPos(int(vLightCamPos.x * 10.f));

	num.Format(_T("%f"), vLightCamPos.y);
	m_editGlobLightPosY.SetWindowTextW(num);

	m_ctrSliderGlobLightPosY.SetPos(int(vLightCamPos.y * 10.f));

	num.Format(_T("%f"), vLightCamPos.z);
	m_editGlobLightPosZ.SetWindowTextW(num);
	
	m_ctrSliderGlobLightPosZ.SetPos(int(vLightCamPos.z * 10.f));


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CLightTab::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	float fPosX = m_ctrSliderGlobLightPosX.GetPos() / 10.f;
	float fPosY = m_ctrSliderGlobLightPosY.GetPos() / 10.f;
	float fPosZ = m_ctrSliderGlobLightPosZ.GetPos() / 10.f;

	CTransform *pTr = m_pLightCam->GetTransform();
	pTr->SetWorldPos(fPosX, fPosY, fPosZ);

	SAFE_RELEASE(pTr);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
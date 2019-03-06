
// AnimToolView.cpp: CAnimToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "AnimTool.h"
#endif

#include "AnimToolDoc.h"
#include "AnimToolView.h"
#include "Core.h"
#include "Core/Input.h"
#include "Device.h"
#include "Scene/Scene.h"
#include "Scene/Layer.h"
#include "Scene/SceneManager.h"
#include "Component/Renderer.h"
#include "Core/TimerManager.h"
#include "Component/ColliderSphere.h"
#include "MainFrm.h"
#include "Component/Camera.h"
#include "Component/Material.h"
#include "Resources/ResourcesManager.h"
#include "Component/LandScape.h"
#include "Component/Transform.h"
#include "Core/PathManager.h"
#include "Component/Arm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAnimToolView

IMPLEMENT_DYNCREATE(CAnimToolView, CView)

BEGIN_MESSAGE_MAP(CAnimToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CAnimToolView 생성/소멸

CAnimToolView::CAnimToolView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CAnimToolView::~CAnimToolView()
{
	SAFE_RELEASE(m_pCamera);
	SAFE_RELEASE(m_pCamTr);
	SAFE_RELEASE(m_pTimer);
	DESTROY_SINGLE(CCore);
}

BOOL CAnimToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CAnimToolView 그리기

void CAnimToolView::OnDraw(CDC* /*pDC*/)
{
	CAnimToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CAnimToolView 인쇄

BOOL CAnimToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CAnimToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CAnimToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CAnimToolView 진단

#ifdef _DEBUG
void CAnimToolView::AssertValid() const
{
	CView::AssertValid();
}

void CAnimToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAnimToolDoc* CAnimToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAnimToolDoc)));
	return (CAnimToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CAnimToolView 메시지 처리기

void CAnimToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CWnd *pWnd = AfxGetMainWnd();
	HWND  hWnd = pWnd->m_hWnd;

	// 엔진 초기화
	if (!GET_SINGLE(CCore)->Init(AfxGetInstanceHandle(), m_hWnd, 1400, 900, true, true, false))
		return;


	// Timer
	m_pTimer = GET_SINGLE(CTimerManager)->FindTimer("MainThread");

	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer* pLayer = pScene->GetLayer("Default");

	// SkyBox
	CGameObject* pSky = CGameObject::FindObject("Sky");
	CRenderer*   pRenderer = pSky->FindComponentFromTag<CRenderer>("SkyRenderer");
	CMaterial*   pMaterial = pRenderer->GetMaterial();

	pMaterial->ResetTextureInfo();
	GET_SINGLE(CResourcesManager)->FindAndDeleteTexture("Sky");
	pMaterial->SetDiffuseTexInfo(SAMPLER_LINEAR, "Sky", 0, 0, L"Skybox\\Skybox_6.dds");

	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pSky);

	// Load Terrain
	CGameObject* pLandScapeObj = CGameObject::CreateObject("LandScape", pLayer);
	CLandScape* pLandScape = pLandScapeObj->AddComponent<CLandScape>("LandScape");

	pLandScape->Load_Terrain("AnimTool");

	SAFE_RELEASE(pLandScape);
	SAFE_RELEASE(pLandScapeObj);

	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pScene);
}

int CAnimToolView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	return 0;
}

void CAnimToolView::UpdateView()
{
	float fTime = m_pTimer->GetDeltaTime();

	UpdateInput(fTime);
	UpdateObject(fTime);
}

void CAnimToolView::UpdateInput(const float & fTime)
{
	if (KEYPUSH("MoveFront"))
	{
	}

	if (KEYPUSH("MoveBack"))
	{
	}

	if (KEYPUSH("MoveLeft"))
	{
	}

	if (KEYPUSH("MoveRight"))
	{
	}

	if (KEYPUSH("MouseLButton"))
	{
	}

	if (KEYPUSH("MouseRButton"))
	{
	}

	if (KEYPUSH("MouseMButton"))
	{
	}

	if (GET_SINGLE(CInput)->GetWheel() == 1)
	{
		CGameObject* pMeshObj = CGameObject::FindObject("Mesh");

		if (pMeshObj)
		{
			CTransform *pTransform = pMeshObj->GetTransform();
			CArm* pArm = pMeshObj->FindComponentFromTag<CArm>("Arm");
			pArm->Zoom(pTransform->GetLocalAxis(AXIS_Z), 3.f);

			SAFE_RELEASE(pTransform);
			SAFE_RELEASE(pArm);
			SAFE_RELEASE(pMeshObj);
		}
	}

	if (GET_SINGLE(CInput)->GetWheel() == -1)
	{
		CGameObject* pMeshObj = CGameObject::FindObject("Mesh");

		if (pMeshObj)
		{
			CTransform *pTransform = pMeshObj->GetTransform();
			CArm* pArm = pMeshObj->FindComponentFromTag<CArm>("Arm");
			pArm->Zoom(pTransform->GetLocalAxis(AXIS_Z), -3.f);

			SAFE_RELEASE(pTransform);
			SAFE_RELEASE(pArm);
			SAFE_RELEASE(pMeshObj);
		}
	}
}

void CAnimToolView::UpdateObject(const float & fTime)
{
}



BOOL CAnimToolView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	GET_SINGLE(CInput)->SetWheel(zDelta);


	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

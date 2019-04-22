
// EffectToolView.cpp: CEffectToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "EffectTool.h"
#endif

#include "EffectToolDoc.h"
#include "EffectToolView.h"
#include "EditForm.h"
#include "EffectTab.h"
#include "EffectTab1.h"

#include "Core.h"
#include "Core/Input.h"
#include "Device.h"
#include "Scene/Scene.h"
#include "Scene/Layer.h"
#include "Scene/SceneManager.h"
#include "Component/Renderer.h"
#include "Core/TimerManager.h"
#include "Component/LandScape.h"
#include "Component/Picking.h"
#include "Component/ColliderSphere.h"
#include "MainFrm.h"
#include "Scene/Scene.h"
#include "Component/Camera.h"

#include "Resources/ResourcesManager.h"
#include "Component/Material.h"
#include "Component/ColliderRay.h"
#include "Component/ColliderSphere.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEffectToolView

IMPLEMENT_DYNCREATE(CEffectToolView, CView)

BEGIN_MESSAGE_MAP(CEffectToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CEffectToolView 생성/소멸

CEffectToolView::CEffectToolView() noexcept :
	m_pCamera(nullptr)
	, m_pCamTr(nullptr)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CEffectToolView::~CEffectToolView()
{
	SAFE_RELEASE(m_pTimer);
	DESTROY_SINGLE(CCore);

	SAFE_RELEASE(m_pCamTr);
	SAFE_RELEASE(m_pCamera);
}

BOOL CEffectToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CEffectToolView 그리기

void CEffectToolView::OnDraw(CDC* /*pDC*/)
{
	CEffectToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CEffectToolView 인쇄

BOOL CEffectToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CEffectToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CEffectToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CEffectToolView 진단

#ifdef _DEBUG
void CEffectToolView::AssertValid() const
{
	CView::AssertValid();
}

void CEffectToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEffectToolDoc* CEffectToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEffectToolDoc)));
	return (CEffectToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CEffectToolView 메시지 처리기


void CEffectToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.


	CWnd * pWnd = AfxGetMainWnd();
	HWND hWnd = pWnd->m_hWnd;

	// 엔진 초기화
	if (!GET_SINGLE(CCore)->Init(AfxGetInstanceHandle(), m_hWnd, 1400, 1000, true, true, false))
		return;

	m_pTimer = GET_SINGLE(CTimerManager)->FindTimer("MainThread");

	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	pScene->CreateLayer("ParticleLayer", 2000);
	CLayer* pLayer = pScene->GetLayer("Default");

	//Camera
	m_pCamera = GET_SINGLE(CSceneManager)->GetCurrentScene()->GetMainCameraObj();
	m_pCamTr = GET_SINGLE(CSceneManager)->GetCurrentScene()->GetMainCameraTr();

	m_pCamTr->SetWorldPos(50.f / 2.f, 2.f, 50.f / 2.f - 5.f);

	// SkyBox
	CGameObject* pSky = CGameObject::FindObject("Sky");
	if (pSky)
	{
		CRenderer*   pRenderer = pSky->FindComponentFromTag<CRenderer>("SkyRenderer");
		CMaterial*   pMaterial = pRenderer->GetMaterial();

		pMaterial->ResetTextureInfo();
		GET_SINGLE(CResourcesManager)->FindAndDeleteTexture("Sky");
		pMaterial->SetDiffuseTexInfo(SAMPLER_LINEAR, "Sky", 0, 0, L"Skybox\\Skybox_2.dds");

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pSky);
	}

	// Load Terrain
	CGameObject* pLandScapeObj = CGameObject::CreateObject("LandScape", pLayer);
	CLandScape* pLandScape = pLandScapeObj->AddComponent<CLandScape>("LandScape");
	pLandScape->Load_Terrain("EffectTool1");

	/* Wire mode */
	for (auto& node : *pLandScape->GetAllNodes())
	{
		CRenderer*   pRenderer = node->pGameObject->FindComponentFromType<CRenderer>(CT_RENDERER);
		pRenderer->SetRenderState(WIRE_FRAME);
		//pRenderer->SetRenderState(CULLING_NONE);
		SAFE_RELEASE(pRenderer);
	}

	// Mouse Picking
	CPicking* pPicking = pLandScapeObj->AddComponent<CPicking>("Picking");

	SAFE_RELEASE(pPicking);
	SAFE_RELEASE(pLandScape);
	SAFE_RELEASE(pLandScapeObj);

	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pScene);
}

void CEffectToolView::UpdateView()
{
	float	fTime = m_pTimer->GetDeltaTime();

	this->UpdateInput(fTime);
	this->UpdateForm(fTime);
	this->UpdateObject(fTime);
}

void CEffectToolView::UpdateInput(const float & fTime)
{
	if (KEYPUSH("MoveFront"))
	{
		m_pCamTr->MoveWorld(AXIS_Z, 30 * 0.5f, fTime);
	}

	if (KEYPUSH("MoveBack"))
	{
		m_pCamTr->MoveWorld(AXIS_Z, -30 * 0.5f, fTime);
	}

	if (KEYPUSH("MoveLeft"))
	{
		m_pCamTr->MoveWorld(AXIS_X, -30 * 0.5f, fTime);
	}

	if (KEYPUSH("MoveRight"))
	{
		m_pCamTr->MoveWorld(AXIS_X, 30 * 0.5f, fTime);
	}

	if (KEYDOWN("MouseRButton"))
	{
	}

	if (KEYUP("MouseRButton"))
	{
	}

	if (KEYPUSH("MouseRButton"))
	{
		PickingProcess();

		if (m_pCollideObject)
		{
			CTransform *pTr = m_pCollideObject->GetTransform();
			
			Vector3 vWorldPos = pTr->GetWorldPos();
			
			vWorldPos.x = m_vPickPos.x;
			vWorldPos.z = m_vPickPos.z;

			pTr->SetWorldPos(vWorldPos);

			SAFE_RELEASE(pTr);
		}
	}

	if (KEYPUSH("MouseMButton"))
	{
		POINT	ptMouse = GET_SINGLE(CInput)->GetMousePos();
		POINT   ptMouseMove = GET_SINGLE(CInput)->GetMouseMove();

		if (ptMouseMove.x != 0)
		{
			float fAngle = ptMouseMove.x / 2000.f * PG_PI;

			m_pCamTr->RotateWorldY(fAngle);
		}

		if (ptMouseMove.y != 0)
		{
			float fAngle = ptMouseMove.y / 2000.f * PG_PI;

			m_pCamTr->RotateWorldX(fAngle);
		}
	}
}

void CEffectToolView::UpdateObject(const float & fTime)
{
	
}

void CEffectToolView::UpdateForm(const float & fTime)
{
	((CMainFrame*)AfxGetMainWnd())->GetEdit()->UpdateForm();
}

void CEffectToolView::PickingProcess()
{
	CGameObject* pLandScapeObj = CGameObject::FindObject("LandScape");
	if (pLandScapeObj != NULL)
	{
		CLandScape* pLandScape = pLandScapeObj->FindComponentFromTag<CLandScape>("LandScape");
		CPicking* pPicking = pLandScapeObj->FindComponentFromTag<CPicking>("Picking");

		list<QUADTREENODE*>* pNodes = pLandScape->FindNode_ByMouse();

		bool bFirstCheck = false;

		if (!pNodes->empty())
		{
			for (const auto iter : *pNodes)
			{
				if (pPicking->Picking_ToBuffer(&m_vPickPos,
					GET_SINGLE(CInput)->GetRayPos(),
					GET_SINGLE(CInput)->GetRayDir(),
					iter->vecVtx, iter->vecIndex))
				{
					break;
				}
			}
		}

		SAFE_RELEASE(pPicking);
		SAFE_RELEASE(pLandScape);
		SAFE_RELEASE(pLandScapeObj);
	}
}


int CEffectToolView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}


BOOL CEffectToolView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	GET_SINGLE(CInput)->SetWheel(zDelta);

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CEffectToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	

	CView::OnLButtonDown(nFlags, point);
}


void CEffectToolView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CGameObject *pMouseObj = GET_SINGLE(CInput)->GetMouseObj();
	CColliderRay *pRay = pMouseObj->FindComponentFromTag<CColliderRay>("MouseRay");
	
	if (((CMainFrame*)AfxGetMainWnd())->GetEdit()->GetEffectTab()->GetTargetObject() == nullptr)
	{
		m_pCollideObject = nullptr;
	}

	/* for문을 빠져 여기까지 왔다는 뜻은 부딪힌 충돌체가 없다는 뜻. 즉 선택할 오브젝트가 없다는 뜻 */
	if (m_pCollideObject)
	{
		CColliderSphere *pColl = m_pCollideObject->FindComponentFromType<CColliderSphere>(CT_COLLIDER);
		pColl->SetColliderRenderCheck(false);
		SAFE_RELEASE(pColl);
	}

	m_pCollideObject = nullptr;
	((CMainFrame*)AfxGetMainWnd())->GetEdit()->GetEffectTab()->SetTargetObject(nullptr);

	for (const auto& object : CGameObject::getObjectList())
	{
		CColliderSphere *pColl = object->FindComponentFromType<CColliderSphere>(CT_COLLIDER);

		if (pRay->CheckCollList(pColl))
		{
			if (!m_pCollideObject)
			{
				pColl->SetColliderRenderCheck(true);
				m_pCollideObject = object;
				((CMainFrame*)AfxGetMainWnd())->GetEdit()->GetEffectTab()->SetTargetObject(object);
			}
			SAFE_RELEASE(pColl);
			SAFE_RELEASE(pRay);
			SAFE_RELEASE(pMouseObj);
			return;
		}
		SAFE_RELEASE(pColl);
	}

	SAFE_RELEASE(pRay);
	SAFE_RELEASE(pMouseObj);

	CView::OnRButtonDown(nFlags, point);
}

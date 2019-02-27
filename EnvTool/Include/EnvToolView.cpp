﻿
// EnvToolView.cpp: CEnvToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "EnvTool.h"
#endif

#include "EnvToolDoc.h"
#include "EnvToolView.h"
#include "Core.h"
#include "Core/Input.h"
#include "Core/DirectInput.h"
#include "Device.h"
#include "Scene/Scene.h"
#include "Scene/Layer.h"
#include "Scene/SceneManager.h"
#include "Component/Terrain2D.h"
#include "Component/Tile.h"
#include "Component/Renderer.h"
#include "Core/TimerManager.h"
#include "Component/LandScape.h"
#include "Component/Picking.h"
#include "Component/ColliderSphere.h"
#include "MainFrm.h"
#include "Scene/Scene.h"
#include "Component/Camera.h"
#include "EditForm.h"

#include "BrushTool.h"
//#include "TarrainEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CEnvToolView

IMPLEMENT_DYNCREATE(CEnvToolView, CView)

BEGIN_MESSAGE_MAP(CEnvToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()

// CEnvToolView 생성/소멸

CEnvToolView::CEnvToolView() noexcept :
	  m_pCamera(nullptr)
	, m_pBrushObj(nullptr)
	, m_pCamTr(nullptr)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_vPickPos = Vector3{ 0.f, 0.f, 0.f };
}

CEnvToolView::~CEnvToolView()
{
	SAFE_RELEASE(m_pCamera);
	SAFE_RELEASE(m_pBrushObj)
	SAFE_RELEASE(m_pCamTr);
	SAFE_RELEASE(m_pTimer);
	DESTROY_SINGLE(CCore);
}

BOOL CEnvToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CEnvToolView 그리기

void CEnvToolView::OnDraw(CDC* /*pDC*/)
{
	CEnvToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CEnvToolView 인쇄

BOOL CEnvToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CEnvToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CEnvToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CEnvToolView 진단

#ifdef _DEBUG
void CEnvToolView::AssertValid() const
{
	CView::AssertValid();
}

void CEnvToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEnvToolDoc* CEnvToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEnvToolDoc)));
	return (CEnvToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CEnvToolView 메시지 처리기


void CEnvToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CWnd * pWnd = AfxGetMainWnd();
	HWND hWnd = pWnd->m_hWnd;

	// 엔진 초기화
	if (!GET_SINGLE(CCore)->Init(AfxGetInstanceHandle(), m_hWnd, 1400, 900, true, true, false))
		return;

	// 카메라 조작을 위한 키 생성
	//GET_SINGLE(CInput)->CreateKey("")

	m_pTimer = GET_SINGLE(CTimerManager)->FindTimer("MainThread");

	// Camera
	this->SetMainCamera();

	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer* pLayer = pScene->GetLayer("Default");

	// Brush
	m_pBrushObj = CGameObject::CreateObject("Brush", pLayer);
	CBrushTool* pBrushTool = m_pBrushObj->AddComponent<CBrushTool>("BrushTool");

	m_vPickPos = Vector3{ 0.f, 0.f, 0.f };

	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pScene);
}


void CEnvToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	CView::OnMouseMove(nFlags, point);
}

 
void CEnvToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonDown(nFlags, point);
}

void CEnvToolView::UpdateView()
{
	float	fTime = m_pTimer->GetDeltaTime();

	this->UpdateInput(fTime);
	this->UpdateObject(fTime);
}

void CEnvToolView::UpdateInput(const float& fTime)
{
	TOOLTAB_TYPE type = ((CMainFrame*)AfxGetMainWnd())->GetEdit()->GetTabType();

	/*POINT	ptMouse = GET_SINGLE(CInput)->GetMousePos();
	POINT   ptMouseMove = GET_SINGLE(CInput)->GetMouseMove();*/

	//if (ptMouseMove.x != 0 || ptMouseMove.y != 0)
	{
		PickingProcess(type);
	}

	if (KEYPUSH("MoveFront"))
	{
		m_pCamTr->MoveWorld(AXIS_Z, 30 * 2.f, fTime);
	}

	if (KEYPUSH("MoveBack"))
	{
		m_pCamTr->MoveWorld(AXIS_Z, -30 * 2.f, fTime);
	}

	if (KEYPUSH("MoveLeft"))
	{
		m_pCamTr->MoveWorld(AXIS_X, -30 * 2.f, fTime);
	}

	if (KEYPUSH("MoveRight"))
	{
		m_pCamTr->MoveWorld(AXIS_X, 30 * 2.f, fTime);

	}
	
	if (KEYPUSH("MouseLButton"))
	{
		switch (type)
		{
		case TAB_TERRAIN:
		case TAB_OBJECT:
		default:
			break;
		}
	}
	
	if (KEYPUSH("MouseRButton"))
	{
		CGameObject* pLandScapeObj = CGameObject::FindObject("LandScape");
		if (pLandScapeObj != NULL)
		{
			CLandScape* pLandScape = pLandScapeObj->FindComponentFromTag<CLandScape>("LandScape");
			switch (type)
			{
			case TAB_TERRAIN:
			{
				CBrushTool* pBrushTool = m_pBrushObj->FindComponentFromTag<CBrushTool>("BrushTool");

				if (!pBrushTool->GetBrushCheck())
				{
					SAFE_RELEASE(pBrushTool);
					break;
				}

				if (pBrushTool->GetHeightCheck())
				{
					list<QUADTREENODE*>* pNodes = pLandScape->FindNode_ByRadius(pBrushTool->GetBrushRange());
					if (!pNodes->empty())
					{
						pBrushTool->MoveHeight(pNodes, m_vPickPos, fTime);
					}
				}
				else if (pBrushTool->GetSplattingCheck())
				{
					pBrushTool->MovePixel(m_vPickPos, fTime);
				}
				SAFE_RELEASE(pBrushTool);
				break;
			}
			case TAB_OBJECT:
			{
				break;
			}
			default:
				break;
			}

			SAFE_RELEASE(pLandScape);
			SAFE_RELEASE(pLandScapeObj);
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

void CEnvToolView::UpdateObject(const float & fTime)
{
}

void CEnvToolView::SetMainCamera()
{
	SAFE_RELEASE(m_pCamera);
	SAFE_RELEASE(m_pCamTr);
	m_pCamera = GET_SINGLE(CSceneManager)->GetCurrentScene()->GetMainCameraObj();
	m_pCamTr = GET_SINGLE(CSceneManager)->GetCurrentScene()->GetMainCameraTr();
}

void CEnvToolView::SetLightCamera()
{
	SAFE_RELEASE(m_pCamera);
	SAFE_RELEASE(m_pCamTr);
	m_pCamera = GET_SINGLE(CSceneManager)->GetCurrentScene()->GetLightCameraObj();
	m_pCamTr = GET_SINGLE(CSceneManager)->GetCurrentScene()->GetLightCameraTr();
}

void CEnvToolView::PickingProcess(TOOLTAB_TYPE type)
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
				if (bFirstCheck)
					break;

				if (pPicking->Picking_ToBuffer(&m_vPickPos,
					GET_SINGLE(CInput)->GetRayPos(),
					GET_SINGLE(CInput)->GetRayDir(),
					iter->vecVtx, iter->vecIndex))
				{
					switch (type)
					{
					case TAB_TERRAIN:
					{
						CBrushTool* pBrushTool = m_pBrushObj->FindComponentFromTag<CBrushTool>("BrushTool");
						//_cprintf("x : %f, y : %f, z : %f\n", pickPos.x, pickPos.y, pickPos.z);
						pBrushTool->SetBrushInformation(m_vPickPos);
						SAFE_RELEASE(pBrushTool);
						bFirstCheck = true;
						break;
					}
					case TAB_OBJECT:
					{
						break;
					}
					default:
						break;
					}
				}
			}
			//_cprintf("%d\n", pNodes->size());
		}

		SAFE_RELEASE(pPicking);
		SAFE_RELEASE(pLandScape);
		SAFE_RELEASE(pLandScapeObj);
	}
}

void CEnvToolView::SetTileColor(float x, float y)
{

}

int CEnvToolView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}


void CEnvToolView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CEnvToolView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}
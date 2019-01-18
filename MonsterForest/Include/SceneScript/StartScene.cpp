#include "StartScene.h"
#include "Scene/Layer.h"
#include "GameObject/GameObject.h"
#include "Component/Renderer2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "Component/Animation2D.h"
#include "Component/Terrain2D.h"
#include "Component/Camera.h"
#include "Component/UIButton.h"
#include "Component/ColliderRect.h"
#include "Core/Input.h"
#include "Device.h"
#include "MainScene.h"
#include "Core/Scheduler.h"

CStartScene::CStartScene()
{
}

CStartScene::~CStartScene()
{
}

bool CStartScene::Init()
{
	CLayer*	pUILayer = m_pScene->GetLayer("UI");

	CGameObject*	pBackPanel = CGameObject::CreateObject("StartSceneBack", pUILayer);
	
	CTransform*	pBackTr = pBackPanel->GetTransform();

	pBackTr->SetWorldScale(DEVICE_RESOLUTION.iWidth, 
		DEVICE_RESOLUTION.iHeight, 1.f);

	SAFE_RELEASE(pBackTr);

	CRenderer2D*	pRenderer = pBackPanel->AddComponent<CRenderer2D>("Renderer");

	pRenderer->SetShader(UI_SHADER);
	pRenderer->SetInputLayout("VertexTex");
	pRenderer->SetMesh("RectOrthoLTTex");
	pRenderer->SetRenderState(ALPHA_BLEND);

	/*pRenderer->CreateCBuffer("Button", 11, sizeof(BUTTONCBUFFER),
		SCT_VERTEX | SCT_PIXEL);*/

	CMaterial* pMaterial = pRenderer->CreateMaterial(SAMPLER_LINEAR,
		"Yasuo", 0, 0, L"Panel.jpg");

	SAFE_RELEASE(pMaterial);

	SAFE_RELEASE(pRenderer);

	SAFE_RELEASE(pBackPanel);

	CGameObject*	pButtonObj = CGameObject::CreateObject("Button", pUILayer);

	CUIButton*	pButton = pButtonObj->AddComponent<CUIButton>("Button");

	pButton->SetCallback(this, &CStartScene::StartCallback);

	SAFE_RELEASE(pButton);

	CTransform*	pButtonTr = pButtonObj->GetTransform();
	
	//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
	pButtonTr->SetWorldScale(200.f, 100.f, 1.f);
	pButtonTr->SetWorldPos(DEVICE_RESOLUTION.iWidth / 2.f - 100.f,
		DEVICE_RESOLUTION.iHeight / 2.f - 120.f, 0.f);

	SAFE_RELEASE(pButtonTr);

	pRenderer = pButtonObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);

	pMaterial = pRenderer->GetMaterial();

	pMaterial->SetDiffuseTexInfo("Linear", "StartButton",
		0, 0, L"Start.png");

	SAFE_RELEASE(pMaterial);

	SAFE_RELEASE(pRenderer);

	CColliderRect* pRC = pButtonObj->FindComponentFromType<CColliderRect>(CT_COLLIDER);

	pRC->SetRect(0, 0, 200, 100);

	SAFE_RELEASE(pRC);

	SAFE_RELEASE(pButtonObj);

	SAFE_RELEASE(pUILayer);

	GET_SINGLE(CScheduler)->AddSchedule<CStartScene>("GameStart",
		SO_ONCE, this, &CStartScene::ScheduleStartCallback,
		5.f, 0.f, 0);

	return true;
}

int CStartScene::Update(float fTime)
{
	return 0;
}

void CStartScene::StartCallback(float fTime)
{
	GET_SINGLE(CScheduler)->DeleteSchedule("GameStart");

	CScene*	pScene = GET_SINGLE(CSceneManager)->CreateNextScene("MainScene");

	CMainScene*	pMainScene = pScene->CreateSceneScript<CMainScene>("MainScene");

	SAFE_RELEASE(pMainScene);

	SAFE_RELEASE(pScene);
}

void CStartScene::ScheduleStartCallback(float fTime)
{
	CScene*	pScene = GET_SINGLE(CSceneManager)->CreateNextScene("MainScene");

	CMainScene*	pMainScene = pScene->CreateSceneScript<CMainScene>("MainScene");

	SAFE_RELEASE(pMainScene);

	SAFE_RELEASE(pScene);
}

#include "QuestScene.h"
#include "global.h"
#include "Scene/Layer.h"
#include "Core/Input.h"
#include "Device.h"
#include "Core/Scheduler.h"
#include "Core.h"
#include "Device.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

#include "GameObject/GameObject.h"

#include "Component/Renderer2D.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "Component/Animation2D.h"
#include "Component/Terrain2D.h"
#include "Component/Camera.h"
#include "Component/UIButton.h"
#include "Component/ColliderRect.h"

#include "Component/Renderer2D.h"
#include "Component/Font.h"
#include "Component/ColliderRect.h"
#include "Component/UIPanel.h"



QuestScene::QuestScene()
{
}


QuestScene::~QuestScene()
{
}

bool QuestScene::Init()
{
	//{
	//	CLayer* pLayer = m_pScene->CreateLayer("UI+1", UI_LAYER + 1);
	//	SAFE_RELEASE(pLayer);
	//}
	//{
	//	CLayer* pLayer = m_pScene->CreateLayer("UI+2", UI_LAYER + 2);
	//	SAFE_RELEASE(pLayer);
	//}
	///// < Background>
	//{
	//	CLayer*	pUILayer = m_pScene->GetLayer("UI");

	//	CGameObject*	pLoginSceneBackgroundObj = CGameObject::CreateObject("LoginSceneBackground", pUILayer);

	//	CUIPanel*	pLoginSceneBackgroundUI = pLoginSceneBackgroundObj->AddComponent<CUIPanel>("LoginSceneBackground");
	//	pLoginSceneBackgroundUI->SetTitleArea(0, 0, 0, 0);
	//	SAFE_RELEASE(pLoginSceneBackgroundUI);

	//	CTransform*	pLoginSceneBackgroundTr = pLoginSceneBackgroundObj->GetTransform();

	//	pLoginSceneBackgroundTr->SetWorldScale(DEVICE_RESOLUTION.iWidth,
	//		DEVICE_RESOLUTION.iHeight, 1.f);

	//	SAFE_RELEASE(pLoginSceneBackgroundTr);

	//	CRenderer2D*	pRenderer = pLoginSceneBackgroundObj->AddComponent<CRenderer2D>("Renderer");

	//	pRenderer->SetShader(UI_SHADER);
	//	pRenderer->SetInputLayout("VertexTex");
	//	pRenderer->SetMesh("RectOrthoLTTex");
	//	pRenderer->SetRenderState(ALPHA_BLEND);

	//	/*pRenderer->CreateCBuffer("Button", 11, sizeof(BUTTONCBUFFER),
	//		SCT_VERTEX | SCT_PIXEL);*/

	//	CMaterial* pMaterial = pRenderer->CreateMaterial(SAMPLER_LINEAR,
	//		"LoginSceneBackground", 0, 0, L"LoginSceneBackground.png");

	//	SAFE_RELEASE(pMaterial);

	//	SAFE_RELEASE(pRenderer);
	//	SAFE_RELEASE(pUILayer);
	//}
	return true;
}

int QuestScene::Update(float fTime)
{
	return 0;
}

void QuestScene::Input(float fTime)
{
}

int QuestScene::LateUpdate(float fTime)
{
	return 0;
}

void QuestScene::Render(float fTime)
{
}

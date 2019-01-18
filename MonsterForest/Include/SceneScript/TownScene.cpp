#include "TownScene.h"
#include "GameObject/GameObject.h"
#include "Scene/Layer.h"
#include "Scene/Scene.h"
#include "Component/Transform.h"
#include "Scene/SceneManager.h"
#include "MainScene.h"

CTownScene::CTownScene()
{
}

CTownScene::~CTownScene()
{
}

bool CTownScene::Init()
{
	return true;
}

int CTownScene::Update(float fTime)
{
	static bool	bCreate;

	if (!bCreate)
	{
		bCreate = true;
		CLayer*	pLayer = m_pScene->GetLayer("Default");
		CGameObject*	pGoblin = CGameObject::CreateClone("BigGoblin", pLayer);

		CTransform*	pTr = pGoblin->GetTransform();

		pTr->SetWorldPos(1.f, 1.f, 1.f);

		SAFE_RELEASE(pTr);

		SAFE_RELEASE(pGoblin);
		SAFE_RELEASE(pLayer);
	}

	if (GetAsyncKeyState(VK_F7) & 0x8000)
	{
		CScene*	pScene = GET_SINGLE(CSceneManager)->CreateNextScene("MainScene");

		CMainScene*	pStartScene = pScene->CreateSceneScript<CMainScene>("MainScene");

		SAFE_RELEASE(pStartScene);

		SAFE_RELEASE(pScene);
	}

	return 0;
}

#include "Chatting.h"
#include "Component/Renderer2D.h"
#include "Scene/Layer.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/Renderer2D.h"
#include "Component/ColliderRect.h"
#include "Component/Material.h"
#include "Component/Transform.h"

Chatting::Chatting()
{
}


Chatting::~Chatting()
{
}

bool Chatting::initialize()
{
	{
		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer*	pLayer = pScene->GetLayer("UI");
		
		this->pUIChat = CGameObject::CreateObject("UIChat", pLayer);

		CUIButton*	pButton = pUIChat->AddComponent<CUIButton>("ChatEdit");
		SAFE_RELEASE(pButton);

		CTransform*	pButtonTr = pUIChat->GetTransform();

		//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
		pButtonTr->SetWorldScale(350.f, 50.f, 1.f);
		pButtonTr->SetWorldPos(0.f , 670.f, 0.f);

		SAFE_RELEASE(pButtonTr);
		CRenderer2D* pRenderer = pUIChat->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "ChatEdit",
			0, 0, L"LoginEdit.bmp");

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);

		CColliderRect* pRC = pUIChat->FindComponentFromType<CColliderRect>(CT_COLLIDER);

		pRC->SetRect(0, 0, 350, 50);

		SAFE_RELEASE(pRC);

		pUIChatText = pUIChat->AddComponent<CFont>("TextUI");
		pUIChatText->SetFont("³ª´®°íµñ");
		chatString = L"Ã¤ÆÃ Å×½ºÆ®";
		pUIChatText->SetText(chatString);
		pUIChatText->SetArea(10, 0, 350, 30.f);

		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
	}

	{
		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer*	pLayer = pScene->GetLayer("UI");

		this->pUIChatLog = CGameObject::CreateObject("UIChatLog", pLayer);

		CUIButton*	pButton = pUIChatLog->AddComponent<CUIButton>("ChatEdit");
		SAFE_RELEASE(pButton);

		CTransform*	pButtonTr = pUIChatLog->GetTransform();

		//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
		pButtonTr->SetWorldScale(350.f, 200.f, 1.f);
		pButtonTr->SetWorldPos(0.f, 470.f, 0.f);

		SAFE_RELEASE(pButtonTr);
		CRenderer2D* pRenderer = pUIChatLog->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "ChatEdit",
			0, 0, L"LoginEdit.bmp");

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);

		CColliderRect* pRC = pUIChatLog->FindComponentFromType<CColliderRect>(CT_COLLIDER);

		pRC->SetRect(0, 0, 350.f, 200.f);

		SAFE_RELEASE(pRC);

		pUIChatLogText = pUIChatLog->AddComponent<CFont>("TextUI");
		pUIChatLogText->SetFont("³ª´®°íµñ");
		pUIChatLogText->SetText(L"");
		pUIChatLogText->SetArea(10, 190, 350, 30.f);

		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
	}
	return true;
}

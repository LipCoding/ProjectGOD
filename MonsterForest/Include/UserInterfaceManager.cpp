#include "UserInterfaceManager.h"
#include "Chatting.h"
#include "Inventory.h"

#include "Component/Renderer2D.h"
#include "Scene/Layer.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/Renderer2D.h"
#include "Component/ColliderRect.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "Core/TimerManager.h"
#include "Core\Timer.h"
#include "Core/Input.h"
#include "Status.h"
#include "ObjectScript/Player.h"
#include "MiniMap.h"
DEFINITION_SINGLE(UserInterfaceManager);


UserInterfaceManager::UserInterfaceManager()
{
}


UserInterfaceManager::~UserInterfaceManager()
{
}

bool UserInterfaceManager::initialize()
{
	this->pChatting = new Chatting;
	this->pChatting->initialize();
	this->pInventory = new Inventory;
	this->pInventory->initialize();
	//this->pStatus = new Status;
	//this->pStatus->initialize();
	//this->pEnemyStatus = new Status;
	//this->pEnemyStatus->initialize();
	{
		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer*	pLayer = pScene->GetLayer("UI+1");
		CGameObject* pStatusObj = CGameObject::CreateObject("PlayerStatus", pLayer);
		this->pStatus = pStatusObj->AddComponent<Status>("Status");
		this->pStatus->initialize();

		CTransform* pStatusTr = pStatusObj->GetTransform();
		pStatusTr->SetWorldScale(100.f, 100.f, 1.f);
		CRenderer2D* pRenderer = pStatusObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "UI_PlayerPortrait",
			0, 0, L"UserInterface/UI_GAUGE_PLAYER.png");
		SAFE_RELEASE(pStatusTr);
		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pStatusObj);
		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
	}

	{
		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer*	pLayer = pScene->GetLayer("UI+1");
		CGameObject* pEnemyStatusObj = CGameObject::CreateObject("EnemyStatus", pLayer);
		this->pEnemyStatus = pEnemyStatusObj->AddComponent<Status>("Status");
		pEnemyStatus->initialize();
		CTransform* pStatusTr = pEnemyStatusObj->GetTransform();
		pStatusTr->SetWorldScale(100.f, 100.f, 1.f);
		pStatusTr->SetWorldPos(300.f, 0.f, 1.f);
		CRenderer2D* pRenderer = pEnemyStatusObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "UI_EnemyPortrait",
			0, 0, L"UserInterface/UI_GAUGE_PLAYER.png");
		SAFE_RELEASE(pStatusTr);
		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pEnemyStatusObj);
		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
	}

	{
		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer*	pLayer = pScene->GetLayer("UI+1");
		CGameObject* pMinimapObj = CGameObject::CreateObject("Minimap", pLayer);
		this->pMiniMap = pMinimapObj->AddComponent<MiniMap>("Minimap");
		this->pMiniMap->initialize();

		CTransform* pStatusTr = pMinimapObj->GetTransform();
		pStatusTr->SetWorldScale(150.f, 150.f, 1.f);
		pStatusTr->SetWorldPos(1100.f, 0.f, 0.f);
		CRenderer2D* pRenderer = pMinimapObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "UIMINIMAP",
			0, 0, L"UserInterface/UI_MINIMAP_CIRCLE.png");
		SAFE_RELEASE(pStatusTr);
		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pMinimapObj);
		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
	}
#pragma region frameRender
	{
		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer*	pLayer = pScene->GetLayer("UI");

		this->pFrameRender = CGameObject::CreateObject("FrameRender", pLayer);

		CUIButton*	pButton = pFrameRender->AddComponent<CUIButton>("frameRender");
		SAFE_RELEASE(pButton);

		CTransform*	pButtonTr = pFrameRender->GetTransform();

		//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
		pButtonTr->SetWorldScale(50.f, 50.f, 1.f);
		pButtonTr->SetWorldPos(600.f, 0.f, 0.f);

		SAFE_RELEASE(pButtonTr);
		CRenderer2D* pRenderer = pFrameRender->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "Frame",
			0, 0, L"LoginEdit.bmp");

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);

		CColliderRect* pRC = pFrameRender->FindComponentFromType<CColliderRect>(CT_COLLIDER);

		pRC->SetRect(0, 0, 200, 50);

		SAFE_RELEASE(pRC);

		pFrameText = pFrameRender->AddComponent<CFont>("TextFrameRender");
		pFrameText->SetFont("³ª´®°íµñ");
		frameString = L"Ã¤ÆÃ Å×½ºÆ®";
		pFrameText->SetText(frameString);
		pFrameText->SetArea(10, 0, 200, 30.f);

		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
	}
#pragma endregion

	return true;
}

void UserInterfaceManager::update(float time)
{
	pInventory->update(time);

	CTimer*	pTimer = GET_SINGLE(CTimerManager)->FindTimer("MainThread");
	pTimer->Update();
	float fTime = pTimer->GetFPS();
	frameString = to_wstring((int)fTime);
	SAFE_RELEASE(pTimer);
	this->pFrameText->SetText(frameString);
	/*
			Chatting* pChatting = GET_SINGLE(UserInterfaceManager)->getChatting();
		wstring edit_view = L"";
		for (auto input : pChatting->getChatCont())
		{
			//wchar_t temp[64];
			wchar_t temp2 = input;
			wstring appendString = L"";
			appendString = appendString + temp2;
			edit_view = edit_view + appendString;
		}
		pChatting->setChatString(edit_view);
		pChatting->getUIChatText()->SetText(edit_view);
	*/
	{
		wstring edit_view = L"";
		for (auto input : pChatting->getChatCont())
		{
			//wchar_t temp[64];
			wchar_t temp2 = input;
			wstring appendString = L"";
			appendString = appendString + temp2;
			edit_view = edit_view + appendString;
		}
		pChatting->setChatString(edit_view);
		pChatting->getUIChatText()->SetText(edit_view);
	}

	if (KEYDOWN("INVENTORY"))
	{
		this->pInventory->enableShowInventory();
	}

	if (pPlayer != nullptr)
	{
		wstring HPText = to_wstring(pPlayer->getCurrentHP());
		HPText += L" / ";
		HPText += to_wstring(pPlayer->getMaxHP());
		CGameObject* pHearthBarObj = pStatus->getUIHearthBar()->GetGameObject();
		CFont* pFont = pHearthBarObj->FindComponentFromTag<CFont>("HPFont");
		pFont->SetText(HPText);
		SAFE_RELEASE(pFont);
		SAFE_RELEASE(pHearthBarObj);
	}
}

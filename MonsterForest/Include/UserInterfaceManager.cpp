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
#include "DropTableUI.h"
#include "TargetPlayerUI.h"
#include "PGMessageBox.h"
#include "PartyStatus.h"
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
	for (auto& player : partyCont)
		player = nullptr;
	//this->pStatus = new Status;
	//this->pStatus->initialize();
	//this->pEnemyStatus = new Status;
	//this->pEnemyStatus->initialize();

#pragma region PlayerStatus
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
#pragma endregion

#pragma region EnemyStatus
	{
		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer*	pLayer = pScene->GetLayer("UI+1");
		CGameObject* pEnemyStatusObj = CGameObject::CreateObject("EnemyStatus", pLayer);
		CTransform* pStatusTr = pEnemyStatusObj->GetTransform();
		pStatusTr->SetWorldScale(100.f, 100.f, 1.f);
		pStatusTr->SetWorldPos(400.f, 0.f, 1.f);
		this->pEnemyStatus = pEnemyStatusObj->AddComponent<Status>("Status");
		pEnemyStatus->initialize();
		CRenderer2D* pRenderer = pEnemyStatusObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "UI_EnemyPortrait",
			0, 0, L"UserInterface/UI_GAUGE_PLAYER.png");
		pEnemyStatus->enableRender(false);
		SAFE_RELEASE(pStatusTr);
		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pEnemyStatusObj);
		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
	}
#pragma endregion

#pragma region DropTableUI
	{
		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer*	pLayer = pScene->GetLayer("UI+1");

		CGameObject* pDropTableUIObj = CGameObject::CreateObject("DropTableUI", pLayer);


		this->pDropTableUI = pDropTableUIObj->AddComponent<DropTableUI>("DropTableUI");
		pDropTableUI->initialize();
		CRenderer2D* pRenderer = pDropTableUIObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "UI_DropTable",
			0, 0, L"UserInterface/UI_BASIC_BOX_BIG.png");
		//pDropTableUI->enableRender(false);

		CTransform* pDropTableTr = pDropTableUIObj->GetTransform();
		pDropTableTr->SetWorldScale(200.f, 150.f, 1.f);
		pDropTableTr->SetWorldPos(400.f, 400.f, 1.f);

		//CColliderRect* pRC = pDropTableUIObj->FindComponentFromType<CColliderRect>(CT_COLLIDER);
		//pRC->SetRect(0, 0, 200.f, 150.f);
		//pRC->SetTag("DropTable");
		//SAFE_RELEASE(pRC);

		pDropTableUI->enableRender(false);

		SAFE_RELEASE(pDropTableTr);
		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pDropTableUIObj);
		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
	}
#pragma endregion

#pragma region MiniMap
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
#pragma endregion
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
		pFrameText->SetFont("나눔고딕");
		frameString = L"채팅 테스트";
		pFrameText->SetText(frameString);
		pFrameText->SetArea(10, 0, 200, 30.f);

		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
	}
#pragma endregion

#pragma region TargetPlayerUI
	{
		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer*	pLayer = pScene->GetLayer("UI+1");

		CGameObject* pTargetPlayerUIObj = CGameObject::CreateObject("TargetPlayerUI", pLayer);


		this->pTargetPlayerUI = pTargetPlayerUIObj->AddComponent<TargetPlayerUI>("TargetPlayerUI");
		pTargetPlayerUI->initialize();
		CRenderer2D* pRenderer = pTargetPlayerUIObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "UI_TargetPlayerUI",
			0, 0, L"UserInterface/UI_BASIC_BOX_BIG.png");

		CTransform* pDropTableTr = pTargetPlayerUIObj->GetTransform();
		pDropTableTr->SetWorldScale(200.f, 150.f, 1.f);
		pDropTableTr->SetWorldPos(300.f, 300.f, 0.f);

		pTargetPlayerUI->enableRender(false);

		SAFE_RELEASE(pDropTableTr);
		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pTargetPlayerUIObj);
		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
	}
#pragma endregion

#pragma region PGMessageBox
	{
		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer*	pLayer = pScene->GetLayer("UI+1");

		CGameObject* pPGMessageBoxObj = CGameObject::CreateObject("MessageBox", pLayer);

		this->pPGMessageBox = pPGMessageBoxObj->AddComponent<PGMessageBox>("TargetPlayerUI");
		pPGMessageBox->initialize();
		CRenderer2D* pRenderer = pPGMessageBoxObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "UI_TargetPlayerUI",
			0, 0, L"UserInterface/UI_BASIC_BOX_BIG.png");

		CTransform* pDropTableTr = pPGMessageBoxObj->GetTransform();
		pDropTableTr->SetWorldScale(200.f, 150.f, 1.f);
		pDropTableTr->SetWorldPos(300.f, 300.f, 0.f);

		pPGMessageBox->enableRender(false);

		SAFE_RELEASE(pDropTableTr);
		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pPGMessageBoxObj);
		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
	}
#pragma endregion

#pragma region PartyFrame
	{
		// first
		{
			CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
			CLayer*	pLayer = pScene->GetLayer("UI+1");
			CGameObject* pPartyFrameObj = CGameObject::CreateObject("PartyFrame1", pLayer);
			CTransform* pPartyFrameTr = pPartyFrameObj->GetTransform();
			pPartyFrameTr->SetWorldPos(0.f, 150.f, 1.f);
			partyCont[0] = pPartyFrameObj->AddComponent<PartyStatus>("Status");
			partyCont[0]->setScale(0.75f);
			partyCont[0]->initialize();
			CRenderer2D* pRenderer = pPartyFrameObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
			CMaterial* pMaterial = pRenderer->GetMaterial();

			pMaterial->SetDiffuseTexInfo("Linear", "PartyFrame1",
				0, 0, L"UserInterface/UI_GAUGE_PLAYER.png");
			partyCont[0]->enableRender(false);
			pPartyFrameTr->SetWorldScale(75.f, 75.f, 1.f);
			SAFE_RELEASE(pPartyFrameTr);
			SAFE_RELEASE(pMaterial);
			SAFE_RELEASE(pRenderer);
			SAFE_RELEASE(pPartyFrameObj);
			SAFE_RELEASE(pLayer);
			SAFE_RELEASE(pScene);
		}

		// second
		{
			CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
			CLayer*	pLayer = pScene->GetLayer("UI+1");
			CGameObject* pPartyFrameObj = CGameObject::CreateObject("PartyFrame1", pLayer);
			CTransform* pPartyFrameTr = pPartyFrameObj->GetTransform();
			pPartyFrameTr->SetWorldPos(0.f, 230.f, 1.f);
			partyCont[1] = pPartyFrameObj->AddComponent<PartyStatus>("Status");
			partyCont[1]->setScale(0.75f);
			partyCont[1]->initialize();
			CRenderer2D* pRenderer = pPartyFrameObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
			CMaterial* pMaterial = pRenderer->GetMaterial();

			pMaterial->SetDiffuseTexInfo("Linear", "PartyFrame1",
				0, 0, L"UserInterface/UI_GAUGE_PLAYER.png");
			partyCont[1]->enableRender(false);
			pPartyFrameTr->SetWorldScale(75.f, 75.f, 1.f);
			SAFE_RELEASE(pPartyFrameTr);
			SAFE_RELEASE(pMaterial);
			SAFE_RELEASE(pRenderer);
			SAFE_RELEASE(pPartyFrameObj);
			SAFE_RELEASE(pLayer);
			SAFE_RELEASE(pScene);
		}

		// third
		{
			CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
			CLayer*	pLayer = pScene->GetLayer("UI+1");
			CGameObject* pPartyFrameObj = CGameObject::CreateObject("PartyFrame1", pLayer);
			CTransform* pPartyFrameTr = pPartyFrameObj->GetTransform();
			pPartyFrameTr->SetWorldPos(0.f, 310.f, 1.f);
			partyCont[2] = pPartyFrameObj->AddComponent<PartyStatus>("Status");
			partyCont[2]->setScale(0.75f);
			partyCont[2]->initialize();
			CRenderer2D* pRenderer = pPartyFrameObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
			CMaterial* pMaterial = pRenderer->GetMaterial();

			pMaterial->SetDiffuseTexInfo("Linear", "PartyFrame1",
				0, 0, L"UserInterface/UI_GAUGE_PLAYER.png");
			partyCont[2]->enableRender(false);
			pPartyFrameTr->SetWorldScale(75.f, 75.f, 1.f);
			SAFE_RELEASE(pPartyFrameTr);
			SAFE_RELEASE(pMaterial);
			SAFE_RELEASE(pRenderer);
			SAFE_RELEASE(pPartyFrameObj);
			SAFE_RELEASE(pLayer);
			SAFE_RELEASE(pScene);
		}

		// forth
		{
			CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
			CLayer*	pLayer = pScene->GetLayer("UI+1");
			CGameObject* pPartyFrameObj = CGameObject::CreateObject("PartyFrame1", pLayer);
			CTransform* pPartyFrameTr = pPartyFrameObj->GetTransform();
			pPartyFrameTr->SetWorldPos(0.f, 390.f, 1.f);
			partyCont[3] = pPartyFrameObj->AddComponent<PartyStatus>("Status");
			partyCont[3]->setScale(0.75f);
			partyCont[3]->initialize();
			CRenderer2D* pRenderer = pPartyFrameObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
			CMaterial* pMaterial = pRenderer->GetMaterial();

			pMaterial->SetDiffuseTexInfo("Linear", "PartyFrame1",
				0, 0, L"UserInterface/UI_GAUGE_PLAYER.png");
			partyCont[3]->enableRender(false);
			pPartyFrameTr->SetWorldScale(75.f, 75.f, 1.f);
			SAFE_RELEASE(pPartyFrameTr);
			SAFE_RELEASE(pMaterial);
			SAFE_RELEASE(pRenderer);
			SAFE_RELEASE(pPartyFrameObj);
			SAFE_RELEASE(pLayer);
			SAFE_RELEASE(pScene);
		}
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

void UserInterfaceManager::addPartyPlayer(const string& playerName)
{
	for (auto& player : partyCont)
	{
		if (false == player->getUsed())
		{
			player->setPlayerName(playerName);
			player->enableRender(true);
			player->setUsed(true);
		}
	}
}

void UserInterfaceManager::removePartyPlayer(const string& playerName)
{
	// 앞으로 옮겨준다.
}

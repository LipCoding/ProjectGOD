#include "TargetPlayerUI.h"
#include "Component/Renderer2D.h"
#include "Scene/Layer.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/Renderer2D.h"
#include "Component/ColliderRect.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "Core/Input.h"
#include "UserInterfaceManager.h"
#include "Inventory.h"
#include "NetworkManager.h"
#include "UserInfoUI.h"
#include "InviteToParty.h"
#include "TradeUI.h"

TargetPlayerUI::TargetPlayerUI()
{
}


TargetPlayerUI::~TargetPlayerUI()
{
}

void TargetPlayerUI::initialize()
{

	// 상대유저정보
	{
		CLayer* pLayer = m_pScene->GetLayer("UI+1");
		CGameObject* pUserInfoObj = CGameObject::CreateObject("UserInfoUI", pLayer);
		this->pUserInfoUI = pUserInfoObj->AddComponent<UserInfoUI>("Slot");
		static_cast<UserInfoUI*>(pUserInfoUI)->initialize();
		static_cast<UserInfoUI*>(pUserInfoUI)->setBaseUI(this);
		CTransform*	pSlotTr = pUserInfoObj->GetTransform();
		pSlotTr->SetWorldPos(0.f, 0.f, 0.f);
		pSlotTr->SetWorldScale(180.f, 40.f, 1.f);
		SAFE_RELEASE(pSlotTr);
		pUserInfoObj->Enable(false);
		CRenderer2D* pRenderer = pUserInfoObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "UserInfoUI",
			0, 0, L"UserInterface/UI_BASIC_BOX_LONG.png");

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);

		CFont* UserInfoFont = pUserInfoObj->AddComponent<CFont>("UserInfoFont");
		UserInfoFont->SetFont("돋움25");
		UserInfoFont->SetText(L"유저정보");
		UserInfoFont->SetArea(35, 30, 200, 30.f);
		SAFE_RELEASE(UserInfoFont);

		CColliderRect* pRC = pUserInfoObj->FindComponentFromType<CColliderRect>(CT_COLLIDER);
		pRC->SetRect(0, 0, 180.f, 40.f);

		SAFE_RELEASE(pRC);
		SAFE_RELEASE(pUserInfoObj);
		SAFE_RELEASE(pLayer);
	}

	// 파티신청
	{
		CLayer* pLayer = m_pScene->GetLayer("UI+1");
		CGameObject* pInviteToParty = CGameObject::CreateObject("InviteToParty", pLayer);
		this->pInviteToPartyUI = pInviteToParty->AddComponent<InviteToParty>("Slot");
		static_cast<InviteToParty*>(pInviteToPartyUI)->initialize();
		static_cast<InviteToParty*>(pInviteToPartyUI)->setBaseUI(this);
		CTransform*	pUserInfoTr = pInviteToParty->GetTransform();
		pUserInfoTr->SetWorldPos(0.f, 0.f, 0.f);
		pUserInfoTr->SetWorldScale(180.f, 45.f, 1.f);
		SAFE_RELEASE(pUserInfoTr);
		pInviteToParty->Enable(false);
		
		CFont* partyFont = pInviteToParty->AddComponent<CFont>("PartyFont");
		partyFont->SetFont("돋움25");
		partyFont->SetText(L"파티신청");
		partyFont->SetArea(35, 30, 200, 30.f);
		SAFE_RELEASE(partyFont);
		
		CRenderer2D* pRenderer = pInviteToParty->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "UserInfoUI",
			0, 0, L"UserInterface/UI_BASIC_BOX_LONG.png");

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);

		CColliderRect* pRC = pInviteToParty->FindComponentFromType<CColliderRect>(CT_COLLIDER);
		pRC->SetRect(0, 0, 180.f, 45.f);

		SAFE_RELEASE(pRC);
		SAFE_RELEASE(pInviteToParty);
		SAFE_RELEASE(pLayer);
	}
	// 교환신청
	{
		CLayer* pLayer = m_pScene->GetLayer("UI+1");
		CGameObject* pTradeUIObj = CGameObject::CreateObject("TradeUI", pLayer);
		this->pTradeUI = pTradeUIObj->AddComponent<TradeUI>("Slot");
		static_cast<TradeUI*>(pTradeUI)->initialize();
		static_cast<TradeUI*>(pTradeUI)->setBaseUI(this);
		CTransform*	pTradeTr = pTradeUIObj->GetTransform();
		pTradeTr->SetWorldPos(0.f, 0.f, 0.f);
		pTradeTr->SetWorldScale(180.f, 45.f, 1.f);
		SAFE_RELEASE(pTradeTr);
		pTradeUIObj->Enable(false);

		CFont* TradeFont = pTradeUIObj->AddComponent<CFont>("TradeFont");
		TradeFont->SetFont("돋움25");
		TradeFont->SetText(L"교환신청");
		TradeFont->SetArea(35, 30, 200, 30.f);
		SAFE_RELEASE(TradeFont);


		CRenderer2D* pRenderer = pTradeUIObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();
		
		pMaterial->SetDiffuseTexInfo("Linear", "UserInfoUI",
			0, 0, L"UserInterface/UI_BASIC_BOX_LONG.png");

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);

		CColliderRect* pRC = pTradeUIObj->FindComponentFromType<CColliderRect>(CT_COLLIDER);
		pRC->SetRect(0, 0, 180.f, 40.f);

		SAFE_RELEASE(pRC);
		SAFE_RELEASE(pTradeUIObj);
		SAFE_RELEASE(pLayer);
	}
}

int TargetPlayerUI::Update(float fTime)
{
	CTransform* pDropTableTr = this->GetTransform();
	Vector3 basePos = pDropTableTr->GetWorldPos();
	SAFE_RELEASE(pDropTableTr);
	{
		CTransform* pTransform = this->pUserInfoUI->GetTransform();
		pTransform->SetWorldPos(Vector3(basePos.x+10,basePos.y+10,0.f));
		SAFE_RELEASE(pTransform);
	}
	{
		CTransform* pTransform = this->pInviteToPartyUI->GetTransform();
		pTransform->SetWorldPos(Vector3(basePos.x + 10, basePos.y + 52, 0.f));
		SAFE_RELEASE(pTransform);
	}
	{
		CTransform* pTransform = this->pTradeUI->GetTransform();
		pTransform->SetWorldPos(Vector3(basePos.x + 10, basePos.y + 97, 0.f));
		SAFE_RELEASE(pTransform);
	}

	return 0;
}

void TargetPlayerUI::enableRender(bool show)
{
	this->m_pGameObject->Enable(show);
	{
		CGameObject* pGameObject = this->pUserInfoUI->GetGameObject();
		pGameObject->Enable(show);
		SAFE_RELEASE(pGameObject);
	}

	{
		CGameObject* pGameObject = pInviteToPartyUI->GetGameObject();
		pGameObject->Enable(show);
		SAFE_RELEASE(pGameObject);
	}

	{
		CGameObject* pGameObject = pTradeUI->GetGameObject();
		pGameObject->Enable(show);
		SAFE_RELEASE(pGameObject);
	}
}
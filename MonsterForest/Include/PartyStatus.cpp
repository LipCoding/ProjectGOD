#include "PartyStatus.h"
#include "Component/Renderer2D.h"
#include "Scene/Layer.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/Renderer2D.h"
#include "Component/ColliderRect.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "Component/Font.h"

PartyStatus::PartyStatus()
{
}


PartyStatus::~PartyStatus()
{
}

bool PartyStatus::initialize()
{
	//this->m_pTransform->SetWorldScale(Vector3(10.f, 10.f, 1.f));
	Vector3 basePos = this->m_pTransform->GetWorldPos();
	{
		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer*	pLayer = pScene->GetLayer("UI+2");
		CGameObject* pUIHearthBarBackgroundObj = CGameObject::CreateObject("UIHearthBarBackground", pLayer);
		this->pUIHearthBarBackground = pUIHearthBarBackgroundObj->AddComponent<CUIButton>("PartyStatus");
		CTransform* pPartyStatusTr = pUIHearthBarBackgroundObj->GetTransform();
		pPartyStatusTr->SetWorldPos(basePos.x + 75.f, basePos.y + 0.f, 1.f);
		pPartyStatusTr->SetWorldScale(200.f * scale, 30.f*scale, 1.f);
		CRenderer2D* pRenderer = pUIHearthBarBackgroundObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "UIHPBackground",
			0, 0, L"UserInterface/UI_GAUGE_HP.png");

		SAFE_RELEASE(pPartyStatusTr);
		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pUIHearthBarBackgroundObj);
		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
	}

	{
		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer*	pLayer = pScene->GetLayer("UI+2");
		CGameObject* pUIManaBarBackgroundObj = CGameObject::CreateObject("UIManahBarBackground", pLayer);
		this->pUIManaBarBackground = pUIManaBarBackgroundObj->AddComponent<CUIButton>("PartyStatus");

		CTransform* pPartyStatusTr = pUIManaBarBackgroundObj->GetTransform();
		pPartyStatusTr->SetWorldPos(basePos.x + 75.f, basePos.y + 27.f, 1.f);
		pPartyStatusTr->SetWorldScale(200.f*scale, 30.f*scale, 1.f);
		CRenderer2D* pRenderer = pUIManaBarBackgroundObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "UIManaBackground",
			0, 0, L"UserInterface/UI_GAUGE_MP.png");
		SAFE_RELEASE(pPartyStatusTr);
		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pUIManaBarBackgroundObj);
		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
	}

	{
		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer*	pLayer = pScene->GetLayer("UI+2");
		CGameObject* pUIPureBarBackgroundObj = CGameObject::CreateObject("UIPureBackground", pLayer);
		this->pUIPureBarBackground = pUIPureBarBackgroundObj->AddComponent<CUIButton>("PartyStatus");

		CTransform* pPartyStatusTr = pUIPureBarBackgroundObj->GetTransform();
		pPartyStatusTr->SetWorldPos(basePos.x + 75.f, basePos.y + 55.f, 1.f);
		pPartyStatusTr->SetWorldScale(200.f*scale, 30.f*scale, 1.f);
		CRenderer2D* pRenderer = pUIPureBarBackgroundObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "UIPureBackground",
			0, 0, L"UserInterface/UI_GAUGE_GREENP.png");
		SAFE_RELEASE(pPartyStatusTr);
		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pUIPureBarBackgroundObj);
		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
	}

	{
		CLayer*	pUILayer = m_pScene->GetLayer("UI+2");

		CGameObject*	pMPUIObj = CGameObject::CreateObject("MPUI", pUILayer);
		pUIManaBar = pMPUIObj->AddComponent<CUIButton>("MPUI");
		pUIManaBar->setUILength(95.f);
		pUIManaBar->setLengthRatio(1.f);

		CTransform*	pButtonTr = pMPUIObj->GetTransform();
		//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
		pButtonTr->SetWorldScale(100.f, 19.f, 1.f);
		pButtonTr->SetWorldPos(basePos.x + 95.f, basePos.y + 30.f, 0.f);

		SAFE_RELEASE(pButtonTr);

		CRenderer2D* pRenderer = pMPUIObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		pRenderer->SetShader(UI_HEARTH_BAR_SHADER);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "UIManaPoint",
			0, 0, L"UserInterface/UI_MP.png");

		SAFE_RELEASE(pMaterial);

		SAFE_RELEASE(pRenderer);

		CColliderRect* pRC = pMPUIObj->FindComponentFromType<CColliderRect>(CT_COLLIDER);

		pRC->SetRect(0, 0, 125, 25);

		SAFE_RELEASE(pRC);

		CFont* HPFont = pMPUIObj->AddComponent<CFont>("HPFont");
		HPFont->SetFont("³ª´®°íµñ");
		HPFont->SetText(L"");
		HPFont->SetArea(25, 0, 200, 25.f);
		SAFE_RELEASE(HPFont);

		SAFE_RELEASE(pMPUIObj);

		SAFE_RELEASE(pUILayer);
	}

	{
		CLayer*	pUILayer = m_pScene->GetLayer("UI+2");

		CGameObject*	pHPUIObj = CGameObject::CreateObject("HPUI", pUILayer);
		pUIHearthBar = pHPUIObj->AddComponent<CUIButton>("HPUI");
		pUIHearthBar->setUILength(95.f);
		pUIHearthBar->setLengthRatio(1.f);

		CTransform*	pButtonTr = pHPUIObj->GetTransform();
		//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
		pButtonTr->SetWorldScale(100.f, 19.f, 1.f);
		pButtonTr->SetWorldPos(basePos.x + 95.f, basePos.y + 1.f, 0.f);

		SAFE_RELEASE(pButtonTr);

		CRenderer2D* pRenderer = pHPUIObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		pRenderer->SetShader(UI_HEARTH_BAR_SHADER);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "HealthPoint",
			0, 0, L"UserInterface/UI_HP.png");

		SAFE_RELEASE(pMaterial);

		SAFE_RELEASE(pRenderer);

		CColliderRect* pRC = pHPUIObj->FindComponentFromType<CColliderRect>(CT_COLLIDER);

		pRC->SetRect(0, 0, 125, 25);

		SAFE_RELEASE(pRC);

		CFont* HPFont = pHPUIObj->AddComponent<CFont>("HPFont");
		HPFont->SetFont("³ª´®°íµñ");
		HPFont->SetText(L"");
		HPFont->SetArea(25, 0, 200, 25.f);
		SAFE_RELEASE(HPFont);

		SAFE_RELEASE(pHPUIObj);

		SAFE_RELEASE(pUILayer);
	}

	{
		CLayer*	pUILayer = m_pScene->GetLayer("UI+2");

		CGameObject*	pHPUIObj = CGameObject::CreateObject("PureUI", pUILayer);
		pUIPureBar = pHPUIObj->AddComponent<CUIButton>("PureUI");
		pUIPureBar->setUILength(95.f);
		pUIPureBar->setLengthRatio(1.f);

		CTransform*	pButtonTr = pHPUIObj->GetTransform();
		//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
		pButtonTr->SetWorldScale(100.f, 19.f, 1.f);
		pButtonTr->SetWorldPos(basePos.x + 95.f, basePos.y + 58.f, 0.f);

		SAFE_RELEASE(pButtonTr);

		CRenderer2D* pRenderer = pHPUIObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		pRenderer->SetShader(UI_HEARTH_BAR_SHADER);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "PurePoint",
			0, 0, L"UserInterface/UI_GREENP.png");

		SAFE_RELEASE(pMaterial);

		SAFE_RELEASE(pRenderer);

		CColliderRect* pRC = pHPUIObj->FindComponentFromType<CColliderRect>(CT_COLLIDER);

		pRC->SetRect(0, 0, 125, 25);

		SAFE_RELEASE(pRC);

		CFont* HPFont = pHPUIObj->AddComponent<CFont>("PureFont");
		HPFont->SetFont("³ª´®°íµñ");
		HPFont->SetText(L"");
		HPFont->SetArea(25, 0, 200, 25.f);
		SAFE_RELEASE(HPFont);

		SAFE_RELEASE(pHPUIObj);

		SAFE_RELEASE(pUILayer);
	}

	return true;
}

void PartyStatus::enableRender(bool show)
{
	CGameObject* pPartyStatusBaseObj = this->GetGameObject();
	pPartyStatusBaseObj->Enable(show);
	SAFE_RELEASE(pPartyStatusBaseObj);
	CGameObject* pUIHearthBarBackgroundObj = pUIHearthBarBackground->GetGameObject();
	pUIHearthBarBackgroundObj->Enable(show);
	SAFE_RELEASE(pUIHearthBarBackgroundObj);

	CGameObject* pUIHearthBarObj = pUIHearthBar->GetGameObject();
	pUIHearthBarObj->Enable(show);
	SAFE_RELEASE(pUIHearthBarObj);

	CGameObject* pUIManaBarBackgroundObj = pUIManaBarBackground->GetGameObject();
	pUIManaBarBackgroundObj->Enable(show);
	SAFE_RELEASE(pUIManaBarBackgroundObj);
	CGameObject* pUIManaBarObj = pUIManaBar->GetGameObject();
	pUIManaBarObj->Enable(show);
	SAFE_RELEASE(pUIManaBarObj);

	CGameObject* pUIPureBarBackgroundObj = pUIPureBarBackground->GetGameObject();
	pUIPureBarBackgroundObj->Enable(show);
	SAFE_RELEASE(pUIPureBarBackgroundObj);

	CGameObject* pUIPureBarObj = pUIPureBar->GetGameObject();
	pUIPureBarObj->Enable(show);
	SAFE_RELEASE(pUIPureBarObj);
}

void PartyStatus::OnCollisionEnter(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void PartyStatus::OnCollision(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void PartyStatus::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

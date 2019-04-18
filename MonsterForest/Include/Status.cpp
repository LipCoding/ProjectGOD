#include "Status.h"
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

Status::Status()
{
}


Status::~Status()
{
}

bool Status::initialize()
{
	{
		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer*	pLayer = pScene->GetLayer("UI+2");
		CGameObject* pUIHearthBarBackgroundObj = CGameObject::CreateObject("UIHearthBarBackground", pLayer);
		this->pUIHearthBarBackground = pUIHearthBarBackgroundObj->AddComponent<CUIButton>("Status");

		CTransform* pStatusTr = pUIHearthBarBackgroundObj->GetTransform();
		pStatusTr->SetWorldPos(100.f, 5.f, 1.f);
		pStatusTr->SetWorldScale(200.f, 30.f, 1.f);
		CRenderer2D* pRenderer = pUIHearthBarBackgroundObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "UIHPBackground",
			0, 0, L"UserInterface/UI_GAUGE_HP.png");
		SAFE_RELEASE(pStatusTr);
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
		this->pUIManaBarBackground = pUIManaBarBackgroundObj->AddComponent<CUIButton>("Status");

		CTransform* pStatusTr = pUIManaBarBackgroundObj->GetTransform();
		pStatusTr->SetWorldPos(100.f, 35.f, 1.f);
		pStatusTr->SetWorldScale(200.f, 30.f, 1.f);
		CRenderer2D* pRenderer = pUIManaBarBackgroundObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "UIManaBackground",
			0, 0, L"UserInterface/UI_GAUGE_MP.png");
		SAFE_RELEASE(pStatusTr);
		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pUIManaBarBackgroundObj);
		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
	}

	{
		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer*	pLayer = pScene->GetLayer("UI+2");
		CGameObject* pUISkillBarBackgroundObj = CGameObject::CreateObject("UISkillBarBackground", pLayer);
		this->pUISkillBarBackground = pUISkillBarBackgroundObj->AddComponent<CUIButton>("Status");

		CTransform* pStatusTr = pUISkillBarBackgroundObj->GetTransform();
		pStatusTr->SetWorldPos(100.f, 65.f, 1.f);
		pStatusTr->SetWorldScale(200.f, 30.f, 1.f);
		CRenderer2D* pRenderer = pUISkillBarBackgroundObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "UISkillBackground",
			0, 0, L"UserInterface/UI_GAUGE_GREENP.png");
		SAFE_RELEASE(pStatusTr);
		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pUISkillBarBackgroundObj);
		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
	}

	{
		CLayer*	pUILayer = m_pScene->GetLayer("UI+2");

		CGameObject*	pMPUIObj = CGameObject::CreateObject("MPUI", pUILayer);
		CUIButton* pUIManaBar = pMPUIObj->AddComponent<CUIButton>("MPUI");
		pUIManaBar->setUILength(125.f);
		pUIManaBar->setLengthRatio(1.f);

		CTransform*	pButtonTr = pMPUIObj->GetTransform();
		//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
		pButtonTr->SetWorldScale(125.f, 20.f, 1.f);
		pButtonTr->SetWorldPos(127.f, 41.f, 0.f);

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



	return true;
}

void Status::OnCollisionEnter(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void Status::OnCollision(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void Status::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

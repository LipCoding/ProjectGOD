#include "PGMessageBox.h"
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
#include "Core/NetworkManager.h"
#include "UserInfoUI.h"
#include "InviteToParty.h"
#include "TradeUI.h"
#include "PGCancel.h"
#include "PGConfirm.h"


PGMessageBox::PGMessageBox()
{
}


PGMessageBox::~PGMessageBox()
{
}

void PGMessageBox::initialize()
{

	{
		// 상대유저정보
		{
			CLayer* pLayer = m_pScene->GetLayer("UI+3");
			CGameObject* pPGConfirmObj = CGameObject::CreateObject("PGConfirm", pLayer);
			this->pConfirm = pPGConfirmObj->AddComponent<PGConfirm>("confirm");
			static_cast<PGConfirm*>(pConfirm)->initialize();
			CTransform*	pPGTransform = pPGConfirmObj->GetTransform();
			pPGTransform->SetWorldPos(0.f, 0.f, 0.f);
			pPGTransform->SetWorldScale(80.f, 40.f, 1.f);
			SAFE_RELEASE(pPGTransform);
			pPGConfirmObj->Enable(false);
			CRenderer2D* pRenderer = pPGConfirmObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
			CMaterial* pMaterial = pRenderer->GetMaterial();

			pMaterial->SetDiffuseTexInfo("Linear", "PGConfirm",
				0, 0, L"UserInterface/UI_BASIC_BOX_LONG.png");

			SAFE_RELEASE(pMaterial);
			SAFE_RELEASE(pRenderer);

			CFont* PGConfirmFont = pPGConfirmObj->AddComponent<CFont>("PGConfirmFont");
			PGConfirmFont->SetFont("돋움25");
			PGConfirmFont->SetText(L"확인");
			PGConfirmFont->SetArea(10, 30, 100, 30.f);
			SAFE_RELEASE(PGConfirmFont);

			CColliderRect* pRC = pPGConfirmObj->FindComponentFromType<CColliderRect>(CT_COLLIDER);
			pRC->SetRect(0, 0, 80.f, 40.f);

			SAFE_RELEASE(pRC);
			SAFE_RELEASE(pPGConfirmObj);
			SAFE_RELEASE(pLayer);
		}
	}

	{
		CLayer* pLayer = m_pScene->GetLayer("UI+3");
		CGameObject* pPGCancelObj = CGameObject::CreateObject("PGCancel", pLayer);
		this->pCancel = pPGCancelObj->AddComponent<PGCancel>("confirm");
		static_cast<PGCancel*>(pCancel)->initialize();
		CTransform*	pSlotTr = pPGCancelObj->GetTransform();
		pSlotTr->SetWorldPos(0.f, 0.f, 0.f);
		pSlotTr->SetWorldScale(80.f, 40.f, 1.f);
		SAFE_RELEASE(pSlotTr);
		pPGCancelObj->Enable(false);
		CRenderer2D* pRenderer = pPGCancelObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "PGCancel",
			0, 0, L"UserInterface/UI_BASIC_BOX_LONG.png");

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);

		CFont* UserInfoFont = pPGCancelObj->AddComponent<CFont>("UserInfoFont");
		UserInfoFont->SetFont("돋움25");
		UserInfoFont->SetText(L"취소");
		UserInfoFont->SetArea(10, 30, 90, 30.f);
		SAFE_RELEASE(UserInfoFont);

		CColliderRect* pRC = pPGCancelObj->FindComponentFromType<CColliderRect>(CT_COLLIDER);
		pRC->SetRect(0, 0, 80.f, 40.f);

		SAFE_RELEASE(pRC);
		SAFE_RELEASE(pPGCancelObj);
		SAFE_RELEASE(pLayer);
	}
}

int PGMessageBox::Update(float fTime)
{
	CTransform* pMessageBoxTr = this->GetTransform();
	Vector3 basePos = pMessageBoxTr->GetWorldPos();
	SAFE_RELEASE(pMessageBoxTr);
	{
		CTransform* pTransform = this->pConfirm->GetTransform();
		pTransform->SetWorldPos(Vector3(basePos.x + 15, basePos.y + 100, 0.f));
		SAFE_RELEASE(pTransform);
	}
	{
		CTransform* pTransform = this->pCancel->GetTransform();
		pTransform->SetWorldPos(Vector3(basePos.x + 110, basePos.y + 100, 0.f));
		SAFE_RELEASE(pTransform);
	}

	return 0;
}

void PGMessageBox::enableRender(bool show)
{
	this->m_pGameObject->Enable(show);
	{
		CGameObject* pGameObject = this->pConfirm->GetGameObject();
		pGameObject->Enable(show);
		SAFE_RELEASE(pGameObject);
	}

	{
		CGameObject* pGameObject = pCancel->GetGameObject();
		pGameObject->Enable(show);
		SAFE_RELEASE(pGameObject);
	}
}

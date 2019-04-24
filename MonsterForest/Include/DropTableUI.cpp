#include "DropTableUI.h"
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
#include "DropItemSlot.h"
DropTableUI::DropTableUI()
{
}


DropTableUI::~DropTableUI()
{
}

bool DropTableUI::initialize()
{
	CTransform* pDropTableTr = this->GetTransform();
	Vector3 basePos = pDropTableTr->GetWorldPos();
	SAFE_RELEASE(pDropTableTr);
	// DropItemSlot1
	{
		CLayer* pLayer = m_pScene->GetLayer("UI+2");
		CGameObject* pObj = CGameObject::CreateObject("DropItemSlot1", pLayer);
		pDropItemSlot1 = pObj->AddComponent<DropItemSlot>("DropItemSlot1");
		CTransform* pDropItemSlot1Tr = pDropItemSlot1->GetTransform();
		pDropItemSlot1Tr->SetWorldScale(180.f, 45.f, 1.f);
		pDropItemSlot1Tr->SetWorldPos(Vector3(basePos.x, basePos.y + 10, 0.f));
		CRenderer2D* pRenderer = pObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "DropItemSlot1",
			0, 0, L"UserInterface/UI_INVEN_BOX_1.png");

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);

		CColliderRect* pRC = pObj->FindComponentFromType<CColliderRect>(CT_COLLIDER);
		pRC->SetRect(0, 0, 32.f, 32.f);
		pObj->Enable(false);
		SAFE_RELEASE(pRC);
		SAFE_RELEASE(pLayer);
	}

	// DropItemSlot2
	{
		CLayer* pLayer = m_pScene->GetLayer("UI+2");
		CGameObject* pObj = CGameObject::CreateObject("DropItemSlot2", pLayer);
		pDropItemSlot2 = pObj->AddComponent<DropItemSlot>("DropItemSlot1");
		CTransform* pDropItemSlot2Tr = pDropItemSlot2->GetTransform();
		pDropItemSlot2Tr->SetWorldScale(180.f, 45.f, 1.f);
		pDropItemSlot2Tr->SetWorldPos(Vector3(basePos.x, basePos.y + 40, 0.f));
		CRenderer2D* pRenderer = pObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "DropItemSlot2",
			0, 0, L"UserInterface/UI_INVEN_BOX_1.png");

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);

		CColliderRect* pRC = pObj->FindComponentFromType<CColliderRect>(CT_COLLIDER);
		pRC->SetRect(0, 0, 32.f, 32.f);
		pObj->Enable(false);
		SAFE_RELEASE(pRC);
		SAFE_RELEASE(pLayer);
	}

	// DropItemSlot3
	{
		CLayer* pLayer = m_pScene->GetLayer("UI+2");
		CGameObject* pObj = CGameObject::CreateObject("DropItemSlot3", pLayer);
		pDropItemSlot3 = pObj->AddComponent<DropItemSlot>("DropItemSlot3");
		CTransform* pDropItemSlot3Tr = pDropItemSlot3->GetTransform();
		pDropItemSlot3Tr->SetWorldScale(180.f, 45.f, 1.f);
		pDropItemSlot3Tr->SetWorldPos(Vector3(basePos.x, basePos.y + 70, 0.f));
		CRenderer2D* pRenderer = pObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "DropItemSlot3",
			0, 0, L"UserInterface/UI_INVEN_BOX_1.png");

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);

		CColliderRect* pRC = pObj->FindComponentFromType<CColliderRect>(CT_COLLIDER);
		pRC->SetRect(0, 0, 32.f, 32.f);
		pObj->Enable(false);
		SAFE_RELEASE(pRC);
		SAFE_RELEASE(pLayer);
	}

	return true;
}

void DropTableUI::enableRender(bool show)
{
	CGameObject* pDropTableObj = this->GetGameObject();
	pDropTableObj->Enable(show);
	SAFE_RELEASE(pDropTableObj);

	CGameObject* pDropItemSlot1Obj = this->pDropItemSlot1->GetGameObject();
	pDropItemSlot1Obj->Enable(show);
	SAFE_RELEASE(pDropItemSlot1Obj);

	CGameObject* pDropItemSlot2Obj = this->pDropItemSlot2->GetGameObject();
	pDropItemSlot2Obj->Enable(show);
	SAFE_RELEASE(pDropItemSlot2Obj);

	CGameObject* pDropItemSlot3Obj = this->pDropItemSlot3->GetGameObject();
	pDropItemSlot3Obj->Enable(show);
	SAFE_RELEASE(pDropItemSlot3Obj);

}

int DropTableUI::Update(float fTime)
{
	CTransform* pDropTableTr = this->GetTransform();
	Vector3 basePos = pDropTableTr->GetWorldPos();
	SAFE_RELEASE(pDropTableTr);

	CGameObject* pDropItemSlot1Obj = this->pDropItemSlot1->GetGameObject();
	CTransform* pDropItemSlot1Tr = pDropItemSlot1Obj->GetTransform();
	pDropItemSlot1Tr->SetWorldPos(Vector3(basePos.x+10, basePos.y + 10, 0.f));
	SAFE_RELEASE(pDropItemSlot1Tr);
	SAFE_RELEASE(pDropItemSlot1Obj);

	CGameObject* pDropItemSlot2Obj = this->pDropItemSlot2->GetGameObject();
	CTransform* pDropItemSlot2Tr = pDropItemSlot2Obj->GetTransform();
	pDropItemSlot2Tr->SetWorldPos(Vector3(basePos.x+10, basePos.y + 55, 0.f));
	SAFE_RELEASE(pDropItemSlot2Tr);
	SAFE_RELEASE(pDropItemSlot2Obj);

	CGameObject* pDropItemSlot3Obj = this->pDropItemSlot3->GetGameObject();
	CTransform* pDropItemSlot3Tr = pDropItemSlot3Obj->GetTransform();
	pDropItemSlot3Tr->SetWorldPos(Vector3(basePos.x+10, basePos.y + 100, 0.f));
	SAFE_RELEASE(pDropItemSlot3Tr);
	SAFE_RELEASE(pDropItemSlot3Obj);
	return 0;
}

void DropTableUI::OnCollisionEnter(CCollider * pSrc, CCollider * pDest, float fTime)
{

}

void DropTableUI::OnCollision(CCollider * pSrc, CCollider * pDest, float fTime)
{

}

void DropTableUI::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

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

	return true;
}

void DropTableUI::enableRender(bool show)
{
	CGameObject* pDropTableObj = this->GetGameObject();
	pDropTableObj->Enable(show);
	SAFE_RELEASE(pDropTableObj);
	for (const auto& dropItemSlot : dropItemSlots)
	{
		CGameObject* pDropItemSlot1Obj = dropItemSlot->GetGameObject();
		pDropItemSlot1Obj->Enable(show);
		SAFE_RELEASE(pDropItemSlot1Obj);
	}
}

void DropTableUI::addDropItemSlot(const wstring& itemname)
{
	CTransform* pDropTableTr = this->GetTransform();
	Vector3 basePos = pDropTableTr->GetWorldPos();
	SAFE_RELEASE(pDropTableTr);
	// 해당 아이템에 대한 슬롯 생성.
			// DropItemSlot1
	{
		int index = dropItemSlots.size();
		char str[128];
		string appendTag = _itoa(index, str, 10);
		string slotTag = "DropSlot" + appendTag;
		CLayer* pLayer = m_pScene->GetLayer("UI+2");
		CGameObject* pObj = CGameObject::CreateObject(slotTag, pLayer);
		DropItemSlot* pDropItemSlot1 = pObj->AddComponent<DropItemSlot>("DropItemSlot1");
		static_cast<DropItemSlot*>(pDropItemSlot1)->setDropTableUI(this);
		static_cast<DropItemSlot*>(pDropItemSlot1)->setIndex(dropItemSlots.size());
		CTransform* pDropItemSlot1Tr = pDropItemSlot1->GetTransform();
		pDropItemSlot1Tr->SetWorldScale(180.f, 45.f, 1.f);
		Vector3 pos = Vector3(basePos.x, basePos.y + 10 + (dropItemSlots.size() * 30), 0.f);
		pDropItemSlot1Tr->SetWorldPos(pos);
		CRenderer2D* pRenderer = pObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", slotTag,
			0, 0, L"UserInterface/UI_INVEN_BOX_1.png");
		pDropItemSlot1->initialize(itemname);
		dropItemSlots.push_back(pDropItemSlot1);
		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);

		CColliderRect* pRC = pObj->FindComponentFromType<CColliderRect>(CT_COLLIDER);
		pRC->SetRect(0, 0, 180.f, 40.f);
		//pObj->Enable(false);
		SAFE_RELEASE(pRC);
		SAFE_RELEASE(pLayer);
	}
}

void DropTableUI::detachDropItemSlot(int slotNumber)
{
	auto iter = dropItemSlots.begin();
	for (int i = 0; i < slotNumber; ++i)
	{
		++iter;
	}
	static_cast<DropItemSlot*>((*iter))->detachItem();
	/*(*iter)->Die();

	CUIButton* pButton = (*iter);
	CGameObject* pObject = pButton->GetGameObject();
	CColliderRect* pRC = pObject->FindComponentFromType<CColliderRect>(CT_COLLIDER);
	pRC->SetRect(0, 0, 0, 0);
	SAFE_RELEASE(pRC);
	SAFE_RELEASE(pObject);
	dropItemSlots.erase(iter);
	SAFE_RELEASE(pButton);
	for (int i = 0; i < dropItemSlots.size(); ++i)
	{
		static_cast<DropItemSlot*>(dropItemSlots[i])->setIndex(i);
	}*/
}

int DropTableUI::Update(float fTime)
{
	CTransform* pDropTableTr = this->GetTransform();
	Vector3 basePos = pDropTableTr->GetWorldPos();
	SAFE_RELEASE(pDropTableTr);

	for (int i = 0; i < dropItemSlots.size(); ++i)
	{
		CGameObject* pDropItemSlot1Obj = dropItemSlots[i]->GetGameObject();
		CTransform* pDropItemSlot1Tr = pDropItemSlot1Obj->GetTransform();
		int index = static_cast<DropItemSlot*>(dropItemSlots[i])->getIndex();
		pDropItemSlot1Tr->SetWorldPos(Vector3(basePos.x + 10, basePos.y + 10 + (i*50), 0.f));
		SAFE_RELEASE(pDropItemSlot1Tr);
		SAFE_RELEASE(pDropItemSlot1Obj);
	}

	return 0;
}
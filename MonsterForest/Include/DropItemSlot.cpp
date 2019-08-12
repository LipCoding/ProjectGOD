#include "DropItemSlot.h"
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
#include "DropTableUI.h"
#include "ObjectScript/Player.h"
#include "NetworkManager.h"

DropItemSlot::DropItemSlot()
{
}


DropItemSlot::~DropItemSlot()
{
	if (nullptr != pItemUI)
	{
		pItemUI->Die();
		SAFE_RELEASE(pItemUI);
		pItemUI = nullptr;
	}
}

bool DropItemSlot::initialize(const wstring& itemname)
{
	CTransform* pDropTableTr = this->GetTransform();
	Vector3 basePos = pDropTableTr->GetWorldPos();
	SAFE_RELEASE(pDropTableTr);

	CLayer* pLayer = m_pScene->GetLayer("UI+3");

	char str[128];
	string appendTag = _itoa(index, str, 10);
	string itemTag = "DropSlot" + appendTag + "Item";

	CGameObject* pObj = CGameObject::CreateObject(itemTag, pLayer);
	pItemUI = pObj->AddComponent<CUIPanel>("DropItemSlot1");
	CTransform* pItemUITr = pItemUI->GetTransform();
	pItemUITr->SetWorldScale(40.f, 40.f, 1.f);
	pItemUITr->SetWorldPos(Vector3(basePos.x + 5, basePos.y + 5, 0.f));
	SAFE_RELEASE(pItemUITr);
	CRenderer2D* pRenderer = pObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
	CMaterial* pMaterial = pRenderer->GetMaterial();

	wstring itemName = L"ItemIcon/" + itemname;
	itemName = itemName + L".png";

	string itemKey = strconv(itemname);

	pMaterial->SetDiffuseTexInfo("Linear", itemKey,
		0, 0, itemName.c_str());

	CFont* itemFont = pObj->AddComponent<CFont>("HPFont");
	itemFont->SetFont("³ª´®°íµñ");
	itemFont->SetText(itemname);
	itemFont->SetArea(50, 0, 200, 30.f);
	SAFE_RELEASE(itemFont);

	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);

	CColliderRect* pRC = pObj->FindComponentFromType<CColliderRect>(CT_COLLIDER);
	pRC->SetRect(0, 0, 0, 0.f);
	SAFE_RELEASE(pRC);
	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pObj);

	return true;
}

int DropItemSlot::Update(float fTime)
{
	CTransform* pDropTableTr = this->GetTransform();
	Vector3 basePos = pDropTableTr->GetWorldPos();
	SAFE_RELEASE(pDropTableTr);

	if (nullptr != pItemUI)
	{
		CGameObject* pDropItemSlot1Obj = pItemUI->GetGameObject();
		CTransform* pDropItemSlot1Tr = pDropItemSlot1Obj->GetTransform();
		pDropItemSlot1Tr->SetWorldPos(Vector3(basePos.x + 5, basePos.y + 5, 0.f));
		char str[128];
		string appendTag = _itoa(index, str, 10);
		string itemTag = "DropSlot" + appendTag + "Item";
		pItemUI->SetTag(itemTag);
		SAFE_RELEASE(pDropItemSlot1Tr);
		SAFE_RELEASE(pDropItemSlot1Obj);
	}

	return 0;
}

void DropItemSlot::detachItem()
{
	CGameObject* pGameObject = this->pItemUI->GetGameObject();
	pGameObject->Die();
	SAFE_RELEASE(pGameObject);
	this->pItemUI = nullptr;
}

void DropItemSlot::OnCollisionEnter(CCollider * pSrc, CCollider * pDest, float fTime)
{

}

void DropItemSlot::OnCollision(CCollider * pSrc, CCollider * pDest, float fTime)
{

	if (KEYDOWN("MouseLButton"))
	{
		if ((pSrc->GetTag() == "MousePoint" || pDest->GetTag() == "MousePoint") && GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			char str[128];
			string appendTag = _itoa(NetworkManager::getInstance()->getMyClientID(), str, 10);
			string objectTag = "Player" + appendTag;
			CGameObject* pGameObject = CGameObject::FindObject(objectTag);
			CPlayer* pPlayer = (CPlayer*)pGameObject->FindComponentFromTypeName<Actor>("Actor");

			cs_packet_additem_inventory* pPacket = reinterpret_cast<cs_packet_additem_inventory*>(NetworkManager::getInstance()->getSendBuffer());
			pPacket->size = sizeof(cs_packet_additem_inventory);
			pPacket->type = CS_PACKET_ADDITEM_INVENTORY;
			pPacket->targetId = pPlayer->clickedID;
			pPacket->index = this->index;
			NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_additem_inventory);
			DWORD iobyte;
			int ret = WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);

			SAFE_RELEASE(pPlayer);
			SAFE_RELEASE(pGameObject);
		}
	}

}

void DropItemSlot::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void DropItemSlot::deleteSlot()
{
	this->pDropTableUI->detachDropItemSlot(index);
	this->Die();
}

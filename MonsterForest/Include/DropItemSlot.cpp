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
#include "Core/NetworkManager.h"

DropItemSlot::DropItemSlot()
{
}


DropItemSlot::~DropItemSlot()
{
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
			// 아이템을 클릭하면 서버에게 인벤토리로 아이템 추가 요청을 한다.
			/*
			// 서버로부터 해당 아이템이 어떤 종류가 있는지 테이블을 얻어온다.
				cs_packet_require_itemtable* packet = reinterpret_cast<cs_packet_require_itemtable*>(NetworkManager::getInstance()->getSendBuffer());
				packet->size = sizeof(cs_packet_require_itemtable);
				packet->type = CS_PACKET_ROOTING_TABLE;
				packet->targetId = id;
				DWORD iobyte;
				NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_require_itemtable);
				int ret = WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);
			*/
			cs_packet_require_itemtable* pPacket = reinterpret_cast<cs_packet_require_itemtable*>(NetworkManager::getInstance()->getSendBuffer());
			pPacket->size = sizeof(cs_packet_require_itemtable);
			pPacket->type = CS_PACKET_ADDITEM_INVENTORY;
			pPacket->index = this->index;
			NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_require_itemtable);
			DWORD iobyte;
			int ret = WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);

			if (ret)
			{
				char a = 0;
			}

			// 어떤 아이템슬롯을 눌렀는지. 어느 타겟인지.
			/*
			CLayer*	pLayer = m_pScene->GetLayer("UI+2");
			CGameObject* pItem = CGameObject::CreateObject("SWORD_ITEM", pLayer);

			CUIButton*	pItemUI = pItem->AddComponent<CUIButton>("SWORD_ITEM");
			SAFE_RELEASE(pItemUI);

			CTransform*	pItemTr = pItem->GetTransform();
			//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
			pItemTr->SetWorldScale(30.f, 30.f, 1.f);

			SAFE_RELEASE(pItemTr);
			CRenderer2D* pRenderer = pItem->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
			CMaterial* pMaterial = pRenderer->GetMaterial();

			pMaterial->SetDiffuseTexInfo("Linear", "SWORD_ICON",
				0, 0, L"SWORD_ICON.jpg");

			SAFE_RELEASE(pMaterial);
			SAFE_RELEASE(pRenderer);

			CColliderRect* pRC = pItem->FindComponentFromType<CColliderRect>(CT_COLLIDER);

			pRC->SetRect(0, 0, 0, 0);

			SAFE_RELEASE(pRC);

			GET_SINGLE(UserInterfaceManager)->getInventory()->addItem(pItem);
			SAFE_RELEASE(pItem);
			*/
			//this->pDropTableUI->detachDropItemSlot(index);
			//this->Die();
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

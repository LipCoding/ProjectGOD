#include "Slot.h"
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

Slot::Slot()
{
	SetTag("Slot");
	SetTypeName("Slot");
	SetTypeID<Slot>();
	m_eType = CT_UI;
}

Slot::~Slot()
{
}

bool Slot::initialize(int index)
{
	this->index = index;

	return true;
}

void Slot::addItem(CGameObject* pItem)
{
	this->pItem = pItem;
	this->pItem->Enable(false);
}

void Slot::dropItem()
{
	// 월드에 아이템 생성

	this->pItem = nullptr;
}

void Slot::removeItem()
{
	pItem->Die();
}

bool Slot::isHaveItem()
{
	return this->pItem != nullptr;
}

void Slot::update(float time)
{
	if (pItem != nullptr)
	{

		CTransform* pSlotTr = this->m_pGameObject->GetTransform();
		CTransform* pItemTr = this->pItem->GetTransform();

		Vector3 slotPos = pSlotTr->GetWorldPos();
		pItemTr->SetWorldPos(Vector3(slotPos.x + 1, slotPos.y + 1, 0.f));

		SAFE_RELEASE(pItemTr);
		SAFE_RELEASE(pSlotTr);

	}
}

void Slot::move(const Vector3& movement)
{
	CTransform*	pSlotTr = this->m_pGameObject->GetTransform();

	if (false == equipslot)
	{
		pSlotTr->SetWorldPos(movement.x - 15 + ((index % 5) * 32) + 50 + ((index % 5) * 5), movement.y + ((index / 5) * 32) + 200 + ((index / 5) * 5), 0.f);
	}
	else
	{
		pSlotTr->SetWorldPos(movement.x + offsetPos.x, movement.y + offsetPos.y, 0.f);
	}
	SAFE_RELEASE(pSlotTr);
}

void Slot::showItem(bool show)
{
	if(this->pItem != nullptr)
		this->pItem->Enable(show);
}

void Slot::OnCollisionEnter(CCollider * pSrc, CCollider * pDest, float fTime)
{

}

void Slot::OnCollision(CCollider * pSrc, CCollider * pDest, float fTime)
{

	if (KEYDOWN("MouseLButton"))
	{
		 //현재 몇번째 인덱스를 선택했는지 알려준다.
		 //아이템을 갖고있는지 확인
		// 아이템을 
		if (pDest->GetTag() == "MousePoint")
		{
			if (false == equipslot)
			{
				GET_SINGLE(UserInterfaceManager)->getInventory()->setCurrentIndex(-1);
				GET_SINGLE(UserInterfaceManager)->getInventory()->setTempItem(nullptr);
				GET_SINGLE(UserInterfaceManager)->getInventory()->setCurrentIndex(this->index);
				if (this->pItem != nullptr)
				{
					GET_SINGLE(UserInterfaceManager)->getInventory()->setTempItem(this->pItem);
				}
			}
			else
			{

			}
		}
	}
	if (KEYUP("MouseLButton"))
	{
		if (pDest->GetTag() == "MousePoint")
		{
			if (this->pItem == nullptr)
			{
				if (false == equipslot)
				{
					this->pItem = GET_SINGLE(UserInterfaceManager)->getInventory()->getTempItem();
					int prevIndex = GET_SINGLE(UserInterfaceManager)->getInventory()->getCurrentIndex();
					int row = prevIndex / 5;
					int col = prevIndex % 5;
					GET_SINGLE(UserInterfaceManager)->getInventory()->getSlot()[row][col]->pItem = nullptr;

					// 서버에 아이템 이동패킷을 보낸다.
					cs_packet_moveitem_inventory* packet = reinterpret_cast<cs_packet_moveitem_inventory*>(NetworkManager::getInstance()->getSendBuffer());
					packet->size = sizeof(cs_packet_moveitem_inventory);
					packet->type = CS_PACKET_MOVEITEM_INVENTORY;
					packet->fromslot = prevIndex;
					packet->toslot = this->index;

					DWORD iobyte;
					NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_require_itemtable);
					int ret = WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);
				}
				else
				{
				}
			}
		}
	}
}

void Slot::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{

}

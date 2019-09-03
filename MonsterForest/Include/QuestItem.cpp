#include "QuestItem.h"
#include "QuestBaseUI.h"
#include "Core/PathManager.h"
#include "Scene/Layer.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/Renderer2D.h"
#include "Component/ColliderRect.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "QuestBaseUI.h"
#include "Component/Font.h"
#include "QuestManager.h"
#include "QuestItem.h"
#include "Quest.h"
#include "Core/Input.h"
#include "NetworkManager.h"

QuestItem::QuestItem()
{
	SetTag("QuestItem");
	SetTypeName("QuestItem");
	SetTypeID<QuestItem>();
	m_eType = CT_UI;
}


QuestItem::~QuestItem()
{
}

void QuestItem::OnCollisionEnter(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void QuestItem::OnCollision(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (KEYDOWN("MouseLButton"))
	{
		//���� ���° �ε����� �����ߴ��� �˷��ش�.
		//�������� �����ִ��� Ȯ��
	   // �������� 
		/*
			// ������ ������ �̵���Ŷ�� ������.
					cs_packet_moveitem_inventory* packet = reinterpret_cast<cs_packet_moveitem_inventory*>(NetworkManager::getInstance()->getSendBuffer());
					packet->size = sizeof(cs_packet_moveitem_inventory);
					packet->type = CS_PACKET_MOVEITEM_INVENTORY;
					packet->fromslot = prevIndex;
					packet->toslot = this->index;
					packet->isEquipSlot = false;
					DWORD iobyte;
					NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_require_itemtable);
					int ret = WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);
		*/


		if (pDest->GetTag() == "MousePoint")
		{
			pBaseUI->setCurrentQuest(this->quest);
			pBaseUI->updateQuestView();
			
		}
	}
}

void QuestItem::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

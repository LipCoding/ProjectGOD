#include "InviteToParty.h"
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
#include "NetworkManager.h"
#include "TargetPlayerUI.h"

InviteToParty::InviteToParty()
{
}


InviteToParty::~InviteToParty()
{
}

void InviteToParty::initialize()
{
}

void InviteToParty::OnCollisionEnter(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void InviteToParty::OnCollision(CCollider * pSrc, CCollider * pDest, float fTime)
{
	/*
	서버에 파티신청 패킷을 보낸다.
	*/
	if (KEYDOWN("MouseLButton"))
	{
		if ((pSrc->GetTag() == "MousePoint" || pDest->GetTag() == "MousePoint") && GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			/*
			cs_packet_require_itemtable* pPacket = reinterpret_cast<cs_packet_require_itemtable*>(NetworkManager::getInstance()->getSendBuffer());
			pPacket->size = sizeof(cs_packet_require_itemtable);
			pPacket->type = CS_PACKET_ADDITEM_INVENTORY;
			pPacket->index = this->index;
			NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_require_itemtable);
			DWORD iobyte;
			int ret = WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);
			*/
			int id = NetworkManager::getInstance()->getMyClientID();
			char str[128];
			string appendTag = _itoa(id, str, 10);
			string objectTag = "Player" + appendTag;

			cs_packet_party * pPacket = reinterpret_cast<cs_packet_party*>(NetworkManager::getInstance()->getSendBuffer());
			pPacket->size = sizeof(cs_packet_party);
			pPacket->type = CS_PACKET_INVITE_PARTY;
			pPacket->fromID = id;
			pPacket->toID = pBaseUI->getPlayerNumber();

			NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_party);
			DWORD iobyte;
			int ret = WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);
		}
	}
}

void InviteToParty::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{

}

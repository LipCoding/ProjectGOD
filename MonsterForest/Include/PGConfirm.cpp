#include "PGConfirm.h"
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
#include "TargetPlayerUI.h"
#include "PGMessageBox.h"

PGConfirm::PGConfirm()
{
}


PGConfirm::~PGConfirm()
{
}

void PGConfirm::initialize()
{
}

void PGConfirm::OnCollisionEnter(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void PGConfirm::OnCollision(CCollider * pSrc, CCollider * pDest, float fTime)
{
	// 서버에 파티수락 패킷을 보낸다.
	if (KEYDOWN("MouseLButton"))
	{
		if ( pDest->GetTag() == "MousePoint")
		{
			//_cprintf("패킷전송");
			cs_packet_party * pPacket = reinterpret_cast<cs_packet_party*>(NetworkManager::getInstance()->getSendBuffer());
			pPacket->size = sizeof(cs_packet_party);
			pPacket->type = CS_PACKET_PARTY_CONFIRM;
			pPacket->fromID = GET_SINGLE(UserInterfaceManager)->getTargetPlayerUI()->getFromID();
			pPacket->toID = GET_SINGLE(UserInterfaceManager)->getTargetPlayerUI()->getToID();
			NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_party);
			DWORD iobyte;
			int ret = WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);
		}
		GET_SINGLE(UserInterfaceManager)->getPGMessageBox()->enableRender(false);
	}
}

void PGConfirm::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

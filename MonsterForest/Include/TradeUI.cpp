#include "TradeUI.h"
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


TradeUI::TradeUI()
{
}


TradeUI::~TradeUI()
{
}

void TradeUI::initialize()
{
}

void TradeUI::OnCollisionEnter(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void TradeUI::OnCollision(CCollider * pSrc, CCollider * pDest, float fTime)
{
	/*
	// 서버에 교환신청 패킷을 보낸다.
	*/
}

void TradeUI::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

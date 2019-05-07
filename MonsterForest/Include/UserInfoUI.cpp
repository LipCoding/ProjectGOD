#include "UserInfoUI.h"
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


UserInfoUI::UserInfoUI()
{
}


UserInfoUI::~UserInfoUI()
{
}

void UserInfoUI::initialize()
{
}

void UserInfoUI::OnCollisionEnter(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void UserInfoUI::OnCollision(CCollider * pSrc, CCollider * pDest, float fTime)
{
	// 서버에 유저정보보기 패킷을 보낸다.

}

void UserInfoUI::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

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
	// 마우스 우클릭 하면 아이템목록을 보여준다.
	if (KEYDOWN("MouseLButton"))
	{

		// 서버로부터 해당 아이템이 어떤 종류가 있는지 테이블을 얻어온다.


		// 아이템을 클릭하면 서버에게 요청을 한다.


		// 요청한 것이 성공패킷이 돌아오면 아이템을 인벤토리에 추가.

	}
}

void DropItemSlot::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

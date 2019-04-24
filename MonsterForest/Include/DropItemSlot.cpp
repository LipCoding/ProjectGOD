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
	// ���콺 ��Ŭ�� �ϸ� �����۸���� �����ش�.
	if (KEYDOWN("MouseLButton"))
	{

		// �����κ��� �ش� �������� � ������ �ִ��� ���̺��� ���´�.


		// �������� Ŭ���ϸ� �������� ��û�� �Ѵ�.


		// ��û�� ���� ������Ŷ�� ���ƿ��� �������� �κ��丮�� �߰�.

	}
}

void DropItemSlot::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

#include "Monster.h"
#include "Component/Transform.h"
#include "Component/Animation.h"
#include "Component/AnimationClip.h"
#include "Core/QuadTreeManager.h"
#include "GameObject/GameObject.h"

#include "Scene/Scene.h"
#include "Component/Collider.h"
#include "Component/Camera.h"
#include "Device.h"
#include "Core/Input.h"
#include "Component/ParticleSingle.h"
#include "Component/Animation2D.h"
#include "Component/Navigation3D.h"
#include "../ObjectScript/Effect.h"
#include "../SceneScript/MainScene.h"
#include "Scene/SceneManager.h"
#include "../Client.h"
#include "Player.h"
#include "../NetworkManager.h"
#include "../UserInterfaceManager.h"
#include "../DropItemSlot.h"
#include "../DropTableUI.h"
Monster::Monster()
{
}


Monster::~Monster()
{
}

void Monster::damaged(int damage)
{
	currentHP -= damage;
}

void Monster::attack(const string& target_tag)
{
	CAnimation* pAnimation = FindComponentFromType<CAnimation>(CT_ANIMATION);
	pAnimation->ChangeClip("Attack1");
	SAFE_RELEASE(pAnimation);

	CGameObject* target_object = CGameObject::FindObject(target_tag);
	if (nullptr == target_object)
		return;

	Actor* target_actor_component = target_object->FindComponentFromTypeName<Actor>("Actor");
	target_actor_component->damaged(attackDamage);
}

void Monster::rotate(float x, float y, float z)
{
	auto transform_component = GetTransform();
	transform_component->SetWorldRotY(XMConvertToRadians(y));
	SAFE_RELEASE(transform_component);
}

void Monster::move(float x, float y, float z, bool isBackMove)
{
	CTransform* pTransform = this->GetTransform();

	float yPos = GET_SINGLE(CQuadTreeManager)->GetY(Vector3(x, y, z));
	pTransform->SetWorldPos(x, yPos, z);

	CAnimation* pAnimation = FindComponentFromType<CAnimation>(CT_ANIMATION);
	if (nullptr == pAnimation)
		return;
	pAnimation->ChangeClip("Run1");

}

void Monster::changeAnimation(const string& clip_name)
{
	CAnimation* pAnimation = FindComponentFromType<CAnimation>(CT_ANIMATION);
	pAnimation->ChangeClip(clip_name);
	SAFE_RELEASE(pAnimation);
}

void Monster::worldscale(float x, float y, float z)
{
}

void Monster::setDieState(bool state)
{
	this->dieState = state;
	CAnimation* pAnimation = FindComponentFromType<CAnimation>(CT_ANIMATION);
	pAnimation->ChangeClip("Die1");
	SAFE_RELEASE(pAnimation);
}

void Monster::OnCollisionEnter(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void Monster::OnCollision(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseRay" && GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		string tag = m_pGameObject->GetTag();
		tag.erase(0, 6);
		int id = atoi(tag.c_str());

		char str[128];
		string appendTag = _itoa(NetworkManager::getInstance()->getMyClientID(), str, 10);
		string objectTag = "Player" + appendTag;
		CGameObject* pGameObject = CGameObject::FindObject(objectTag);
		CPlayer* pPlayer = pGameObject->FindComponentFromTypeName<CPlayer>("Actor");
		pPlayer->clickedID = id;

		CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
		float ratio = (float)this->getCurrentHP() / (float)this->getMaxHP();
		pEnemyUIHearthBar->setLengthRatio(ratio);

		CGameObject* pUIObject = pEnemyUIHearthBar->GetGameObject();
		GET_SINGLE(UserInterfaceManager)->getEnemyStatus()->enableRender(true);
		GET_SINGLE(UserInterfaceManager)->getEnemyStatus()->setTarget(this);
		SAFE_RELEASE(pUIObject);
	}

	if (KEYDOWN("MouseRButton"))
	{
		if (pDest->GetTag() == "MouseRay" && GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		{
			if (true == dieState)
			{
				string tag = m_pGameObject->GetTag();
				tag.erase(0, 6);
				int id = atoi(tag.c_str());
				// 서버로부터 해당 아이템이 어떤 종류가 있는지 테이블을 얻어온다.
				cs_packet_require_itemtable* packet = reinterpret_cast<cs_packet_require_itemtable*>(NetworkManager::getInstance()->getSendBuffer());
				packet->size = sizeof(cs_packet_require_itemtable);
				packet->type = CS_PACKET_ROOTING_TABLE;
				packet->targetId = id;
				DWORD iobyte;
				NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_require_itemtable);
				int ret = WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);

				POINT mousePos = GET_SINGLE(CInput)->GetMousePos();
				DropTableUI* pDropTableUI = GET_SINGLE(UserInterfaceManager)->getDropTableUI();
				pDropTableUI->clear();
				//pDropTableUI->
				//pDropTableUI->enableRender(true);
				CTransform* pDropTableUITr = pDropTableUI->GetTransform();
				pDropTableUITr->SetWorldPos(Vector3(mousePos.x, mousePos.y, 0));
				SAFE_RELEASE(pDropTableUITr);
			}
		}
	}
}

void Monster::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

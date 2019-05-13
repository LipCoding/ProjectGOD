#include "Mino.h"
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
#include "Core/NetworkManager.h"
#include "../UserInterfaceManager.h"
#include "../DropTableUI.h"

Mino::Mino() :
	m_pTarget(NULL),
	m_pNavigation(NULL)
{
	SetTag("Mino");
	SetTypeName("Mino");
	SetTypeID<Mino>();
}


Mino::~Mino()
{
	SAFE_RELEASE(m_pNavigation);
	SAFE_RELEASE(m_pAnimation);
	SAFE_RELEASE(m_pTarget);
}

size_t Mino::getCurrentAnimation()
{
	return currentAnimation;
}

size_t Mino::getNextAnimation()
{
	return nextAnimation;
}

void Mino::changeAnimation()
{
	//m_pAnimation->ChangeClip(stateMap[nextAnimation].first);
}

void Mino::SetTarget(const string & strTag)
{
	CGameObject*	pTargetObj = CGameObject::FindObject(strTag);

	if (pTargetObj)
	{
		m_pTarget = pTargetObj->GetTransform();
		SAFE_RELEASE(pTargetObj);
	}
}

CAnimation * Mino::GetAnimation()
{
	return m_pAnimation;
}

bool Mino::Init()
{
	m_pTransform->SetOriginAxis(AXIS_Y);

	m_pAnimation = m_pGameObject->AddComponent<CAnimation>("PlayerAnimation");

	m_pAnimation->Load("99.Dynamic_Mesh\\02.Monster\\Mino\\Mino.anm");
	m_pAnimation->SetDefaultClip("Idle");

	if (!m_pAnimation)
		m_pAnimation = FindComponentFromType<CAnimation>(CT_ANIMATION);

	if (!m_pNavigation)
		m_pNavigation = FindComponentFromType<CNavigation3D>(CT_NAVIGATION3D);

	return true;
}

void Mino::Input(float fTime)
{
}

int Mino::Update(float fTime)
{
	return 0;
}

int Mino::LateUpdate(float fTime)
{
	return 0;
}

Mino * Mino::Clone()
{
	return new Mino(*this);
}

void Mino::OnCollisionEnter(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void Mino::OnCollision(CCollider * pSrc, CCollider * pDest, float fTime)
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
		CPlayer* pPlayer = pGameObject->FindComponentFromTag<CPlayer>("Player");
		pPlayer->clickedID = id;

		CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();

		float ratio = (float)this->getCurrentHP() / (float)this->getMaxHP();
		pEnemyUIHearthBar->setLengthRatio(ratio);

		CGameObject* pUIObject = pEnemyUIHearthBar->GetGameObject();
		GET_SINGLE(UserInterfaceManager)->getEnemyStatus()->enableRender(true);
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

void Mino::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

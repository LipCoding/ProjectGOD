#include "Armored_BlueLizard.h"
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
#include "../DropItemSlot.h"
#include "../DropTableUI.h"

Armored_BlueLizard::Armored_BlueLizard() :
	m_pTarget(NULL),
	m_pNavigation(NULL)
{
	SetTag("Armored_BlueLizard");
	SetTypeName("Armored_BlueLizard");
	SetTypeID<Armored_BlueLizard>();
}


Armored_BlueLizard::~Armored_BlueLizard()
{
	SAFE_RELEASE(m_pNavigation);
	SAFE_RELEASE(m_pAnimation);
	SAFE_RELEASE(m_pTarget);
}

size_t Armored_BlueLizard::getCurrentAnimation()
{
	return currentAnimation;
}

size_t Armored_BlueLizard::getNextAnimation()
{
	return nextAnimation;
}

void Armored_BlueLizard::changeAnimation()
{
	//m_pAnimation->ChangeClip(stateMap[nextAnimation].first);
}

void Armored_BlueLizard::SetTarget(const string & strTag)
{
	CGameObject*	pTargetObj = CGameObject::FindObject(strTag);

	if (pTargetObj)
	{
		m_pTarget = pTargetObj->GetTransform();
		SAFE_RELEASE(pTargetObj);
	}
}

CAnimation * Armored_BlueLizard::GetAnimation()
{
	return m_pAnimation;
}

bool Armored_BlueLizard::Init()
{
	m_pTransform->SetOriginAxis(AXIS_Y);
	//m_pTransform->SetWorldScale(3.f, 3.f, 3.f);

	//m_pAnimation = m_pGameObject->AddComponent<CAnimation>("GolemAnimation");
	m_pAnimation = m_pGameObject->AddComponent<CAnimation>("PlayerAnimation");

	m_pAnimation->Load("99.Dynamic_Mesh\\02.Monster\\Armored_BlueLizard\\Armored_BlueLizard.anm");
	m_pAnimation->SetDefaultClip("Idle");



	if (!m_pAnimation)
		m_pAnimation = FindComponentFromType<CAnimation>(CT_ANIMATION);

	if (!m_pNavigation)
		m_pNavigation = FindComponentFromType<CNavigation3D>(CT_NAVIGATION3D);

	return true;
}

void Armored_BlueLizard::Input(float fTime)
{
}

int Armored_BlueLizard::Update(float fTime)
{
	return 0;
}

int Armored_BlueLizard::LateUpdate(float fTime)
{
	return 0;
}

Armored_BlueLizard * Armored_BlueLizard::Clone()
{
	return new Armored_BlueLizard(*this);
}

void Armored_BlueLizard::OnCollisionEnter(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void Armored_BlueLizard::OnCollision(CCollider * pSrc, CCollider * pDest, float fTime)
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
				//pDropTableUI->
				//pDropTableUI->enableRender(true);
				CTransform* pDropTableUITr = pDropTableUI->GetTransform();
				pDropTableUITr->SetWorldPos(Vector3(mousePos.x, mousePos.y, 0));
				SAFE_RELEASE(pDropTableUITr);
			}
		}
	}
}

void Armored_BlueLizard::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

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

	m_pAnimation->Load("99.Dynamic_Mesh\\02.Monster\\Mino.anm");
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
		pUIObject->Enable(true);
		SAFE_RELEASE(pUIObject);
	}
}

void Mino::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

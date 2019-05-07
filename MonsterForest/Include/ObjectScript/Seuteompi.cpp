#include "Seuteompi.h"
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

Seuteompi::Seuteompi() :
	m_pTarget(NULL),
	m_pNavigation(NULL)
{
	SetTag("Seuteompi");
	SetTypeName("Seuteompi");
	SetTypeID<Seuteompi>();
}


Seuteompi::~Seuteompi()
{
	SAFE_RELEASE(m_pNavigation);
	SAFE_RELEASE(m_pAnimation);
	SAFE_RELEASE(m_pTarget);
}

size_t Seuteompi::getCurrentAnimation()
{
	return currentAnimation;
}

size_t Seuteompi::getNextAnimation()
{
	return nextAnimation;
}

void Seuteompi::changeAnimation()
{
	//m_pAnimation->ChangeClip(stateMap[nextAnimation].first);
}

void Seuteompi::SetTarget(const string & strTag)
{
	CGameObject*	pTargetObj = CGameObject::FindObject(strTag);

	if (pTargetObj)
	{
		m_pTarget = pTargetObj->GetTransform();
		SAFE_RELEASE(pTargetObj);
	}
}

CAnimation * Seuteompi::GetAnimation()
{
	return m_pAnimation;
}

bool Seuteompi::Init()
{
	m_pTransform->SetOriginAxis(AXIS_Y);
	//m_pTransform->SetWorldScale(3.f, 3.f, 3.f);

	//m_pAnimation = m_pGameObject->AddComponent<CAnimation>("GolemAnimation");
	m_pAnimation = m_pGameObject->AddComponent<CAnimation>("PlayerAnimation");

	m_pAnimation->Load("99.Dynamic_Mesh\\02.Monster\\seuteompi.anm");
	m_pAnimation->SetDefaultClip("Idle");



	if (!m_pAnimation)
		m_pAnimation = FindComponentFromType<CAnimation>(CT_ANIMATION);

	if (!m_pNavigation)
		m_pNavigation = FindComponentFromType<CNavigation3D>(CT_NAVIGATION3D);

	return true;
}

void Seuteompi::Input(float fTime)
{
}

int Seuteompi::Update(float fTime)
{
	return 0;
}

int Seuteompi::LateUpdate(float fTime)
{
	return 0;
}

Seuteompi * Seuteompi::Clone()
{
	return new Seuteompi(*this);
}

void Seuteompi::OnCollisionEnter(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void Seuteompi::OnCollision(CCollider * pSrc, CCollider * pDest, float fTime)
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

void Seuteompi::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

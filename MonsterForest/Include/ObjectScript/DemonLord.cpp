#include "DemonLord.h"
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

DemonLord::DemonLord() :
	m_pTarget(NULL),
	m_pNavigation(NULL)
{
	SetTag("DemonLord");
	SetTypeID<DemonLord>();
}


DemonLord::~DemonLord()
{
	SAFE_RELEASE(m_pNavigation);
	SAFE_RELEASE(m_pAnimation);
	SAFE_RELEASE(m_pTarget);
}

size_t DemonLord::getCurrentAnimation()
{
	return currentAnimation;
}

size_t DemonLord::getNextAnimation()
{
	return nextAnimation;
}

void DemonLord::settingStatus(int current_hp, int current_mp, int level, int exp)
{
	currentHP = current_hp;
	currentMP = current_mp;
	this->level = level;
	this->maxHP = 1200;
	this->maxMP = 300;
	this->attackDamage = 25;
}

void DemonLord::changeAttackAnimation()
{
	CAnimation* pAnimation = m_pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
	pAnimation->ChangeClip("Attack1");
	SAFE_RELEASE(pAnimation);
}

void DemonLord::SetTarget(const string & strTag)
{
	CGameObject*	pTargetObj = CGameObject::FindObject(strTag);

	if (pTargetObj)
	{
		m_pTarget = pTargetObj->GetTransform();
		SAFE_RELEASE(pTargetObj);
	}
}

CAnimation * DemonLord::GetAnimation()
{
	return m_pAnimation;
}

bool DemonLord::Init()
{
	m_pTransform->SetOriginAxis(AXIS_Y);
	//m_pTransform->SetWorldScale(3.f, 3.f, 3.f);

	//m_pAnimation = m_pGameObject->AddComponent<CAnimation>("GolemAnimation");
	m_pAnimation = m_pGameObject->AddComponent<CAnimation>("PlayerAnimation");

	m_pAnimation->Load("99.Dynamic_Mesh\\02.Monster\\DemonLord\\DemonLord.anm");
	m_pAnimation->SetDefaultClip("Idle1");



	if (!m_pAnimation)
		m_pAnimation = FindComponentFromType<CAnimation>(CT_ANIMATION);

	if (!m_pNavigation)
		m_pNavigation = FindComponentFromType<CNavigation3D>(CT_NAVIGATION3D);

	return true;
}

void DemonLord::Input(float fTime)
{
}

int DemonLord::Update(float fTime)
{
	return 0;
}

int DemonLord::LateUpdate(float fTime)
{
	return 0;
}

DemonLord * DemonLord::Clone()
{
	return new DemonLord(*this);
}
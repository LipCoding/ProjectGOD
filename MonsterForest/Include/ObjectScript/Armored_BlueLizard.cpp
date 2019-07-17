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
#include "../NetworkManager.h"
#include "../UserInterfaceManager.h"
#include "../DropItemSlot.h"
#include "../DropTableUI.h"

Armored_BlueLizard::Armored_BlueLizard() :
	m_pTarget(NULL),
	m_pNavigation(NULL)
{
	SetTag("Armored_BlueLizard");
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

void Armored_BlueLizard::settingStatus(int current_hp, int current_mp, int level, int exp)
{
	currentHP = current_hp;
	currentMP = current_mp;
	this->level = level;
	this->maxHP = 50 + level * 10;
	this->maxMP = 10 + level * 2;
	this->attackDamage = 10 + level * 2;
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

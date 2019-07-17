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
#include "../NetworkManager.h"
#include "../UserInterfaceManager.h"
#include "../DropTableUI.h"

Mino::Mino() :
	m_pTarget(NULL),
	m_pNavigation(NULL)
{
	SetTag("Mino");
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

void Mino::settingStatus(int current_hp, int current_mp, int level, int exp)
{
	currentHP = current_hp;
	currentMP = current_mp;
	this->level = level;
	this->maxHP = 50 + level * 5;
	this->maxMP = 10 + level * 2;
	this->attackDamage = 5 + level * 1;
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
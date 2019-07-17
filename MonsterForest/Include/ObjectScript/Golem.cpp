#include "Golem.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Component/Collider.h"
#include "Component/Camera.h"
#include "Device.h"
#include "Core/Input.h"
#include "GolemAttack.h"
#include "GolemIdle.h"
#include "GolemMove.h"
#include "GolemDie.h"
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
Golem::Golem() :
	m_pTarget(NULL),
	m_pNavigation(NULL)
{
	SetTag("Golem");
	SetTypeID<Golem>();
}

Golem::Golem(const Golem & goblin)
{
	m_pTarget = NULL;
	m_pAnimation = NULL;
	m_pNavigation = NULL;

	GolemAttack* pAttackState = new GolemAttack;
	GolemIdle* pIdleState = new GolemIdle;
	GolemMove* pMoveState = new GolemMove;
	GolemDie* pDieState = new GolemDie;

	stateMap.insert(make_pair(GOLEM_STATE::GOLEM_STATE_ATTACK, GOLEMSTATE{ "Attack_J",pAttackState }));
	stateMap.insert(make_pair(GOLEM_STATE::GOLEM_STATE_IDLE, GOLEMSTATE{ "Idle2", pIdleState }));
	stateMap.insert(make_pair(GOLEM_STATE::GOLEM_STATE_MOVE, GOLEMSTATE{ "Walk", pMoveState }));
	stateMap.insert(make_pair(GOLEM_STATE::GOLEM_STATE_DIE, GOLEMSTATE{ "Death", pDieState }));

	pState = getState(GOLEM_STATE::GOLEM_STATE_IDLE);
	currentAnimation = GOLEM_STATE::GOLEM_STATE_IDLE;
	nextAnimation = GOLEM_STATE_NONE;
}

Golem::~Golem()
{
	SAFE_RELEASE(m_pNavigation);
	SAFE_RELEASE(m_pAnimation);
	SAFE_RELEASE(m_pTarget);
}

GolemState * Golem::getCurrentState()
{
	return this->pState;
}

GolemState * Golem::getState(int stateID)
{
	const auto& finder = stateMap.find(stateID);

	if (finder == stateMap.end())
		return nullptr;

	return finder->second.second;
}

size_t Golem::getCurrentAnimation()
{
	return currentAnimation;
}

size_t Golem::getNextAnimation()
{
	return nextAnimation;
}

void Golem::changeAnimation()
{
	m_pAnimation->ChangeClip(stateMap[nextAnimation].first);
}

void Golem::settingStatus(int current_hp, int current_mp, int level, int exp)
{
	currentHP = current_hp;
	currentMP = current_mp;
	this->level = level;
	this->maxHP = 30 + level * 10;
	this->maxMP = 10 + level * 2;
	this->attackDamage = 3 + level * 1;
}

void Golem::setCurrentAnimation(size_t currentAnimation)
{
	this->currentAnimation = currentAnimation;
}

void Golem::setNextAnimation(size_t nextAnimation)
{
	this->nextAnimation = nextAnimation;
}

void Golem::setCurrentState(GolemState* pState)
{
	this->pState = pState;
}

void Golem::SetTarget(const string & strTag)
{
	CGameObject*	pTargetObj = CGameObject::FindObject(strTag);

	if (pTargetObj)
	{
		m_pTarget = pTargetObj->GetTransform();
		SAFE_RELEASE(pTargetObj);
	}
}

CAnimation * Golem::GetAnimation()
{
	return m_pAnimation;
}

bool Golem::Init()
{
	m_pTransform->SetOriginAxis(AXIS_Y);
	//m_pTransform->SetWorldScale(3.f, 3.f, 3.f);

	//m_pAnimation = m_pGameObject->AddComponent<CAnimation>("GolemAnimation");
	m_pAnimation = m_pGameObject->AddComponent<CAnimation>("PlayerAnimation");

	m_pAnimation->Load("99.Dynamic_Mesh\\02.Monster\\Golem\\golem.anm");
	m_pAnimation->SetDefaultClip("Idle1");

	GolemAttack* pAttackState = new GolemAttack;
	GolemIdle* pIdleState = new GolemIdle;
	GolemMove* pMoveState = new GolemMove;
	GolemDie* pDieState = new GolemDie;

	stateMap.insert(make_pair(GOLEM_STATE::GOLEM_STATE_ATTACK, GOLEMSTATE{ "Attack",pAttackState }));
	stateMap.insert(make_pair(GOLEM_STATE::GOLEM_STATE_IDLE, GOLEMSTATE{ "Idle1", pIdleState }));
	stateMap.insert(make_pair(GOLEM_STATE::GOLEM_STATE_MOVE, GOLEMSTATE{ "Walk", pMoveState }));
	stateMap.insert(make_pair(GOLEM_STATE::GOLEM_STATE_DIE, GOLEMSTATE{ "Death", pDieState }));

	//Idle2
	//Death
	//Attack
	//Idle1
	//Attack_J
	//Walk

	pState = getState(GOLEM_STATE::GOLEM_STATE_IDLE);
	currentAnimation = GOLEM_STATE::GOLEM_STATE_IDLE;
	nextAnimation = GOLEM_STATE_NONE;
	return true;
}

void Golem::Input(float fTime)
{
	if (!m_pAnimation)
		m_pAnimation = FindComponentFromType<CAnimation>(CT_ANIMATION);

	if (!m_pNavigation)
		m_pNavigation = FindComponentFromType<CNavigation3D>(CT_NAVIGATION3D);
}

int Golem::Update(float fTime)
{
	if (nullptr != pState)
		pState->execute(m_pGameObject);

	return 0;
}

int Golem::LateUpdate(float fTime)
{
	return 0;
}

Golem * Golem::Clone()
{
	return new Golem(*this);
}
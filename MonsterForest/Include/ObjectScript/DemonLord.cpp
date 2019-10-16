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
#include "Core/SoundManager.h"
#include "Core/EffectManager.h"
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

void DemonLord::attack_animation(const string& target_tag, const wstring & name)
{
	CAnimation* pAnimation = FindComponentFromType<CAnimation>(CT_ANIMATION);
	string animation_name = strconv(name);
	pAnimation->ChangeClip(animation_name);
	SAFE_RELEASE(pAnimation);

	CGameObject* target_object = CGameObject::FindObject(target_tag);
	if (nullptr == target_object)
		return;

	if (animation_name == string{ "Attack1" })
	{
		GET_SINGLE(SoundManager)->Play("Boss_Att_Sound", SC_MONSTER);
		GET_SINGLE(CEffectManager)->OperateEffect("BossAttack1", m_pGameObject, Vector3(0.f, 0.f, 0.f),false, true);
		GET_SINGLE(SoundManager)->PlayWithDelay("Boss_Attack1", SC_EFFECT, 0.8f);
	}
	else if (animation_name == string{ "Attack2" })
	{
		GET_SINGLE(SoundManager)->Play("Boss_Att_Sound", SC_MONSTER);
		GET_SINGLE(CEffectManager)->OperateEffect("BossAttack2", m_pGameObject, Vector3(0.f, 0.f, 0.f),false, true);
		GET_SINGLE(SoundManager)->PlayWithDelay("Boss_Attack2", SC_EFFECT, 1.5f);
	}																							 
	else if (animation_name == string{ "Attack3" })												 
	{							
		GET_SINGLE(SoundManager)->Play("Boss_Att_Sound", SC_MONSTER);
		GET_SINGLE(CEffectManager)->OperateEffect("BossAttack3", m_pGameObject, Vector3(0.f, 0.f, 0.f),false, true);
		GET_SINGLE(SoundManager)->PlayWithDelay("Boss_Attack3", SC_EFFECT, 1.6f);
	}																							 
	else if (animation_name == string{ "Attack4" })												 
	{					
		GET_SINGLE(SoundManager)->Play("Boss_Att_Sound", SC_MONSTER);
		GET_SINGLE(CEffectManager)->OperateEffect("BossAttack4", m_pGameObject, Vector3(0.f, 0.f, 0.f),false, true);
		GET_SINGLE(SoundManager)->PlayWithDelay("Boss_Attack4", SC_EFFECT, 0.f);
		GET_SINGLE(SoundManager)->PlayWithDelay("Boss_Attack4", SC_EFFECT, 0.5f);
		GET_SINGLE(SoundManager)->PlayWithDelay("Boss_Attack4", SC_EFFECT, 1.f);
		GET_SINGLE(SoundManager)->PlayWithDelay("Boss_Attack4", SC_EFFECT, 1.5f);
	}																							  
	else if (animation_name == string{ "Spell1" })												  
	{							
		GET_SINGLE(SoundManager)->Play("Boss_Spell_Sound", SC_MONSTER);
		GET_SINGLE(CEffectManager)->OperateEffect("BossSpell1", m_pGameObject, Vector3(0.f, 0.f, 0.f)),false, true;
		GET_SINGLE(SoundManager)->PlayWithDelay("Boss_Spell1", SC_EFFECT, 1.f);
	}																							  
	else if (animation_name == string{ "Spell2" })												  
	{											
		GET_SINGLE(SoundManager)->Play("Boss_Spell_Sound", SC_MONSTER);
		GET_SINGLE(CEffectManager)->OperateEffect("BossSpell2", m_pGameObject, Vector3(0.f, 0.f, 0.f)),false, true;
		GET_SINGLE(SoundManager)->Play("Boss_Spell2", SC_EFFECT);
	}

	Actor* target_actor_component = target_object->FindComponentFromTypeName<Actor>("Actor");
	target_actor_component->damaged(attackDamage);
	//GET_SINGLE(SoundManager)->Play("monster_attack", SC_EFFECT);


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
	
	//Vector3 effect_pos = m_p 
	


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
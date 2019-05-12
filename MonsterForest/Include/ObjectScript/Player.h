#pragma once

#include "Component/Script.h"
#include "Component/Animation2D.h"
#include "Component/Navigation.h"
#include "GameObject/GameObject.h"
#include "Component/Animation.h"
#include "Component/Navigation3D.h"
#include "Component/Arm.h"

PG_USING

class CPlayer :
	public CScript
{
	wstring characterName;
	int Level = 1;

	int currentHP = 100;
	int currentMP = 100;

	int maxHP = 100;
	int maxMP = 100;

	int attackDamage = 30;
	int EXP = 0;
	int maxEXP = 100;
public:
	CPlayer();
	CPlayer(const CPlayer& player);
	~CPlayer();

private:
	float	m_fMoveSpeed;
	float	m_fRotSpeed;
	float   m_fRecentRot;
	bool	m_bOnShield;
	bool	m_bOnShieldRange;
	CGameObject*	m_pShield[3];
	CAnimation*		m_pAnimation;
	CNavigation3D*	m_pNavigation;
	CArm*			m_pArm;
public:
	int clickedID = -1;
	void setAnimation(CAnimation* pAnimation) { this->m_pAnimation = pAnimation; }

	void hpRegeneration(int hpregenration) { this->currentHP += hpregenration; }

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual CPlayer* Clone();
	void addExp(int exp);
public:
	int getLevel() { return Level; }
	int getCurrentHP() { return currentHP; }
	int getCurrentMP() { return currentMP; }
	int getMaxHP() { return maxHP; }
	int getMaxMP() { return maxMP; }
	int getAttackDamage() { return attackDamage; }
	int getEXP() { return EXP; }
	int getMaxEXP() { return maxEXP; }
public:
	void setEXP(int exp) { this->EXP = exp; }
	void setMaxEXP(int maxEXP) { this->maxEXP = maxEXP; }
	void setLevel(int level) { this->Level = level; }
	void setCharacterName(const wstring& characterName) { this->characterName = characterName; }
	void setCurrentHP(int currentHP) { this->currentHP = currentHP; }
	void setCurrentMP(int currentMP) { this->currentMP = currentMP; }
	void setMaxHP(int maxHP) { this->maxHP = maxHP; }
	void setMaxMP(int maxMP) { this->maxMP = maxMP; }
	void setAttackDamag(int attackDamage) { this->attackDamage = attackDamage; }

	virtual void OnCollisionEnter(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollision(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollisionLeave(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
};


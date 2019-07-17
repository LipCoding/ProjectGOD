#pragma once

#include "Component/Script.h"
#include "Component/Animation2D.h"
#include "Component/Navigation.h"
#include "GameObject/GameObject.h"
#include "Component/Animation.h"
#include "Component/Navigation3D.h"
#include "Component/Arm.h"
#include "Actor.h"

PG_USING

class CPlayer :
	public Actor
{

public:
	CPlayer();
	CPlayer(const CPlayer& player);
	~CPlayer();
private:

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
	virtual void damaged(int damage);
	virtual void attack(const string& target_tag);
	void skill1_Attack(const string& target_tag);
	void skill2_Attack(const string& target_tag);
	void skill3_Attack(const string& target_tag);
	void skill4_Buff();
	virtual void settingStatus(int current_hp, int current_mp, int level, int exp);
	virtual void worldscale(float x, float y, float z) ;
	virtual void rotate(float x, float y, float z);
	virtual void move(float x, float y, float z, bool isBackMove = false);
	virtual void changeAnimation(const string& clip_name);

	virtual void OnCollisionEnter(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollision(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollisionLeave(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
};


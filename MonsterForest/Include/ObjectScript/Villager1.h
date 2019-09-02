#pragma once

#include "Monster.h"

class Villager1 :
	public Monster
{
	CAnimation*		m_pAnimation;
public:
	Villager1();
	~Villager1();

public:
	virtual void settingStatus(int current_hp, int current_mp, int level, int exp);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual Villager1* Clone();

public:
	virtual void OnCollisionEnter(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollision(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollisionLeave(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
};


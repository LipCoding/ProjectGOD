#pragma once

#include "Component/Script.h"

PG_USING

class CBullet	:
	public CScript
{
public:
	CBullet();
	CBullet(const CBullet& bullet);
	~CBullet();

private:
	float	m_fSpeed;
	float	m_fDist;
	CGameObject*	m_pTarget;

public:
	void SetTarget(const string& strTag);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual CBullet* Clone();
};


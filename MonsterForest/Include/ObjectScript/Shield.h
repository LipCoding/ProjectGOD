#pragma once

#include "Component/Script.h"
#include "Component/Arm.h"

PG_USING

class CShield :
	public CScript
{
public:
	CShield();
	CShield(const CShield& shield);
	~CShield();

private:
	float	m_fSpeed;
	CArm*	m_pArm;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual CShield* Clone();
};


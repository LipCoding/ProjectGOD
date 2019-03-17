#pragma once

#include "Component/Script.h"
#include "Component/Animation2D.h"
#include "Component/Navigation.h"
#include "GameObject/GameObject.h"
#include "Component/Animation.h"
#include "Component/Navigation3D.h"
#include "Component/Arm.h"

PG_USING

class CPlayer	:
	public CScript
{
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
	void setAnimation(CAnimation* pAnimation) { this->m_pAnimation = pAnimation; }

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual CPlayer* Clone();


};


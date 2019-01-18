#include "Shield.h"
#include "Component/Transform.h"
#include "GameObject/GameObject.h"

CShield::CShield()
{
	SetTag("Shield");
	SetTypeName("CShield");
	SetTypeID<CShield>();
	m_pArm = NULL;
}

CShield::CShield(const CShield & shield)	:
	CScript(shield)
{
	m_fSpeed = shield.m_fSpeed;
	m_pArm = NULL;
}

CShield::~CShield()
{
	SAFE_RELEASE(m_pArm);
}

bool CShield::Init()
{
	m_fSpeed = PG_PI / 9.f;

	return true;
}

void CShield::Input(float fTime)
{
}

int CShield::Update(float fTime)
{
	if (!m_pArm)
		m_pArm = FindComponentFromType<CArm>(CT_ARM);

	m_pArm->RotationY(m_fSpeed * fTime);

	return 0;
}

int CShield::LateUpdate(float fTime)
{
	return 0;
}

CShield * CShield::Clone()
{
	return new CShield(*this);
}

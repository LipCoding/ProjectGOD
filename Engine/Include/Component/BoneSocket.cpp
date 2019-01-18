#include "BoneSocket.h"
#include "../GameObject/GameObject.h"
#include "Transform.h"

PG_USING

CBoneSocket::CBoneSocket()	:
	m_pTarget(NULL),
	m_pTargetTr(NULL)
{
}

CBoneSocket::CBoneSocket(const CBoneSocket & socket)
{
	*this = socket;

	m_pTarget = NULL;
	m_pTargetTr = NULL;
}

CBoneSocket::~CBoneSocket()
{
	SAFE_RELEASE(m_pTargetTr);
	SAFE_RELEASE(m_pTarget);
}

void CBoneSocket::SetTarget(CGameObject * pTarget)
{
	if (pTarget)
	{
		SAFE_RELEASE(m_pTarget);
		SAFE_RELEASE(m_pTargetTr);
		pTarget->AddRef();
		m_pTarget = pTarget;

		m_pTargetTr = pTarget->GetTransform();
	}

	else
	{
		SAFE_RELEASE(m_pTargetTr);
		SAFE_RELEASE(m_pTarget);
	}
}

void CBoneSocket::Update(const Matrix & matWorld, const Matrix & matBone)
{
	Matrix localX = XMMatrixIdentity();
	Matrix localY = XMMatrixIdentity();
	Matrix localZ = XMMatrixIdentity();
	Matrix localRotation = XMMatrixIdentity();
	//	pSwordTr->SetWorldPos(0.24f, 0.1f, -0.3f);
	//local.m[3][0] = 0.24f;
	//local.m[3][1] = -0.1f;
	//local.m[3][2] = -0.3f;
	localX = XMMatrixRotationX(90.f);
	//localY = XMMatrixRotationY(90.f);
	localZ = XMMatrixRotationZ(90.f);

	localRotation = localX * localY * localZ;

	Matrix tempMatrix = matWorld;
	tempMatrix.m[3][0] += 0.05f;
	tempMatrix.m[3][1] += 1.5f;
	tempMatrix.m[3][2] += -0.03f;

	m_pTargetTr->SetParentMatrix(localRotation * matBone * tempMatrix);
}

CBoneSocket * CBoneSocket::Clone() const
{
	return new CBoneSocket(*this);
}

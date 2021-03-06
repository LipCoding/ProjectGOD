#include "Arm.h"
#include "Transform.h"
#include "../GameObject/GameObject.h"

PG_USING

CArm::CArm()	:
	m_pTarget(NULL)
{
	SetTag("Arm");
	SetTypeName("CArm");
	SetTypeID<CArm>();
	m_eType = CT_ARM;
}

CArm::CArm(const CArm & arm)	:
	CComponent(arm)
{
	m_pTarget = NULL;
}

CArm::~CArm()
{
	SAFE_RELEASE(m_pTarget);
}

void CArm::SetTarget(CTransform * pTarget)
{
	SAFE_RELEASE(m_pTarget);
	pTarget->AddRef();
	m_pTarget = pTarget;
}

void CArm::SetTarget(CGameObject * pTarget)
{
	SAFE_RELEASE(m_pTarget);
	m_pTarget = pTarget->GetTransform();
}

void CArm::SetDist(const Vector3 & vDist)
{
	m_vDist = vDist;
}

void CArm::SetLookAtDist(const Vector3 & vLookAtDist)
{
	m_vLookAtDist = vLookAtDist;
}

void CArm::SetMinDist(float fMin)
{
	m_fMinDist = fMin;
}

void CArm::SetMaxDist(float fMax)
{
	m_fMaxDist = fMax;
}

void CArm::Zoom(const Vector3 & vDir, float fSpeed)
{
	m_vDist += vDir * fSpeed;
	float	fDist = m_vDist.Length();

	if (fDist < m_fMinDist)
	{
		fDist -= m_fMinDist;

		m_vDist += vDir * fDist;
	}

	else if (fDist > m_fMaxDist)
	{
		fDist = m_fMaxDist - fDist;

		m_vDist += vDir * fDist;
	}
}

void CArm::RotationY(float fAngle)
{
	// 인자로 들어온 각도만큼 Y축 회전을 돌릴 행렬을 만든다.
	Matrix	matRot = XMMatrixRotationY(fAngle);

	// 위에서 만든 행렬을 이용해서 카메라를 구성하는 3개의 축을 회전시킨다.
	Vector3	vAxis[AXIS_END];
	for (int i = 0; i < AXIS_END; ++i)
	{
		vAxis[i] = m_pTransform->GetWorldAxis((AXIS)i);
		vAxis[i] = vAxis[i].TransformNormal(matRot.mat);
		m_pTransform->SetWorldAxis(vAxis[i].Normalize(), (AXIS)i);
	}

	// 위에서 만들어준 회전된 축을 이용해서 회전행렬을 구성해준다.
	m_pTransform->ComputeWorldRotationMatrix(vAxis);
		
	// 회전행렬의 위치 부분을 타겟의 위치로 설정한다.
	if (m_pTarget)
	{
		memcpy(&matRot.m[3][0], &m_pTarget->GetWorldPos(), sizeof(Vector3));
	}


	Vector3	vPos = m_vDist.TransformCoord(matRot.mat);

	m_vDist = vPos - m_pTarget->GetWorldPos();

	
}

void CArm::RotationAxis(const Vector3 & vRotAxis, float fAngle)
{
	Matrix	matRot = XMMatrixRotationAxis(vRotAxis.Convert(), fAngle);

	// 각각의 축을 위에서 만들어준 회전행렬을 이용해서 회전시켜준다.
	Vector3	vAxis[AXIS_END];
	for (int i = 0; i < AXIS_END; ++i)
	{
		vAxis[i] = m_pTransform->GetWorldAxis((AXIS)i);
		vAxis[i] = vAxis[i].TransformNormal(matRot.mat);
		m_pTransform->SetWorldAxis(vAxis[i].Normalize(), (AXIS)i);
	}

	// 회전된 축을 이용해서 회전행렬을 구해준다.
	m_pTransform->ComputeWorldRotationMatrix(vAxis);

	// 회전행렬의 위치 부분을 타겟의 위치로 설정한다.
	if (m_pTarget)
	{
		memcpy(&matRot.m[3][0], &m_pTarget->GetWorldPos(), sizeof(Vector3));
	}

	Vector3	vPos = m_vDist.TransformCoord(matRot.mat);

	m_vDist = vPos - m_pTarget->GetWorldPos();
}

bool CArm::Init()
{
	m_vDist = Vector3(0.f, 1.f, -7.f);
	m_vLookAtDist = Vector3::Zero;
	m_fMinDist = 1.f;
	m_fMaxDist = 50.f;

	return true;
}

void CArm::Input(float fTime)
{
}

int CArm::Update(float fTime)
{
	if (m_pTarget)
	{
		Vector3	vPos = m_pTarget->GetWorldPos() + m_vLookAtDist + m_vDist;

		m_pTransform->SetWorldPos(vPos);
	}

	return 0;
}

int CArm::LateUpdate(float fTime)
{
	return 0;
}

void CArm::Collision(float fTime)
{
}

void CArm::Render(float fTime)
{
}

CArm * CArm::Clone()
{
	return new CArm(*this);
}

void CArm::Save(FILE * pFile)
{
}

void CArm::Load(FILE * pFile)
{
}

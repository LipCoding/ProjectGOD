#include "Camera.h"
#include "Transform.h"
#include "../GameObject/GameObject.h"
#include "../Device.h"
#include "../Scene/Scene.h"
#include "Frustum.h"

PG_USING

CCamera::CCamera() :
	m_matView(NULL),
	m_matProj(NULL),
	m_matShadowView(NULL),
	m_matShadowProj(NULL),
	m_pAttach(NULL),
	m_pFrustum(NULL)
{
	SetTag("Camera");
	SetTypeName("CCamera");
	SetTypeID<CCamera>();
	m_eType = CT_CAMERA;
	m_tWorldRS.iWidth = UINT_MAX;
	m_tWorldRS.iHeight = UINT_MAX;
	m_vPivot.x = 0.5f;
	m_vPivot.y = 0.5f;
}

CCamera::CCamera(const CCamera & camera)	:
	CComponent(camera)
{
	m_matView = new Matrix;
	m_matProj = new Matrix;
	m_matShadowView = new Matrix;
	m_matShadowProj = new Matrix;

	*m_matView = *camera.m_matView;
	*m_matProj = *camera.m_matProj;

	*m_matShadowView = *camera.m_matShadowView;
	*m_matShadowProj = *camera.m_matShadowProj;

	m_tWorldRS = camera.m_tWorldRS;
	m_vPivot = camera.m_vPivot;

	m_pAttach = NULL;
}

CCamera::~CCamera()
{
	SAFE_DELETE(m_pFrustum);
	SAFE_RELEASE(m_pAttach);
	SAFE_DELETE(m_matView);
	SAFE_DELETE(m_matProj);
	SAFE_DELETE(m_matShadowView);
	SAFE_DELETE(m_matShadowProj);
}

void CCamera::SetPivot(float x, float y)
{
	m_vPivot = Vector2(x, y);
}

void CCamera::SetPivot(const Vector2 & vPivot)
{
	m_vPivot = vPivot;
}

void CCamera::SetWorldResolution(UINT x, UINT y)
{
	m_tWorldRS.iWidth = x;
	m_tWorldRS.iHeight = y;
}

void CCamera::SetWorldResolution(const RESOLUTION & tRS)
{
	m_tWorldRS = tRS;
}

void CCamera::SetAttach(CGameObject * pAttach)
{
	SAFE_RELEASE(m_pAttach);
	if (pAttach)
	{
		m_pAttach = pAttach->GetTransform();
		m_vPrevPos = m_pAttach->GetWorldPos();
	}
}

void CCamera::SetAttach(CComponent * pAttach)
{
	SAFE_RELEASE(m_pAttach);
	if (pAttach)
	{
		m_pAttach = pAttach->GetTransform();
		m_vPrevPos = m_pAttach->GetWorldPos();
	}
}

bool CCamera::FrustumInPoint(const Vector3 & vPos)
{
	return m_pFrustum->FrustumInPoint(vPos);
}

bool CCamera::FrustumInSphere(const Vector3 & vCenter, float fRadius)
{
	return m_pFrustum->FrustumInSphere(vCenter, fRadius);
}

bool CCamera::FrustumInSphere(const SPHERE & tSphere)
{
	return m_pFrustum->FrustumInSphere(tSphere);
}

Matrix CCamera::GetViewMatrix() const
{
	return *m_matView;
}

Matrix CCamera::GetProjMatrix() const
{
	return *m_matProj;
}

Matrix CCamera::GetShadowViewMatrix() const
{
	return *m_matShadowView;
}

Matrix CCamera::GetShadowProjMatrix() const
{
	return *m_matShadowProj;
}

XMMATRIX CCamera::GetLightView()
{
	Vector3 center{ 512.f / 2.f, 0.f, 512.f / 2.f };
	Vector3 lightCamPos = m_pTransform->GetWorldPos();
	XMVECTOR lightPos = XMLoadFloat3(&lightCamPos);
	XMVECTOR targetPos = XMLoadFloat3(&center);
	XMVECTOR up = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	m_xmatLightView = XMMatrixLookAtLH(lightPos, targetPos, up);

	return m_xmatLightView;
}

XMMATRIX CCamera::GetLightProj()
{
	Vector3 center{ 512.f / 2.f, 0.f, 512.f / 2.f };
	XMVECTOR targetPos = XMLoadFloat3(&center);

	XMFLOAT3 sphereCenterLS;
	XMStoreFloat3(&sphereCenterLS, XMVector3TransformCoord(targetPos, m_xmatLightView));

	//old
	//float radius = 325.0f;
	float radius = 425.f;

	float l = sphereCenterLS.x - radius;
	float r = sphereCenterLS.x + radius;
	float t = sphereCenterLS.y + radius;
	float f = sphereCenterLS.z + radius;
	float b = sphereCenterLS.y - radius;
	float n = sphereCenterLS.z - radius;

	m_xmatLightProj = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);

	return m_xmatLightProj;
}

void CCamera::SetOrthoProj(const RESOLUTION & tRS, 
	float fNear, float fFar)
{
	/*
	2/(r-l)      0            0           0
	0            2/(t-b)      0           0
	0            0            1/(zf-zn)   0
	(l+r)/(l-r)  (t+b)/(b-t)  zn/(zn-zf)  l
	*/
	*m_matProj = XMMatrixOrthographicOffCenterLH(0.f, tRS.iWidth,
		tRS.iHeight, 0.f, fNear, fFar);

	m_eCameraType = CT_ORTHO;
}

void CCamera::SetPerspectiveProj(float fViewAngle, float fAspect, float fNear, float fFar)
{
	*m_matProj = XMMatrixPerspectiveFovLH(fViewAngle,
		fAspect, fNear, fFar);

	m_eCameraType = CT_PERSPECTIVE;
}

void CCamera::SetShadowOrthoProj(const RESOLUTION & tRS, float fNear, float fFar)
{
	*m_matShadowProj = XMMatrixOrthographicOffCenterLH(0.f, tRS.iWidth,
		tRS.iHeight, 0.f, fNear, fFar);
}

void CCamera::SetShadowPerspectiveProj(float fViewAngle, float fAspect, float fNear, float fFar)
{
	// 시야 기반 왼손 투시 투영 행렬 작성
	*m_matShadowProj = XMMatrixPerspectiveFovLH(fViewAngle,
		fAspect, fNear, fFar);
}

void CCamera::ComputeOrthoView()
{
	*m_matView = XMMatrixIdentity();

	if (!m_pGameObject->CheckComponentFromType(CT_ARM))
	{
		if (m_tWorldRS.iWidth != UINT_MAX && m_tWorldRS.iHeight != UINT_MAX)
		{
			RESOLUTION	tRS = DEVICE_RESOLUTION;
			Vector3 vAttachPos;
			if (m_pAttach)
				vAttachPos = m_pAttach->GetWorldPos();
			Vector3	vPos = m_pTransform->GetWorldPos();

			// 타겟이 이동할 수 있는 범위를 만들어준다.
			float	fLeft = m_vPivot.x * tRS.iWidth;
			float	fTop = m_vPivot.y * tRS.iHeight;
			float	fRight = m_tWorldRS.iWidth - tRS.iWidth * (1.f - m_vPivot.x);
			float	fBottom = m_tWorldRS.iHeight - tRS.iHeight * (1.f - m_vPivot.y);

			// 좌우로 움직일 수 있는 영역 안에 타겟이 있을 경우
			// 움직일 수 있도록 해준다.
			if (vAttachPos.x <= fLeft)
				vPos.x = 0.f;

			else if (vAttachPos.x >= fRight)
				vPos.x = m_tWorldRS.iWidth - tRS.iWidth;

			else
				vPos.x = vAttachPos.x - fLeft;

			if (vAttachPos.y <= fTop)
				vPos.y = 0.f;

			else if (vAttachPos.y >= fBottom)
				vPos.y = m_tWorldRS.iHeight - tRS.iHeight;

			else
				vPos.y = vAttachPos.y - fTop;

			m_pTransform->SetWorldPos(vPos);
		}
	}

	Vector3 vPos = m_pTransform->GetWorldPos();
	m_matView->m[3][0] = -vPos.x;
	m_matView->m[3][1] = -vPos.y;
	m_matView->m[3][2] = -vPos.z;
}

void CCamera::ComputePerspectiveView()
{
	*m_matView = XMMatrixIdentity();
	/*
	뷰 공간에서 카메라의 X축 : 1, 0, 0
	뷰 공간에서 카메라의 Y축 : 0, 1, 0
	뷰 공간에서 카메라의 Z축 : 0, 0, 1

	월드에서 카메라X축 * 뷰행렬 = 1, 0, 0
	월드에서 카메라Y축 * 뷰행렬 = 0, 1, 0
	월드에서 카메라Z축 * 뷰행렬 = 0, 0, 1
	Xx, Xy, Xz   11 12 13   1, 0, 0
	Yx, Yy, Yz * 21 22 23 = 0, 1, 0
	Zx, Zy, Zz   31 32 33   0, 0, 1

	P : 카메라 위치 . : 내적
	1   0   0   0   Xx Yx Zx 0   Xx   Yx   Zx   0
	0   1   0   0 * Xy Yy Zy 0 = Xy   Yy   Zy   0
	0   0   1   0   Xz Yz Zz 0   Xz   Yz   Zz   0
	-px -py -pz 1   0  0  0  1   -P.X -P.Y -P.Z 1
	*/
	// 카메라 Transform에서 3개의 축을 얻어온다.
	Vector3	vAxis[AXIS_END] = {};
	/*
	Xx Xy Xz 0
	Yx Yy Yz 0
	Zx Zy Zz 0
	0  0  0  1
	*/

	for (int i = 0; i < AXIS_END; ++i)
	{
		vAxis[i] = m_pTransform->GetWorldAxis((AXIS)i);
		memcpy(&m_matView->m[i][0], &vAxis[i], sizeof(Vector3));
	}

	*m_matView = m_matView->Transpose();

	// 위치를 얻어온다.
	Vector3	vPos = m_pTransform->GetWorldPos();

	if (m_pAttach && !m_pGameObject->CheckComponentFromType(CT_ARM))
	{
	}

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_matView->m[3][i] = -vPos.Dot(vAxis[i]);
	}
}

bool CCamera::Init()
{
	m_matView = new Matrix;
	m_matProj = new Matrix;

	*m_matView = XMMatrixIdentity();
	*m_matProj = XMMatrixIdentity();

	m_matShadowView = new Matrix;
	m_matShadowProj = new Matrix;

	*m_matShadowView = XMMatrixIdentity();
	*m_matShadowProj = XMMatrixIdentity();

	m_pFrustum = new CFrustum;

	return true;
}

void CCamera::Input(float fTime)
{
}

int CCamera::Update(float fTime)
{
	return 0;
}

int CCamera::LateUpdate(float fTime)
{
	*m_matView = XMMatrixIdentity();

	switch (m_eCameraType)
	{
	case CT_ORTHO:
		ComputeOrthoView();
		break;
	case CT_PERSPECTIVE:
		ComputePerspectiveView();
		break;
	}

	Matrix	matInvVP = *m_matView * *m_matProj;
	matInvVP	= XMMatrixInverse(&XMMatrixDeterminant(matInvVP.mat),
		matInvVP.mat);

	m_pFrustum->UpdateFrustum(matInvVP);

	// 그림자맵 전용 뷰 행렬 생성
	// 전역 조명을 얻어온다.
	//CTransform*	pLightTr = m_pScene->GetGlobalLightTransform();

	//// 카메라 타겟을 기준으로 전역조명과의 방향을 구한다.
	////if (m_pAttach)
	//{
	//	Vector3	vLookAt = Vector3(0.f, 0.f, 0.f);

	//	if (m_pAttach)
	//		vLookAt = m_pAttach->GetWorldPos();

	//	Vector3	vDir = Vector3(-1.f, 1.f, 0.f);
	//	vDir = vDir.Normalize();

	//	Vector3	vLightPos = vLookAt + vDir * 20.f;
	//	//vLightPos = Vector3(3.f, 5.f, 0.f);

	//	// 조명의 z축을 구한다.
	//	vDir = vDir * -1.f;

	//	// 조명의 x축을 구한다.
	//	Vector3	vUp = Vector3(0.f, 1.f, 0.f);

	//	Vector3	vRight = vUp.Cross(vDir);
	//	vRight = vRight.Normalize();

	//	vUp = vDir.Cross(vRight);
	//	vUp = vUp.Normalize();

	//	*m_matShadowView = XMMatrixIdentity();

	//	memcpy(&m_matShadowView->m[0][0], &vRight, sizeof(Vector3));
	//	memcpy(&m_matShadowView->m[1][0], &vUp, sizeof(Vector3));
	//	memcpy(&m_matShadowView->m[2][0], &vDir, sizeof(Vector3));

	//	*m_matShadowView = m_matShadowView->Transpose();

	//	m_matShadowView->m[3][0] = -vLightPos.Dot(vRight);
	//	m_matShadowView->m[3][1] = -vLightPos.Dot(vUp);
	//	m_matShadowView->m[3][2] = -vLightPos.Dot(vDir);
	//}

	//SAFE_RELEASE(pLightTr);

	return 0;
}

void CCamera::Collision(float fTime)
{
}

void CCamera::Render(float fTime)
{
}

CCamera * CCamera::Clone()
{
	return new CCamera(*this);
}

void CCamera::Save(FILE * pFile)
{
	CComponent::Save(pFile);

	fwrite(m_matView, sizeof(Matrix), 1, pFile);
	fwrite(m_matProj, sizeof(Matrix), 1, pFile);
}

void CCamera::Load(FILE * pFile)
{
	CComponent::Load(pFile);

	fread(m_matView, sizeof(Matrix), 1, pFile);
	fread(m_matProj, sizeof(Matrix), 1, pFile);
}

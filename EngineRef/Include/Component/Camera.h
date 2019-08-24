#pragma once
#include "Component.h"

PG_BEGIN

class PG_DLL CCamera :
	public CComponent
{
private:
	friend class CGameObject;

private:
	CCamera();
	CCamera(const CCamera& camera);
	~CCamera();

private:
	CAMERA_TYPE			m_eCameraType;
	class CTransform*	m_pAttach;
	Vector3				m_vPrevPos;
	RESOLUTION			m_tWorldRS;
	Vector2				m_vPivot;
	class CFrustum*		m_pFrustum;

public:
	void SetPivot(float x, float y);
	void SetPivot(const Vector2& vPivot);
	void SetWorldResolution(UINT x, UINT y);
	void SetWorldResolution(const RESOLUTION& tRS);
	void SetAttach(class CGameObject* pAttach);
	void SetAttach(class CComponent* pAttach);
	bool FrustumInPoint(const Vector3& vPos);
	bool FrustumInSphere(const Vector3& vCenter, float fRadius);
	bool FrustumInSphere(const SPHERE& tSphere);

private:
	PMatrix		m_matView;
	PMatrix		m_matProj;
	PMatrix		m_matShadowView;
	PMatrix		m_matShadowProj;

public:
	Matrix GetViewMatrix()	const;
	Matrix GetProjMatrix()	const;
	Matrix GetShadowViewMatrix()	const;
	Matrix GetShadowProjMatrix()	const;

public:
	XMMATRIX GetLightView();
	XMMATRIX GetLightProj();

private:
	XMMATRIX m_xmatLightView;
	XMMATRIX m_xmatLightProj;

public:
	void SetLightCenterPos(const Vector3& center);
	void SetLightRange(const float& range);
	void SetDistLookAtToEye();

	void SetLightCenterPosToObject(CGameObject *pGameObject);

	Vector3 GetLightCenterPos();
	float	GetLightRange();

private:
	Vector3 m_vCenter;
	float	m_fRange;

private:
	Vector3 m_vDist;

public:
	void SetOrthoProj(const RESOLUTION& tRS, float fNear, float fFar);
	void SetPerspectiveProj(float fViewAngle, float fAspect,
		float fNear, float fFar);
	void SetShadowOrthoProj(const RESOLUTION& tRS, float fNear, float fFar);
	void SetShadowPerspectiveProj(float fViewAngle, float fAspect,
		float fNear, float fFar);

private:
	void ComputeOrthoView();
	void ComputePerspectiveView();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CCamera* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

PG_END

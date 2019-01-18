#pragma once

#include "../PGEngine.h"

PG_BEGIN

enum FRUSTUM_DIR
{
	FD_LEFT,
	FD_RIGHT,
	FD_TOP,
	FD_BOTTOM,
	FD_NEAR,
	FD_FAR,
	FD_END
};

class PG_DLL CFrustum
{
private:
	friend class CCamera;

private:
	CFrustum();
	~CFrustum();

private:
	Vector3		m_vPos[8];
	Vector4		m_Plane[FD_END];
	//XMVECTOR	m_Plane[FD_END];

public:
	void UpdateFrustum(const Matrix& matInvVP);
	bool FrustumInPoint(const Vector3& vPos);
	bool FrustumInSphere(const Vector3& vCenter, float fRadius);
	bool FrustumInSphere(const SPHERE& tSphere);
};

PG_END

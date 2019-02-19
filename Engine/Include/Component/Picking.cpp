#include "Picking.h"
#include "../Device.h"
#include "../Component/Transform.h"

PG_USING


CPicking::CPicking()
{
}


CPicking::~CPicking()
{
}

bool CPicking::Picking_ToBuffer(Vector3 * pOut, Vector3 rayOrigin, Vector3 rayDir, vector<VERTEXBUMP>& vecVtx, vector<UINT>& vecIndex)
{
	// 피킹되는 인덱스를 체크.
	UINT pickedTriangle = -1;
	float min = (numeric_limits<float>::max)();

	for (int i = 0; i < vecIndex.size(); i += 3)
	{
		UINT i0 = vecIndex[i];
		UINT i1 = vecIndex[i + 1];
		UINT i2 = vecIndex[i + 2];

		VERTEXBUMP v0 = vecVtx[i0];
		VERTEXBUMP v1 = vecVtx[i1];
		VERTEXBUMP v2 = vecVtx[i2];

		float dist = 0.f;
		Vector3 temp1, temp2;
		temp1 = rayOrigin;
		temp2 = rayDir;

		if (this->IntersectTri(temp1, temp2, v0.vPos, v1.vPos, v2.vPos, dist))
		{
			if (dist < min)
			{
				min = dist;
				pickedTriangle = i;
				Vector3 vPos = temp1 + (temp2 * dist);
				*pOut = vPos;
				return true;
			}
		}
	}

	return false;
}

XMGLOBALCONST XMVECTORF32 g_RayEpsilon = { 1e-20f, 1e-20f, 1e-20f, 1e-20f };
XMGLOBALCONST XMVECTORF32 g_RayNegEpsilon = { -1e-20f, -1e-20f, -1e-20f, -1e-20f };

bool CPicking::IntersectTri(Vector3 rayOrigin, Vector3 rayDir, Vector3 v0, Vector3 v1, Vector3 v2, float & dist)
{
	Vector3 e1, e2, h, s, q;
	float a, f, u, v;

	e1 = v1 - v0;
	e2 = v2 - v0;

	h = rayDir.Cross(e2);
	a = e1.Dot(h);

	// 구하는 공식
	// t = e2 * (m X e1) / e1 * (u X e2)
	// u = m * (u X e2) / e1 * (u X e2)
	// v = u * (m X e1) / e1 * (u X e2)


	if (a > -0.00001 && a < 0.00001) return false;

	f = 1 / a;
	s = rayOrigin - v0;

	// u
	u = f * s.Dot(h);

	if (u < 0.f || u > 1.f) return false;

	q = s.Cross(e1);
	
	// v
	v = f * rayDir.Dot(q);

	if (v < 0.f || u + v > 1.f) return false;

	// t
	dist = f * e2.Dot(q);

	if (dist > 0.00001)
		return true;
	else
		return false;
}

bool CPicking::Init()
{
	return true;
}

void CPicking::Input(float fTime)
{
}

int CPicking::Update(float fTime)
{
	return 0;
}

int CPicking::LateUpdate(float fTime)
{
	return 0;
}

void CPicking::Collision(float fTime)
{
}

void CPicking::Render(float fTime)
{
}

CPicking * CPicking::Clone()
{
	return nullptr;
}

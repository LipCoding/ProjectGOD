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

bool CPicking::Picking_ToBuffer(Vector3 * pOut, const Matrix & worldMatrix, const Vector2 & mousePos, CCamera * pCamera, vector<VERTEXBUMP>& vecVtx, vector<UINT>& vecIndex)
{
	Matrix matProj, matView, matInvView, matWorld, matInvWorld;

	matProj = pCamera->GetProjMatrix();

	// 뷰포트를 가져온다.
	D3D11_VIEWPORT tVP{};

	UINT iVPCount = 1;
	CONTEXT->RSGetViewports(&iVPCount, &tVP);

	float viewX, viewY;

	viewX = (2.f * mousePos.x / tVP.Width - 1.f) / matProj.m[0][0];
	viewY = (-2.f * mousePos.y / tVP.Height + 1.f) / matProj.m[1][1];

	matView = pCamera->GetViewMatrix();
	matInvView = XMMatrixInverse(&XMMatrixDeterminant(matView.mat), matView.mat);

	matWorld = worldMatrix;
	matInvWorld = XMMatrixInverse(&XMMatrixDeterminant(matWorld.mat), matWorld.mat);

	Matrix toLocal = XMMatrixMultiply(matInvView.mat, matInvWorld.mat);

	XMVECTOR rayOrigin, rayDir;
	Vector3 camPos = pCamera->GetTransform()->GetLocalPos();
	//rayOrigin = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	rayOrigin = XMVectorSet(camPos.x, camPos.y, camPos.z, 1.f);
	rayDir = XMVectorSet(viewX, viewY, 1.f, 0.f);

	rayOrigin = XMVector3TransformCoord(rayOrigin, toLocal.mat);
	rayDir = XMVector3TransformNormal(rayDir, toLocal.mat);

	rayDir = XMVector3Normalize(rayDir);

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

	if (a > -0.00001 && a < 0.00001) return false;

	f = 1 / a;
	s = rayOrigin - v0;

	u = f * s.Dot(h);

	if (u < 0.f || u > 1.f) return false;

	q = s.Cross(e1);

	v = f * rayDir.Dot(q);

	if (v < 0.f || u + v > 1.f) return false;

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

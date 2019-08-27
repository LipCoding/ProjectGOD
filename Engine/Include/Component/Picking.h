#pragma once
#include "Component.h"
#include "../Component/Camera.h"

PG_BEGIN

class PG_DLL CPicking :
	public CComponent
{
private:
	friend class CGameObject;

private:
	CPicking();
	CPicking(const CPicking& picking);
	~CPicking();

public:
	bool Picking_ToBuffer(Vector3 *pOut, Vector3 rayOrigin,  Vector3 rayDir, vector<VERTEXBUMP>& vecVtx, vector<UINT>& vecIndex);
	bool Picking_ToBuffer(Vector3 *pOut, Vector3 rayOrigin, Vector3 rayDir, vector<Vector3>& vecVtx, vector<int>& vecIndex);
	//bool Picking_ToMesh();

private:
	bool IntersectTri(Vector3 rayOrigin, Vector3 rayDir, Vector3 v0, Vector3 v1, Vector3 v2, float& dist);

private:
	LONG m_lBackSizeX = 0;
	LONG m_lBackSizeY = 0;
	

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CPicking* Clone();
};

PG_END
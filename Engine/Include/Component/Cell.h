#pragma once
#include "Component.h"

PG_BEGIN

class PG_DLL CCell
{
public:
	CCell();
	CCell(const CCell& axisLine);
	~CCell();

private:
	class CMesh*		m_pMesh;
	class CShader*		m_pShader;

	Vector3		        m_points[3];
	ID3D11InputLayout*	m_pLayout;
	TRANSFORMCBUFFER	m_tTransform;
	bool				m_bRenderCheck = true;

public:
	void SetPoints(const vector<Vector3>& vecPoints)
	{
		if (vecPoints.size() < 3)
			return;

		m_points[0] = vecPoints[0];
		m_points[1] = vecPoints[1];
		m_points[2] = vecPoints[2];
	}
	void SetRenderCheck(const bool& check)
	{
		m_bRenderCheck = check;
	}

public:
	bool Init(const vector<Vector3>& vecPoints);
	int Update(float fTime);
	int LateUpdate(float fTime);
	void Render(float fTime);
	CCell* Clone();
};

PG_END
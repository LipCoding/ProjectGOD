#pragma once
#include "Component.h"

PG_BEGIN

class PG_DLL CTriangle
{
public:
	CTriangle();
	CTriangle(const CTriangle& axisLine);
	~CTriangle();

private:
	class CMesh*		m_pMesh;
	class CShader*		m_pShader;

	vector<Vector3>     m_vecPoints;
	ID3D11InputLayout*	m_pLayout;
	TRANSFORMCBUFFER	m_tTransform;
	bool				m_bRenderCheck = true;
public:
	void SetPoints(const vector<Vector3>& vecPoints)
	{
		m_vecPoints = vecPoints;
	}
	void SetRenderCheck(const bool& check)
	{
		m_bRenderCheck = check;
	}

public:
	bool Init();
	void Input(float fTime);
	int Update(float fTime);
	int LateUpdate(float fTime);
	void Render(float fTime);
	void TriRender(float fTime);
	CTriangle* Clone();
};

PG_END
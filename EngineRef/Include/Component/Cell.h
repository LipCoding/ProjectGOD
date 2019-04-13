#pragma once
#include "Component.h"
#include "../Resources/Mesh.h"

PG_BEGIN

class PG_DLL CCell
{
public:
	CCell();
	CCell(const CCell& cell);
	~CCell();

private:
	class CShader*		m_pShader;

	Vector3		        m_points[3];
	TRANSFORMCBUFFER	m_tTransform;

	VERTEXBUFFER m_tVB;
	INDEXBUFFER m_tIB;

public:
	bool Init(const vector<NAVIPOINT>& vecPoints);
	int Update();
	int LateUpdate();
	void Render();
	CCell* Clone();

public:
	bool CreateVertexBuffer(UINT iVtxCount, UINT iVtxSize, D3D11_USAGE eVtxUsage,
		D3D11_PRIMITIVE_TOPOLOGY ePrimitive, void* pVtxData);
	bool CreateIndexBuffer(UINT iIdxCount = 0, UINT iIdxSize = 0,
		D3D11_USAGE eIdxUsage = D3D11_USAGE_DEFAULT,
		DXGI_FORMAT eFmt = DXGI_FORMAT_R32_UINT,
		void* pIdxData = NULL);

private:
	bool SetPoints(const vector<NAVIPOINT>& vecPoints)
	{
		if (vecPoints.size() < 3)
			return false;

		m_points[0] = vecPoints[0].vPosition;
		m_points[1] = vecPoints[1].vPosition;
		m_points[2] = vecPoints[2].vPosition;

		return true;
	}
};

PG_END
#pragma once
#include "Component.h"
#include "../Resources/Mesh.h"

PG_BEGIN

class PG_DLL CCell
{
public:
	enum CELL_OPT{OPT_TERRAIN, OPT_OBJECT, OPT_END};
	enum POINT{POINT_A, POINT_B, POINT_C, POINT_END};
	enum DIRECTION{DIR_AB, DIR_BC, DIR_CA, DIR_END};
	enum NEIGHBOR{NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END};

	enum { DW_A = 1 << 0, DW_B = 1 << 1, DW_C = 1 << 2 };

public:
	CCell();
	CCell(const CCell& cell);
	~CCell();

/* Getter Setter */
public:
	const UINT&			Get_Index() { return m_iIndex; }
	const Vector3&		Get_Point(const UINT& iIndex) {return m_vPoints[iIndex]; }
	const Vector3&		Get_Direction(const UINT& iIndex) { return m_vDir[iIndex]; }
	const Vector3&		Get_Normal(const UINT& iIndex) { return m_vNormal[iIndex]; }
	Vector3				Get_CenterPos() { return m_vCenter; }
	const CELL_OPT&		Get_Option() { return m_eOption; }

public:
	void	Set_Neighbor(DIRECTION eDir, CCell* pNeighborCell) { m_pNeighbor[eDir] = pNeighborCell; }
	void	Set_Neighbor(NEIGHBOR eDir, CCell* pNeighborCell) { m_pNeighbor[eDir] = pNeighborCell; }
	void	Set_Index(int iIndex) { m_iIndex = iIndex; }

/* Navi Calculate */
public:
	bool Check_Position(const Vector3& vPos, Vector3* vDir, int* iIdx, int* iOutCount);
	const bool Check_Inclusion(const Vector3& vPos);

	void Compute_Neighbor(CCell* pTargetCell);
	bool Compute_NeighborCell(const Vector3& pPoint1, const Vector3& pPoint2, CCell* pCell);


private:
	/* Visualization */
	class CShader*		m_pShader;
	TRANSFORMCBUFFER	m_tTransform;
	VERTEXBUFFER m_tVB;
	INDEXBUFFER m_tIB;

	/* Information */
	CELL_OPT			m_eOption = OPT_TERRAIN;
	Vector3				m_vPoints[POINT_END];
	Vector3				m_vDir[DIR_END];
	Vector3				m_vEdgeCenter[DIR_END];
	Vector3				m_vNormal[DIR_END];
	CCell*				m_pNeighbor[DIR_END];
	Vector3 			m_vCenter;

	UINT				m_iIndex = 0;


public:
	/* Default */
	bool InitCell(const vector<Vector3>& vecPoints, CCell::CELL_OPT eCellOpt = OPT_TERRAIN);
	bool InitCell(const Vector3* vecPoints, CCell::CELL_OPT eCellOpt = OPT_TERRAIN);
	int UpdateCell();
	int LateUpdateCell();
	void RenderCell();
	CCell* Clone();

private:
	/* Visualization */
	bool CreateVertexBuffer(UINT iVtxCount, UINT iVtxSize, D3D11_USAGE eVtxUsage,
		D3D11_PRIMITIVE_TOPOLOGY ePrimitive, void* pVtxData);
	bool CreateIndexBuffer(UINT iIdxCount = 0, UINT iIdxSize = 0,
		D3D11_USAGE eIdxUsage = D3D11_USAGE_DEFAULT,
		DXGI_FORMAT eFmt = DXGI_FORMAT_R32_UINT,
		void* pIdxData = NULL);

	/* Calculation */
	void Calc_Point(Vector3 P0, Vector3 P1, Vector3 P2);
};

PG_END
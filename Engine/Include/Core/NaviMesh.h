#pragma once
#include "../Component/Cell.h"

PG_BEGIN

class PG_DLL CNaviMesh
{
public:
	CNaviMesh();
	~CNaviMesh();

	/* Getter Setter */
public:
	CCell*	Get_Cell(const int& iIndex) { return m_Cells[iIndex]; }
	CCell*	Get_CurCell() { return m_Cells[m_iCurIndex]; }
	Vector3 Get_CellPosition(const int& iIndex);
	UINT	Get_PositionIndex(const Vector3& vPos);
	UINT	Get_CurCellIndex() { return m_iCurIndex; }

	const CCell::CELL_OPT& Get_CellOption(void) { return m_Cells[m_iCurIndex]->Get_Option(); }

public:
	void	Set_CurIndex(const UINT& index) { m_iCurIndex = index; }

public:
	size_t GetCellSize() { return m_Cells.size(); }
	vector<CCell*>* GetCells() { return &m_Cells; }
	bool   GetCellIsEmpty() { return m_Cells.empty(); }


public:
	float			Get_y(const Vector3& position)
	{
		XMVECTOR plane;
		Vector4  vPlane;

		const Vector3* point[3] = {
			&m_Cells[m_iCurIndex]->Get_Point(0)
			, &m_Cells[m_iCurIndex]->Get_Point(1)
			, &m_Cells[m_iCurIndex]->Get_Point(2)
		};

		plane = XMPlaneFromPoints(point[0]->Convert(), point[1]->Convert(), point[2]->Convert());
		XMStoreFloat4(&vPlane, plane);

		float a, b, c, d, x, y, z;

		x = position.x;
		z = position.z;

		a = vPlane.x;
		b = vPlane.y;
		c = vPlane.z;
		d = vPlane.w;

		y = -(a * x + c * z + d) / b;
		return y;
	}

private:
	vector<CCell*>		m_Cells;
	class CRenderState*	m_pDepthDisable;
	UINT				m_iCurIndex = 0;

public:
	void AddCell(const vector<Vector3>& points);
	void AddCell(const Vector3* points);
	void Compute_Neighbor(void);

public:
	bool InitNavi();
	int  UpdateNavi();
	void RenderNavi();

	bool Check_Position(const Vector3& vPos, Vector3* vDir);
	
	void FreeCell() { Safe_Delete_VecList(m_Cells); };
	
};

PG_END
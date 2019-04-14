#include "NaviMesh.h"
#include "../Rendering/RenderManager.h"
#include "../Rendering/RenderState.h"

PG_USING

CNaviMesh::CNaviMesh()
{
}

CNaviMesh::~CNaviMesh()
{
	Safe_Delete_VecList(m_Cells);
	SAFE_RELEASE(m_pDepthDisable);
}

Vector3 CNaviMesh::Get_CellPosition(const int & iIndex)
{
	return m_Cells[iIndex]->Get_CenterPos();
}

UINT CNaviMesh::Get_PositionIndex(const Vector3 & vPos)
{
	UINT iSize = (UINT)m_Cells.size();

	for (UINT i = 0; i < iSize; ++i)
	{
		if (m_Cells[i]->Check_Inclusion(vPos) == true)
			return i;
	}

	return -1;
}

void CNaviMesh::AddCell(const vector<NAVIPOINT>& points)
{
	CCell* pCell = new CCell;

	if (!pCell->InitCell(points))
	{
		SAFE_DELETE(pCell);
		return;
	}

	m_Cells.push_back(pCell);
}


void CNaviMesh::AddCell(const Vector3* points)
{
	CCell* pCell = new CCell;

	if (!pCell->InitCell(points))
	{
		SAFE_DELETE(pCell);
		return;
	}

	m_Cells.push_back(pCell);
}

void CNaviMesh::Compute_Neighbor(void)
{
	UINT iSize = (UINT)m_Cells.size();

	for (UINT i = 0; i < iSize; ++i)
	{
		for (UINT j = 0; j < iSize; ++j)
		{
			if (i == j)
				continue;

			if (m_Cells[j]->Compute_NeighborCell(m_Cells[i]->Get_Point(CCell::POINT_A), m_Cells[i]->Get_Point(CCell::POINT_B), m_Cells[i]))
				m_Cells[i]->Set_Neighbor(CCell::NEIGHBOR_AB, m_Cells[j]);
			
			else if (m_Cells[j]->Compute_NeighborCell(m_Cells[i]->Get_Point(CCell::POINT_B), m_Cells[i]->Get_Point(CCell::POINT_C), m_Cells[i]))
				m_Cells[i]->Set_Neighbor(CCell::NEIGHBOR_BC, m_Cells[j]);
			
			else if (m_Cells[j]->Compute_NeighborCell(m_Cells[i]->Get_Point(CCell::POINT_C), m_Cells[i]->Get_Point(CCell::POINT_A), m_Cells[i]))
				m_Cells[i]->Set_Neighbor(CCell::NEIGHBOR_CA, m_Cells[j]);
		}

		m_Cells[i]->Set_Index(i);
	}
}

bool CNaviMesh::InitNavi()
{
	m_pDepthDisable = GET_SINGLE(CRenderManager)->FindRenderState(DEPTH_DISABLE);

	return true;
}

int CNaviMesh::UpdateNavi()
{
	for (const auto& iter : m_Cells)
	{
		if (!iter->UpdateCell())
			return -1;
	}

	return 0;
}

void CNaviMesh::RenderNavi()
{
	m_pDepthDisable->SetState();
	for (const auto& iter : m_Cells)
	{
		iter->RenderCell();
	}
	m_pDepthDisable->ResetState();
}

bool CNaviMesh::Check_Position(const Vector3 & vPos, Vector3 * vDir)
{
	bool beAble = false;

	int iIdx = m_iCurIndex;
	int iOutCount = 0;

	beAble = m_Cells[m_iCurIndex]->Check_Position(vPos, vDir, &iIdx, &iOutCount);

	if (true == beAble)
		m_iCurIndex = iIdx;

	return beAble;
}

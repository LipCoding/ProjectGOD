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

void CNaviMesh::AddCell(const vector<NAVIPOINT>& points)
{
	CCell* pCell = new CCell;

	if (!pCell->Init(points))
	{
		SAFE_DELETE(pCell);
		return;
	}

	m_Cells.push_back(pCell);
}

bool CNaviMesh::Init()
{
	m_pDepthDisable = GET_SINGLE(CRenderManager)->FindRenderState(DEPTH_DISABLE);

	return true;
}

int CNaviMesh::Update()
{
	for (const auto& iter : m_Cells)
	{
		if (!iter->Update())
			return -1;
	}

	return 0;
}

void CNaviMesh::Render()
{
	m_pDepthDisable->SetState();
	for (const auto& iter : m_Cells)
	{
		iter->Render();
	}
	m_pDepthDisable->ResetState();
}

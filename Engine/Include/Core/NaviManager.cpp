#include "NaviManager.h"
#include "../Scene/Scene.h"
#include "../Component/Transform.h"
#include "NaviMesh.h"
#include "../Core/PathManager.h"

PG_USING

DEFINITION_SINGLE(CNaviManager)

CNaviManager::CNaviManager()
{
}

CNaviManager::~CNaviManager()
{
	//Safe_Delete_Map(m_mapNaviMesh);
	FreeNaviMesh();
}

/*{
	CNaviMesh * CNaviManager::CreateNaviMesh(const string & strKey)
	{
		CNaviMesh* pNavi = FindNaviMesh(strKey);
	
		if (pNavi)
			return pNavi;
	
		pNavi = new CNaviMesh;
	
		if (!pNavi->InitNavi())
		{
			SAFE_DELETE(pNavi);
			return nullptr;
		}
	
		m_mapNaviMesh.insert(make_pair(strKey, pNavi));
	
		m_curMeshName = strKey;
	
		return pNavi;
	}
	
	CNaviMesh * CNaviManager::FindNaviMesh(const string & strKey)
	{
		unordered_map<string, CNaviMesh*>::iterator iter = m_mapNaviMesh.find(strKey);
	
		if (iter == m_mapNaviMesh.end())
			return nullptr;
	
		return iter->second;
	}
	
	CNaviMesh * CNaviManager::FindNaviMesh(const Vector3 & vPos)
	{
		return nullptr;
	}
	
	void CNaviManager::Render(float fTime)
	{
		if (!m_bRenderCheck)
			return;
	
		CNaviMesh* pNavi = FindNaviMesh(m_curMeshName);
	
		if (pNavi)
		{
			pNavi->RenderNavi();
		}
	}
}*/

const vector<class CCell*>* CNaviManager::GetNaviCells()
{
	if (m_pCurrentNaviMesh == nullptr)
		return nullptr;

	return m_pCurrentNaviMesh->GetCells();
}

float CNaviManager::GetY(const Vector3& vPos)
{
	if(m_pCurrentNaviMesh == nullptr)
		return 0.0f;

	return m_pCurrentNaviMesh->Get_y(vPos);
}

CNaviMesh * CNaviManager::CreateNaviMesh()
{
	FreeNaviMesh();

	m_pCurrentNaviMesh = new CNaviMesh;

	if (!m_pCurrentNaviMesh->InitNavi())
	{
		FreeNaviMesh();
		return nullptr;
	}

	return m_pCurrentNaviMesh;
}

CNaviMesh * CNaviManager::CreateNaviMesh(const string & fileName)
{
	string flexiblePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(DATA_PATH);
	string filePath = "Navi\\" + fileName + ".bin";

	FreeNaviMesh();
	
	m_pCurrentNaviMesh = new CNaviMesh;

	if (!m_pCurrentNaviMesh->InitNavi())
	{
		FreeNaviMesh();
		return nullptr;
	}

	// 파일 불러오기
	ifstream mainFile;
	mainFile.open(flexiblePath + filePath, ios::in);

	if (!mainFile.is_open())
	{
		FreeNaviMesh();
		return nullptr;
	}
	
	// Cell 크기를 불러오고
	UINT iCellSize = 0;
	mainFile >> iCellSize;

	// 크기만큼 순회돌며
	for (UINT i = 0; i < iCellSize; ++i)
	{
		Vector3 pVec[3];

		mainFile >> pVec[0].x >> pVec[0].y >> pVec[0].z;
		mainFile >> pVec[1].x >> pVec[1].y >> pVec[1].z;
		mainFile >> pVec[2].x >> pVec[2].y >> pVec[2].z;

		AddCell(pVec);
	}

	// 이웃 계산
	m_pCurrentNaviMesh->Compute_Neighbor();

	mainFile.close();
	
	return m_pCurrentNaviMesh;
}

void CNaviManager::EraseCell(const CCell * searchCell)
{
	if (m_pCurrentNaviMesh == nullptr)
		return;

	vector<CCell*>* pVecCells = m_pCurrentNaviMesh->GetCells();

	auto& cell = remove_if(pVecCells->begin(), pVecCells->end(),
		[&](CCell* cell) {return searchCell == cell; });

	if (cell == pVecCells->end())
		return;

	pVecCells->erase(cell, pVecCells->end());
}

void CNaviManager::UndoCell()
{
	if (m_pCurrentNaviMesh == nullptr)
		return;

	vector<CCell*>* pVecCells = m_pCurrentNaviMesh->GetCells();

	auto& iter_end = pVecCells->rbegin();
	SAFE_DELETE(*iter_end);
	pVecCells->pop_back();
}

bool CNaviManager::CheckPosition(const Vector3 & vPos, Vector3 * vDir)
{
	if (m_pCurrentNaviMesh == nullptr)
		return false;

	return m_pCurrentNaviMesh->Check_Position(vPos, vDir);
}

void CNaviManager::FreeNaviMesh()
{
	if (m_pCurrentNaviMesh == nullptr)
		return;

	SAFE_DELETE(m_pCurrentNaviMesh);
}

void CNaviManager::AddCell(const vector<Vector3>& vPoint)
{
	if (m_pCurrentNaviMesh == nullptr)
		CreateNaviMesh();

	m_pCurrentNaviMesh->AddCell(vPoint);
}

void CNaviManager::AddCell(const Vector3 * vPoint)
{
	if (m_pCurrentNaviMesh == nullptr)
		CreateNaviMesh();

	m_pCurrentNaviMesh->AddCell(vPoint);
}

bool CNaviManager::IsCellEmpty()
{
	if (m_pCurrentNaviMesh == nullptr)
		return true;

	return m_pCurrentNaviMesh->GetCellIsEmpty();
}

void CNaviManager::Render(float fTime)
{
	if (m_pCurrentNaviMesh == nullptr ||
		m_bRenderCheck == false)
		return;

	m_pCurrentNaviMesh->RenderNavi();
}

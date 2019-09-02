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
	Safe_Delete_Map(m_mapNaviMesh);	
}


CNaviMesh * CNaviManager::CreateNaviMesh(const string & strKey)
{
	CNaviMesh* pNavi = nullptr;
	if (strKey == "")
	{
		pNavi = FindNaviMesh(m_curNaviName);
	}
	else
	{
		pNavi = FindNaviMesh(strKey);
		m_curNaviName = strKey;
	}

	if (pNavi)
		return pNavi;

	pNavi = new CNaviMesh;

	if (!pNavi->InitNavi())
	{
		SAFE_DELETE(pNavi);
		return nullptr;
	}

	m_mapNaviMesh.insert(make_pair(strKey, pNavi));

	m_curNaviName = strKey;

	return pNavi;
}

CNaviMesh * CNaviManager::CreateNaviMeshFromFile(const string & fileName)
{
	string flexiblePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(DATA_PATH);
	string filePath = "Navi\\" + fileName + ".bin";

	m_curNaviName = fileName;

	CNaviMesh *pCurrentNaviMesh = new CNaviMesh;

	m_mapNaviMesh.insert(make_pair(fileName, pCurrentNaviMesh));

	if (!pCurrentNaviMesh->InitNavi())
	{
		FreeNaviMesh(fileName);
		return nullptr;
	}

	// 파일 불러오기
	ifstream mainFile;
	mainFile.open(flexiblePath + filePath, ios::in);

	if (!mainFile.is_open())
	{
		FreeNaviMesh(fileName);
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
	pCurrentNaviMesh->Compute_Neighbor();

	mainFile.close();

	return pCurrentNaviMesh;
}

CNaviMesh * CNaviManager::FindNaviMesh(const string & strKey)
{
	unordered_map<string, CNaviMesh*>::iterator iter = m_mapNaviMesh.find(strKey);

	if (iter == m_mapNaviMesh.end())
		return nullptr;

	return iter->second;
}

void CNaviManager::AddCell(const vector<Vector3>& vPoint, const string & strKey)
{
	CNaviMesh* pNavi = nullptr;

	if (strKey == "")
		pNavi = FindNaviMesh(m_curNaviName);
	else
		pNavi = FindNaviMesh(strKey);

	if (pNavi == nullptr)
	{
		pNavi = CreateNaviMesh(strKey);
		pNavi->AddCell(vPoint);
	}
	else
	{
		pNavi->AddCell(vPoint);
	}
}

void CNaviManager::AddCell(const Vector3 * vPoint, const string & strKey)
{
	CNaviMesh* pNavi = nullptr;

	if (strKey == "")
		pNavi = FindNaviMesh(m_curNaviName);
	else
		pNavi = FindNaviMesh(strKey);

	if (pNavi == nullptr)
	{
		pNavi = CreateNaviMesh(strKey);
		pNavi->AddCell(vPoint);
	}
	else
	{
		pNavi->AddCell(vPoint);
	}
}

void CNaviManager::EraseCell(const CCell * searchCell, const string & strKey)
{
	CNaviMesh* pNavi = nullptr;

	if (strKey == "")
		pNavi = FindNaviMesh(m_curNaviName);
	else
		pNavi = FindNaviMesh(strKey);

	if (pNavi == nullptr)
		return;

	vector<CCell*>* pVecCells = pNavi->GetCells();

	auto& cell = remove_if(pVecCells->begin(), pVecCells->end(),
		[&](CCell* cell) {return searchCell == cell; });

	if (cell == pVecCells->end())
		return;

	pVecCells->erase(cell, pVecCells->end());
}

void CNaviManager::UndoCell(const string & strKey)
{
	CNaviMesh* pNavi = nullptr;

	if (strKey == "")
		pNavi = FindNaviMesh(m_curNaviName);
	else
		pNavi = FindNaviMesh(strKey);

	if (pNavi == nullptr)
		return;

	vector<CCell*>* pVecCells = pNavi->GetCells();

	auto& iter_end = pVecCells->rbegin();
	SAFE_DELETE(*iter_end);
	pVecCells->pop_back();
}

bool CNaviManager::CheckPosition(const Vector3 & vPos, Vector3 * vDir, const string & strKey)
{
	CNaviMesh* pNavi = nullptr;

	if (strKey == "")
		pNavi = FindNaviMesh(m_curNaviName);
	else
		pNavi = FindNaviMesh(strKey);

	if (pNavi == nullptr)
		return false;

	return pNavi->Check_Position(vPos, vDir);
}

void CNaviManager::FreeNaviMesh(const string & strKey)
{
	CNaviMesh* pNavi = nullptr;

	if (strKey == "")
		pNavi = FindNaviMesh(m_curNaviName);
	else
		pNavi = FindNaviMesh(strKey);

	if (pNavi == nullptr)
		return;

	SAFE_DELETE(pNavi);

	if (strKey == "")
		m_mapNaviMesh.erase(m_curNaviName);
	else
		m_mapNaviMesh.erase(strKey);
}

void CNaviManager::Render(float fTime)
{
	if (!m_bRenderCheck)
		return;

	CNaviMesh* pNavi = FindNaviMesh(m_curNaviName);

	if (pNavi)
	{
		pNavi->RenderNavi();
	}
}

const vector<class CCell*>* CNaviManager::GetNaviCells(const string & strKey)
{
	CNaviMesh* pNavi = nullptr;

	if (strKey == "")
		pNavi = FindNaviMesh(m_curNaviName);
	else
		pNavi = FindNaviMesh(strKey);

	if (pNavi == nullptr)
		return nullptr;

	return pNavi->GetCells();
}

bool CNaviManager::IsCellEmpty(const string & strKey)
{
	CNaviMesh* pNavi = nullptr;

	if (strKey == "")
		pNavi = FindNaviMesh(m_curNaviName);
	else
		pNavi = FindNaviMesh(strKey);

	if (pNavi == nullptr)
		return true;

	return pNavi->GetCellIsEmpty();
}

float CNaviManager::GetY(const Vector3 & vPos, const string & strKey)
{
	CNaviMesh* pNavi = nullptr;

	if (strKey == "")
		pNavi = FindNaviMesh(m_curNaviName);
	else
		pNavi = FindNaviMesh(strKey);

	if (pNavi == nullptr)
		return 0.0f;

	return pNavi->Get_y(vPos);
}
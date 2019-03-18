#include "QuadTreeManager.h"
#include "../Component/Transform.h"
#include "../Scene/Scene.h"

PG_USING

DEFINITION_SINGLE(CQuadTreeManager)

CQuadTreeManager::CQuadTreeManager()
{
}


CQuadTreeManager::~CQuadTreeManager()
{
	Safe_Delete_Map(m_mapQuadTreeInfo);
}

void CQuadTreeManager::AddQuadTreeInfo(const string & key, int numX, int numY, Vector3 min, Vector3 max, CGameObject * object)
{
	PQUADTREEINFO pInfo = FindQuadTreeInfo(key);
}

void CQuadTreeManager::DeleteQuadTreeInfo(CScene * scene)
{
}

PQUADTREEINFO CQuadTreeManager::FindQuadTreeInfo(const string & key)
{
	unordered_map<string, PQUADTREEINFO>::iterator iter = m_mapQuadTreeInfo.find(key);

	
}

PQUADTREEINFO CQuadTreeManager::FindQuadTreeInfo(const Vector3 & pos)
{
	
}

float CQuadTreeManager::GetY(const Vector3 & vPos)
{
	return 0.0f;
}

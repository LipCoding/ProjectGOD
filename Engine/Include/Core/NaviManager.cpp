#include "NaviManager.h"
#include "../Scene/Scene.h"
#include "../Component/Transform.h"
#include "NaviMesh.h"

PG_USING

DEFINITION_SINGLE(CNaviManager)

CNaviManager::CNaviManager()
{
}

CNaviManager::~CNaviManager()
{
}

CNaviMesh * CNaviManager::CreateNaviMesh(const string & strKey)
{
	CNaviMesh* pNavi = FindNaviMesh(strKey);

	if (pNavi)
		return pNavi;

	pNavi = new CNaviMesh;

	if (!pNavi->Init())
	{
		SAFE_DELETE(pNavi);
		return nullptr;
	}

	m_mapNaviMesh.insert(make_pair(strKey, pNavi));

	return pNavi;
}

CNaviMesh * CNaviManager::FindNaviMesh(const string & strKey)
{
	return nullptr;
}

CNaviMesh * CNaviManager::FindNaviMesh(const Vector3 & vPos)
{
	return nullptr;
}

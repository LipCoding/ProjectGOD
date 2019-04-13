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
	Safe_Delete_Map(m_mapNaviMesh);
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
		pNavi->Render();
	}
}

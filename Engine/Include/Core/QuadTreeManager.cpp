#include "QuadTreeManager.h"
#include "../GameObject/GameObject.h"
#include "../Component/Renderer.h"
#include "../Resources/Mesh.h"
#include "../Component/Transform.h"
#include "../Scene/Scene.h"

PG_USING

DEFINITION_SINGLE(CQuadTreeManager)

CQuadTreeManager::CQuadTreeManager()
{
}


CQuadTreeManager::~CQuadTreeManager()
{
	map<string, PQUADTREEINFO>::iterator	iter;
	map<string, PQUADTREEINFO>::iterator  iterEnd = m_mapQuadTreeInfo.end();

	for (iter = m_mapQuadTreeInfo.begin(); iter != iterEnd; ++iter)
	{
		for (auto& object : iter->second->pChildObjects)
		{
			SAFE_RELEASE(object);
		}
		iter->second->pChildObjects.clear();
	}

	Safe_Delete_Map(m_mapQuadTreeInfo);
}

void CQuadTreeManager::AddQuadTreeInfo(const string & key, int numX, int numY, Vector3 min, Vector3 max, CGameObject * object)
{
	PQUADTREEINFO pInfo = FindQuadTreeInfo(key);

	if (pInfo)
		return;

	pInfo = new QUADTREEINFO;

	pInfo->iSizeX = numX;
	pInfo->iSizeY = numY;
	pInfo->vMin = min;
	pInfo->vMax = max;
	pInfo->pGameObject = object;
	pInfo->bCullingCheck = false;

	m_mapQuadTreeInfo.insert(make_pair(key, pInfo));
}

void CQuadTreeManager::DeleteQuadTreeInfo()
{
	map<string, PQUADTREEINFO>::iterator	iter;
	map<string, PQUADTREEINFO>::iterator  iterEnd = m_mapQuadTreeInfo.end();

	for (iter = m_mapQuadTreeInfo.begin(); iter != iterEnd; ++iter)
	{
		iter->second->pGameObject = nullptr;
		iter->second->pChildObjects.clear();
	}

	Safe_Delete_Map(m_mapQuadTreeInfo);
}

PQUADTREEINFO CQuadTreeManager::FindQuadTreeInfo(const string & key)
{
	map<string, PQUADTREEINFO>::iterator iter = m_mapQuadTreeInfo.find(key);

	if (iter == m_mapQuadTreeInfo.end())
		return NULL;

	return iter->second;
	
}

PQUADTREEINFO CQuadTreeManager::FindQuadTreeInfo(const Vector3 & pos)
{
	map<string, PQUADTREEINFO>::iterator iter;
	map<string, PQUADTREEINFO>::iterator iterEnd = m_mapQuadTreeInfo.end();

	for (iter = m_mapQuadTreeInfo.begin(); iter != iterEnd; ++iter)
	{
		if (iter->second->vMin <= pos && pos <= iter->second->vMax)
			return iter->second;
	}

	return NULL;
}

float CQuadTreeManager::GetY(const Vector3 & vPos)
{
	//CNavigationMesh*	pMesh = FindNavigationMesh(vPos);

	//if (!pMesh)
	//	return 0.f;

	//return pMesh->GetY(vPos);

	// 먼저 지형 정보를 얻어온다.
	PQUADTREEINFO	pInfo = FindQuadTreeInfo(vPos);

	if (!pInfo)
		return 0.f;

	CTransform* pTransform = pInfo->pGameObject->GetTransform();
	CRenderer* pRenderer = pInfo->pGameObject->FindComponentFromType<CRenderer>(CT_RENDERER);
	CMesh* pMesh = pRenderer->GetMesh();
	vector<Vector3>* pvecPos = new vector<Vector3>;
	pMesh->GetMeshPosition(pvecPos);

	SAFE_RELEASE(pMesh);
	SAFE_RELEASE(pRenderer);

	// 지형 정보가 있을 경우 현재 위치가 지형의 어느 사각형에 존재하는지를
	// 얻어온다.
	Vector3	vScale = pTransform->GetWorldScale();

	float	fWidth = (*pvecPos)[1].x - (*pvecPos)[0].x;
	float	fHeight = (*pvecPos)[pInfo->iSizeX + 1].z - (*pvecPos)[0].z;

	// 지형 격자의 하나의 Cell 크기를 구한다.
	fWidth *= vScale.x;
	fHeight *= vScale.z;

	// 원점 기준으로 지형을 맞춰준다.
	Vector3	vLandScapePos = pTransform->GetWorldPos();

	Vector3	vOriginPos = vPos;

	vOriginPos -= vLandScapePos;

	// 한칸의 크기를 1, 1 크기로 만들어준다.
	vOriginPos.x /= fWidth;
	vOriginPos.z /= fHeight;

	vOriginPos.x -= pInfo->vMin.x;
	vOriginPos.z -= pInfo->vMin.z;


	// 예외처리. x 범위나 z 범위 밖으로 나갔을 경우 예외처리한다.
	if (vOriginPos.x <= 0 || vOriginPos.x >= pInfo->iSizeX ||
		vOriginPos.z <= 0 || vOriginPos.z >= pInfo->iSizeY)
		return 0.f;

	// 어느 사각형에 위치해 있는지 인덱스를 구한다.
	int	xIdx = (int)vOriginPos.x;
	int	zIdx = (int)vOriginPos.z - 1;

	if (zIdx < 0)
	{
		zIdx = 0;
	}

	int	iIndex = zIdx * (pInfo->iSizeX + 1) + xIdx;

	float	fLandScapeY[4] = {};
	fLandScapeY[0] = (*pvecPos)[iIndex].y;
	fLandScapeY[1] = (*pvecPos)[iIndex + 1].y;
	fLandScapeY[2] = (*pvecPos)[iIndex + (pInfo->iSizeX + 1)].y;
	fLandScapeY[3] = (*pvecPos)[iIndex + (pInfo->iSizeX + 1) + 1].y;

	float	fX, fY;
	fX = (vPos.x - (*pvecPos)[iIndex].x) / fWidth;
	fY = (vPos.z - (*pvecPos)[iIndex].z) / fHeight;

	delete pvecPos;
	SAFE_RELEASE(pTransform);

	// 우하단 삼각형에 존재할 경우
	if (fX >= fY)
		return fLandScapeY[0] + (fLandScapeY[1] - fLandScapeY[0]) * fX + (fLandScapeY[3] - fLandScapeY[1]) * fY;

	// 좌상단
	return fLandScapeY[0] + (fLandScapeY[3] - fLandScapeY[2]) * fX + (fLandScapeY[2] - fLandScapeY[0]) * fY;
}

void CQuadTreeManager::CheckAndAddChild(CGameObject * pGameObject)
{
	map<string, PQUADTREEINFO>::iterator iter;
	map<string, PQUADTREEINFO>::iterator iterEnd = m_mapQuadTreeInfo.end();

	Vector3 vMin, vMax;

	CTransform *pTr = pGameObject->GetTransform();
	Vector3 vPos = pTr->GetWorldPos();

	for (iter = m_mapQuadTreeInfo.begin(); iter != iterEnd; ++iter)
	{
		vMin = iter->second->vMin;
		vMax = iter->second->vMax;

		if ((vPos.x >= vMin.x && vPos.z >= vMin.z) &&
			(vPos.x <= vMax.x && vPos.z <= vMax.z))
		{
			iter->second->pChildObjects.push_back(pGameObject);
			break;
		}		
	}

	SAFE_RELEASE(pTr);
}

void CQuadTreeManager::CheckRenderingChild()
{
	map<string, PQUADTREEINFO>::iterator iter;
	map<string, PQUADTREEINFO>::iterator iterEnd = m_mapQuadTreeInfo.end();

	for (iter = m_mapQuadTreeInfo.begin(); iter != iterEnd; ++iter)
	{
		if (!iter->second->bCullingCheck &&
			iter->second->pGameObject->GetCulling()) 
		{
			for (auto& object : iter->second->pChildObjects)
			{
				object->SetCulling(true);
			}
			iter->second->bCullingCheck = true;
		}
		else if (iter->second->bCullingCheck &&
			!iter->second->pGameObject->GetCulling())
		{
			for (auto& object : iter->second->pChildObjects)
			{
				object->SetCulling(false);
			}
			iter->second->bCullingCheck = false;
		}
	}
}
 
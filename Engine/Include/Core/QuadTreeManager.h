#pragma once

#include "../PGEngine.h"


PG_BEGIN

typedef struct _tagQuadTreeInfo
{
	Vector3 vMin;
	Vector3 vMax;
	int iSizeX;
	int iSizeY;
	class CGameObject* pGameObject;
}QUADTREEINFO, *PQUADTREEINFO;

class PG_DLL CQuadTreeManager
{
private:
	unordered_map<string, PQUADTREEINFO> m_mapQuadTreeInfo;
	
public:
	void AddQuadTreeInfo(const string& key, int numX, int numY,
		Vector3 min, Vector3 max,
		class CGameObject* object);
	void DeleteQuadTreeInfo(class CScene* scene);
	PQUADTREEINFO FindQuadTreeInfo(const string& key);
	PQUADTREEINFO FindQuadTreeInfo(const Vector3& pos);
	float GetY(const Vector3& vPos);


	DECLARE_SINGLE(CQuadTreeManager)
};

PG_END
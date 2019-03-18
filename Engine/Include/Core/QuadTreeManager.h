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
public:
	CQuadTreeManager();
	~CQuadTreeManager();
};

PG_END
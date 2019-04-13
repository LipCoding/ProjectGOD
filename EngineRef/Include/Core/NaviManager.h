#pragma once

#include "../PGEngine.h"

PG_BEGIN

class PG_DLL CNaviManager
{
private:
	unordered_map<string, class CNaviMesh*> m_mapNaviMesh;

public:
	class CNaviMesh * CreateNaviMesh(const string& strKey);
	class CNaviMesh* FindNaviMesh(const string& strKey);
	class CNaviMesh* FindNaviMesh(const Vector3& vPos);


	DECLARE_SINGLE(CNaviManager);
};

PG_END
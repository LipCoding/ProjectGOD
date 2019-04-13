#pragma once

#include "../PGEngine.h"

PG_BEGIN

class PG_DLL CNaviManager
{
private:
	unordered_map<string, class CNaviMesh*> m_mapNaviMesh;
	
	bool m_bRenderCheck = false;
	string m_curMeshName = "";
public:
	void SetRenderCheck(bool check)
	{
		m_bRenderCheck = check;
	}

public:
	class CNaviMesh * CreateNaviMesh(const string& strKey);
	class CNaviMesh* FindNaviMesh(const string& strKey);
	class CNaviMesh* FindNaviMesh(const Vector3& vPos);

public:
	void Render(float fTime);

	DECLARE_SINGLE(CNaviManager);
};

PG_END
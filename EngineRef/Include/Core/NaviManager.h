#pragma once

#include "../PGEngine.h"

PG_BEGIN

class PG_DLL CNaviManager
{	
public:
	const vector<class CCell*>* GetNaviCells(const string& strKey = "");
	void SetRenderCheck(bool check)
	{
		m_bRenderCheck = check;
	}

public:
	// 현재 맵의 네비메쉬는 직접 Set이 가능하다.
	void SetCurNaviName(const string& strName) { m_curNaviName = strName; }
	const string& GetCurNaviName() { return m_curNaviName; }

public:
	float GetY(const Vector3& vPos, const string& strKey = "");
	bool  IsCellEmpty(const string& strKey = "");
public:
	// strKey가 디폴트일 경우 m_curNaviMesh의 이름으로 Navi를 찾는다.

	/* Default */
	class CNaviMesh * CreateNaviMesh(const string& strKey = "");

	/* Load */
	class CNaviMesh *CreateNaviMeshFromFile(const string& fileName);
	class CNaviMesh *FindNaviMesh(const string& strKey = "");

	/* Add Cell, */
	void AddCell(const vector<Vector3>& vPoint, const string& strKey = "");
	void AddCell(const Vector3* vPoint, const string& strKey = "");

	/* Erase */
	void EraseCell(const CCell* searchCell, const string& strKey = "");
	void UndoCell(const string& strKey = "");

	/* Check Navi Index */
	bool CheckPosition(const Vector3& vPos, Vector3* vDir, const string& strKey = "");
	void FreeNaviMesh(const string& strKey = "");

public:
	void Render(float fTime);

private:
	//class CNaviMesh* m_pCurrentNaviMesh = nullptr;
	bool m_bRenderCheck = false;

	/* Dummy */
private:
	unordered_map<string, class CNaviMesh*> m_mapNaviMesh;

	// 현재 맵의 네비메쉬를 나타낸다
	string m_curNaviName = "Default";

	DECLARE_SINGLE(CNaviManager);
};

PG_END
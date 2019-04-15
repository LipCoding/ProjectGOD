#pragma once

#include "../PGEngine.h"

PG_BEGIN

class PG_DLL CNaviManager
{	
public:
	const vector<class CCell*>* GetNaviCells();
	bool IsCellEmpty();

	void SetRenderCheck(bool check)
	{
		m_bRenderCheck = check;
	}

public:
	/* Default */
	class CNaviMesh * CreateNaviMesh();
	/* Load */
	class CNaviMesh * CreateNaviMesh(const string& filePath);
	
	/* Add Cell, */
	void AddCell(const vector<Vector3>& vPoint);
	void AddCell(const Vector3* vPoint);

	/* Erase */
	void EraseCell(const CCell* searchCell);
	void UndoCell();

	void FreeNaviMesh();

public:
	void Render(float fTime);

private:
	class CNaviMesh* m_pCurrentNaviMesh = nullptr;
	bool m_bRenderCheck = false;

	/* Dummy */
//private:
	//unordered_map<string, class CNaviMesh*> m_mapNaviMesh;
	//string m_curMeshName = "";

	/*class CNaviMesh * CreateNaviMesh(const string& strKey);
	class CNaviMesh* FindNaviMesh(const string& strKey);
	class CNaviMesh* FindNaviMesh(const Vector3& vPos);*/

	DECLARE_SINGLE(CNaviManager);
};

PG_END
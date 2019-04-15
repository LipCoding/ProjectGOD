#pragma once

#include "../PGEngine.h"

PG_BEGIN

class PG_DLL CNaviManager
{
private:
	//unordered_map<string, class CNaviMesh*> m_mapNaviMesh;
	//string m_curMeshName = "";
	
	class CNaviMesh* m_pCurrentMesh = nullptr;
	bool m_bRenderCheck = false;
	
public:
	const vector<class CCell*>* GetNaviCells();
	bool IsCellEmpty();

	void SetRenderCheck(bool check)
	{
		m_bRenderCheck = check;
	}

public:
	/*class CNaviMesh * CreateNaviMesh(const string& strKey);
	class CNaviMesh* FindNaviMesh(const string& strKey);
	class CNaviMesh* FindNaviMesh(const Vector3& vPos);*/

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

	DECLARE_SINGLE(CNaviManager);
};

PG_END
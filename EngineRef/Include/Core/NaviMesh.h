#pragma once
#include "../Component/Cell.h"

PG_BEGIN

class PG_DLL CNaviMesh
{
public:
	CNaviMesh();
	~CNaviMesh();

private:
	vector<CCell*> m_Cells;
	class CRenderState*	m_pDepthDisable;

public:
	void AddCell(const vector<NAVIPOINT>& points);

public:
	bool Init();
	int  Update();
	void Render();
};

PG_END
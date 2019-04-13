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

public:
	bool Init();
};

PG_END
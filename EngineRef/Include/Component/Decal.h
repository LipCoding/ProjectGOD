#pragma once
#include "Component.h"

PG_BEGIN

class PG_DLL CDecal :
	public CComponent
{
private:
	friend class CGameObject;

private:
	CDecal();
	CDecal(const CDecal& decal);
	~CDecal();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CDecal* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

PG_END

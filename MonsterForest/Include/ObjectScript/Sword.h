#pragma once

#include "Component/Script.h"

PG_USING

class CSword :
	public CScript
{
public:
	CSword();
	CSword(const CSword& gun);
	~CSword();

private:
	Matrix* m_pBoneMatrix = nullptr;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual CSword* Clone();
};


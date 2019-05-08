#pragma once

#include "Component/Script.h"
#include "GameObject/GameObject.h"

PG_USING

class CSword :
	public CScript
{
public:
	CSword();
	CSword(const CSword& sword);
	~CSword();

private:
	int targetPlayerID = -1;
	Matrix* m_pBoneMatrix = nullptr;
	class CTransform* m_pParentTr = nullptr;

public:
	int getTargetPlayerID() { return targetPlayerID; }
	void setTargetPlayerID(int id) { targetPlayerID = id; }
public:
	virtual bool Init() { return true; }
	virtual bool initialize();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual CSword* Clone();
};


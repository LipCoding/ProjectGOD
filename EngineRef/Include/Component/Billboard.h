#pragma once
#include "Component.h"

PG_BEGIN

class PG_DLL CBillboard : 
	public CComponent
{
private:
	friend class CGameObject;

private:
	CBillboard();
	CBillboard(const CBillboard& billboard);
	~CBillboard();

public:
	bool GetOperateBillboard() { return m_isOperate; }
	void SetOperateBillboard(bool check) { m_isOperate = check; }

private:
	class CTransform    *m_pObjTr = nullptr;
	class CCamera		*m_pCam = nullptr;
	class CTransform    *m_pCameraTr = nullptr;
	bool  m_isOperate = true;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CBillboard* Clone();
};

PG_END
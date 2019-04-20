#pragma once
#include "Component.h"

PG_BEGIN

class PG_DLL CMouse :
	public CComponent
{
private:
	friend class CGameObject;

private:
	CMouse();
	CMouse(const CMouse& mouse);
	~CMouse();

private:
	BOOL	m_bMouseShow = false;
	Vector3	m_vRayPos;
	Vector3	m_vRayDir;

public:
	Vector3 GetRayPos()	const;
	Vector3 GetRayDir()	const;

	void SetMouseShow(bool bShowCheck);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CMouse* Clone();
};

PG_END

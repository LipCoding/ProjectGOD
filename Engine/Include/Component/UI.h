#pragma once
#include "Component.h"

PG_BEGIN

class PG_DLL CUI :
	public CComponent
{
protected:
	friend class CGameObject;

protected:
	CUI();
	CUI(const CUI& ui);
	~CUI() = 0;

protected:
	int			m_iZOrder;

public:
	void SetZOrder(int iZOrder);
	int GetZOrder()	const;

public:
	virtual bool Init() = 0;
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CUI* Clone() = 0;
};

PG_END

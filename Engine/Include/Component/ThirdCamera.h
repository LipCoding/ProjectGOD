#pragma once
#include "Component.h"

PG_BEGIN

class PG_DLL CThirdCamera :
	public CComponent
{
private:
	friend class CGameObject;

private:
	CThirdCamera();
	CThirdCamera(const CThirdCamera& camera);
	~CThirdCamera();

private:
	int		m_iMoveDir = 0;
	float	m_fZoomSpeed = 0;
	class CArm*	m_pArm;
	int		m_iDragCount = 0;

private:
	void Zoom();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CThirdCamera* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

PG_END

#pragma once

#include "Component/Script.h"

PG_USING

class CGun	:
	public CScript
{
public:
	CGun();
	CGun(const CGun& gun);
	~CGun();

private:
	float	m_fOffsetAngle[AXIS_END];
	Vector3	m_vOffset;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual CGun* Clone();
};


#pragma once
#include "Component/Script.h"
#include "GameObject/GameObject.h"

PG_USING

class CBrushTool	:
	public CScript
{
public:
	CBrushTool();
	CBrushTool(const CBrushTool& brushtool);
	~CBrushTool();

private:
	float m_fRange = 0.f;
	float m_fUpSpeed = 0.f;
public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual CBrushTool* Clone();
};


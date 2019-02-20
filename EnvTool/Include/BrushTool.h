#pragma once
#include "Component/Script.h"
#include "GameObject/GameObject.h"

PG_USING

class CBrushTool :
	public CScript
{
public:
	CBrushTool();
	CBrushTool(const CBrushTool& brushtool);
	~CBrushTool();

public:
	float GetBrushRange() { return m_fRange; }

	void SetBrushCheck(bool check);
	void SetBrushInformation(float range);
	void SetBrushInformation(Vector4 color);
	void SetBrushInformation(Vector3 mousePos);
	
	void SetSpeed(float Speed) { m_fUpSpeed = Speed; }
public:
	void MoveHeight(vector<VERTEXBUMP>* pVtx , Vector3 mousePos,const float& fTime);

private:
	bool  m_bBrushCheck = false;
	float m_fRange = 1.f;
	float m_fUpSpeed = 10.f;
	Vector4 m_vColor = Vector4::Red;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual CScript* Clone();
};


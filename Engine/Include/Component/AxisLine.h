#pragma once
#include "Component.h"

PG_BEGIN

class PG_DLL CAxisLine :
	public CComponent
{
public:
	CAxisLine();
	CAxisLine(const CAxisLine& axisLine);
	virtual ~CAxisLine();

private:
	class CMesh*		m_pMesh;
	class CShader*		m_pShader;

	ID3D11InputLayout*	m_pLayout;
	TRANSFORMCBUFFER	m_tTransform;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CAxisLine* Clone();

};

PG_END

#pragma once
#include "Component.h"

PG_BEGIN

class PG_DLL CSphere :
	public CComponent
{
public:
	CSphere();
	CSphere(const CSphere& sphere);
	virtual ~CSphere();

private:
	class CMesh*	m_pMeshSphere;
	class CShader*  m_pShaderSphere;

	Matrix*			m_matObjWorld;
	ID3D11InputLayout*	m_pLayout;
	TRANSFORMCBUFFER	m_tTransform;
	bool			m_bRenderCheck;

public:
	void SetObjMatrix(Matrix *matrix);
	void SetRenderCheck(bool check);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual void SphereRender(float fTime);
	virtual CSphere* Clone();
};

PG_END
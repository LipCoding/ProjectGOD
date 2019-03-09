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

	Matrix				m_matObjWorld;
	Vector3				m_vecScale;
	Vector3				m_vecRot;
	Vector3				m_vecPos;
	Matrix*				m_matBoneWorld;

	ID3D11InputLayout*	m_pLayout;
	TRANSFORMCBUFFER	m_tTransform;

	bool				m_bRednerCheck;
public:
	void SetObjVector(Vector3 scale, Vector3 rot, Vector3 pos);
	void SetObjMatrix(Matrix matrix);
	void SetBoneMatrix(Matrix* matrix);
	void SetRenderCheck(bool check);
public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CAxisLine* Clone();

};

PG_END
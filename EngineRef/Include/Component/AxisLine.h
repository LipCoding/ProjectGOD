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
	//class CMesh*		m_pMeshSphere;
	class CMesh*		m_pMeshLine;
	class CShader*		m_pShaderSphere;

	Matrix				m_matObjWorld;
	Matrix*				m_matBoneWorld;
	ID3D11InputLayout*	m_pLayout;
	TRANSFORMCBUFFER	m_tTransform;
	bool				m_bRenderCheck;

private:
	/* font */
	IDWriteTextFormat*			m_pFont[3];
	ID2D1SolidColorBrush*		m_pBrush[3];
	RECTINFO					m_tArea[3];
	wstring						m_strText[3];
	Vector3						m_vecPosFont[3];

public:
	void SetObjMatrix(Matrix matrix);
	void SetBoneMatrix(Matrix* matrix);
	void SetRenderCheck(bool check);
public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual void AxisRender(float fTime);
	virtual CAxisLine* Clone();

	void RenderFont();
};

PG_END
#pragma once
#include "Component.h"

PG_BEGIN

class PG_DLL CLight :
	public CComponent
{
protected:
	friend class CGameObject;

protected:
	CLight();
	CLight(const CLight& light);
	~CLight() = 0;

protected:
	LIGHT	m_tInfo;

public:
	virtual bool Init() = 0;
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CLight* Clone() = 0;

public:
	void SetLightColor(const Vector4& vDif,
		const Vector4& vAmb = Vector4{ 1.f, 1.f, 1.f, 1.f }
		, const Vector4& vSpc = Vector4::White);
	void SetLightRange(float fRange);
	void SetLightInAngle(float fInAngle);
	void SetLightOutAngle(float fOutAngle);
	void SetLightIntensityPercent(float fPercent);
	void SetShader();

	const Vector4& GetLightColorDiffuse();
};

PG_END

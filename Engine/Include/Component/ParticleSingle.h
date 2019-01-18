#pragma once
#include "Component.h"

PG_BEGIN

class PG_DLL CParticleSingle :
	public CComponent
{
private:
	friend class CGameObject;

private:
	CParticleSingle();
	CParticleSingle(const CParticleSingle& particle);
	~CParticleSingle();

private:
	class CRenderer*	m_pRenderer;
	Vector2				m_vSize;
	float				m_vAlpha;
	float				m_Angle;
public:
	void SetSize(const Vector2& vSize);
	void SetSize(float x, float y);
	void SetAlpha(float alpha)
	{
		m_vAlpha = alpha;
	}
	void SetAngle(float angle)
	{
		m_Angle = angle;
	}
	const Vector2& GetSize() { return m_vSize; }
	class CRenderer* GetRenderer() { return m_pRenderer; }
	const float& GetAlpha() { return m_vAlpha; }
	const float& GetAngle() { return m_Angle; }
public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CParticleSingle* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

PG_END

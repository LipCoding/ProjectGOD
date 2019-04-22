#pragma once
#include "Component.h"

PG_BEGIN

class PG_DLL CEffect : 
	public CComponent
{
private:
	friend class CGameObject;

public:
	CEffect();
	CEffect(const CEffect& effect);
	~CEffect();

public:
	virtual void SetEffect();
	virtual void SetTexture(const string& strFullPath);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CEffect* Clone();

public:
	/* Load Effect Mesh */
	bool LoadEffectMesh(const string& filePath, const string& fileName);
	bool LoadEffectLocalInfo(const string& filePath);
	bool CreateEffectCollider();

	/* Material */
	void SetEffectTexture(const string& name, const string& fullPath);

private:
	class CRenderer* m_pRenderer = nullptr;

	/* Fade */
	float m_vAlpha = 1.f;

	/* Angle */
	float m_Angle = 0.f;

	/* Info */
	Vector3 m_vAngle;
	Vector3 m_vScale;
	Vector3 m_vPos;
};

PG_END

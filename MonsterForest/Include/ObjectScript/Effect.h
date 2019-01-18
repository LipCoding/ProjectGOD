#pragma once

#include "Component/Script.h"
#include "Component/Renderer.h"
#include "Component/Navigation3D.h"
#include "Component/Animation2D.h"
#include "Component/ParticleSingle.h"
#include "GameObject/GameObject.h"

PG_USING

class Effect :
	public CScript
{
public:
	Effect();
	Effect(const Effect& effect);
	~Effect();

public:
	void SetParticleSingle(CParticleSingle* particle_single)
	{
		m_pParticleSingle = particle_single;
	}
	void SetRenderer(CRenderer* renderer)
	{
		m_pRenderer = renderer;
	}
	void SetObject(CGameObject* object)
	{
		m_pObject = object;
	}
private:
	CGameObject* m_pObject = nullptr;
	CRenderer* m_pRenderer = nullptr;
	CParticleSingle* m_pParticleSingle = nullptr;
	CAnimation2D* m_pAnimation = nullptr;
	CNavigation3D* m_pNavigation = nullptr;

	float m_limit_size = 17.55f;
	bool  m_limit_check = false;

	float m_acc_scale_y_origin = 70.f;
	float m_speed_scale_y_origin = 30.f;
	float m_acc_scale_y = m_acc_scale_y_origin;
	float m_speed_scale_y = m_speed_scale_y_origin;

	float m_acc_alpha_origin = 7.f;
	float m_speed_alpha_origin = 1.f;
	float m_acc_alpha = m_acc_alpha_origin;
	float m_speed_alpha = m_speed_alpha_origin;
	bool  m_key_check = false;
public:
	void Init_Other();
public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual Effect* Clone();
};


#pragma once
#include "Effect.h"

PG_BEGIN

class PG_DLL CEffectTexture :
	public CEffect
{
public:
	CEffectTexture();
	CEffectTexture(const CEffectTexture& effect);
	~CEffectTexture();

private:
	Vector2			m_vSize;

public:
	virtual void SetEffect();
	virtual void SetTexture(const string& strFullPath);

public:
	//virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CEffectTexture* Clone();
};

PG_END

/*
CLayer*	pParticleLayer = m_pScene->CreateLayer("ParticleLayer", 10);

	for (int i = 0; i < 5; ++i)
	{
		CGameObject*	pParticleObj = CGameObject::CreateObject(
			"ParticleObj", pParticleLayer);

		CTransform*	pParticleTr = pParticleObj->GetTransform();

		pParticleTr->SetWorldPos(1.f, 0.f, 2.f * i);

		SAFE_RELEASE(pParticleTr);

		CParticleSingle*	pParticleSingle = pParticleObj->AddComponent<CParticleSingle>("ParticleSingle");

		pParticleSingle->SetSize(3.f, 3.f);

		SAFE_RELEASE(pParticleSingle);

		CRenderer*	pParticleRenderer = pParticleObj->FindComponentFromType<CRenderer>(CT_RENDERER);

		//pMaterial = pParticleRenderer->GetMaterial();

		//pMaterial->SetDiffuseTex()

		//SAFE_RELEASE(pMaterial);

		pParticleRenderer->CreateCBuffer("Animation2D", 10, sizeof(ANIMATION2DBUFFER),
			SCT_VERTEX | SCT_PIXEL);

		CAnimation2D*	pParticleAnimation = pParticleObj->AddComponent<CAnimation2D>("ParticleAnimation");

		pParticleAnimation->SetRenderer2DEnable(false);

		vector<wstring>	vecExplosion;

		for (int i = 1; i <= 89; ++i)
		{
			wchar_t	strPath[MAX_PATH] = {};
			wsprintf(strPath, L"Explosion/Explosion%d.png", i);

			vecExplosion.push_back(strPath);
		}

		pParticleAnimation->CreateClip("Explosion", A2D_FRAME, A2DO_LOOP,
			89, 1, 89, 1, 0, 0.5f, 0, 0.f, "Explosion",
			&vecExplosion);

		SAFE_RELEASE(pParticleAnimation);

		pParticleRenderer->SetRenderState(ALPHA_BLEND);

		SAFE_RELEASE(pParticleRenderer);

		SAFE_RELEASE(pParticleObj);
	}
*/
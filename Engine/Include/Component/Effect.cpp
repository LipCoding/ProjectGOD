#include "Effect.h"
#include "Renderer.h"
#include "../GameObject/GameObject.h"
#include "../Scene/Scene.h"
#include "Transform.h"
#include "Material.h"

PG_USING

CEffect::CEffect()
{
	m_eType = CT_EFFECT;

	m_vAngle = Vector3{ 0.f, 0.f, 0.f };
	m_vScale = Vector3{ 0.f, 0.f, 0.f };
	m_vPos = Vector3{ 0.f, 0.f, 0.f };
}

CEffect::CEffect(const CEffect & effect) :
	CComponent(effect)
{
	m_pRenderer = FindComponentFromType<CRenderer>(CT_RENDERER);
}

CEffect::~CEffect()
{
	SAFE_RELEASE(m_pRenderer);
}

void CEffect::SetEffect()
{

}

void CEffect::SetTexture(const string & strFullPath)
{
}

bool CEffect::Init()
{
	m_pRenderer = m_pGameObject->AddComponent<CRenderer>("Effect");

	CMaterial *pMaterial = m_pRenderer->CreateMaterial();
	//pMaterial->SetDiffuseSampler(SAMPLER_LINEAR);
	SAFE_RELEASE(pMaterial);

	return true;
}

void CEffect::Input(float fTime)
{

}

int CEffect::Update(float fTime)
{
	return 0;
}

int CEffect::LateUpdate(float fTime)
{
	return 0;
}

void CEffect::Collision(float fTime)
{
}

void CEffect::Render(float fTime)
{
}

CEffect * CEffect::Clone()
{
	return new CEffect(*this);
}

#include "EffectTexture.h"

#include "../GameObject/GameObject.h"
#include "../Scene/Scene.h"
#include "Renderer.h"
#include "Transform.h"
#include "Material.h"

PG_USING

CEffectTexture::CEffectTexture()
{
	SetTag("EffectTexture");
	SetTypeName("CEffectTexture");
	SetTypeID<CEffectTexture>();
}

CEffectTexture::CEffectTexture(const CEffectTexture & effect) :
	CEffect(effect)
{
	
}

CEffectTexture::~CEffectTexture()
{
	
}

//bool CEffectTexture::Init()
//{
//	m_pRenderer = 
//
//	return true;
//}

void CEffectTexture::SetEffect()
{
}

void CEffectTexture::SetTexture(const string & strFullPath)
{
	CMaterial *pMaterial = m_pRenderer->GetMaterial();

	SAFE_RELEASE(pMaterial);
}

void CEffectTexture::Input(float fTime)
{
}

int CEffectTexture::Update(float fTime)
{
	return 0;
}

int CEffectTexture::LateUpdate(float fTime)
{
	return 0;
}

void CEffectTexture::Collision(float fTime)
{
}

void CEffectTexture::Render(float fTime)
{
}

CEffectTexture * CEffectTexture::Clone()
{
	return new CEffectTexture(*this);
}

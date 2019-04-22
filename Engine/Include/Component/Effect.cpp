#include "Effect.h"
#include "Renderer.h"
#include "../GameObject/GameObject.h"
#include "../Scene/Scene.h"
#include "Transform.h"
#include "Material.h"
#include "Renderer.h"
#include "../Resources/Mesh.h"
#include "ColliderSphere.h"

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
	m_pRenderer = m_pGameObject->AddComponent<CRenderer>("Renderer");
	CMaterial *pMaterial = m_pRenderer->CreateMaterial();
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

bool CEffect::LoadEffectMesh(const string & filePath, const string & fileName)
{
	string tag = filePath + ".msh";
	wstring wTag;

	wTag.assign(tag.begin(), tag.end());

	if (!m_pGameObject)
		return false;

	m_pRenderer->SetMeshFromFullPath(fileName, wTag.c_str());
	
	CMesh *pMesh = m_pRenderer->GetMesh();

	if (!pMesh)
		return false;

	SAFE_RELEASE(pMesh);

	return true;
}

bool CEffect::LoadEffectLocalInfo(const string & filePath)
{
	string tag = filePath + ".dat";

	if (!m_pGameObject)
		return false;

	FILE* pFile = nullptr;

	fopen_s(&pFile, tag.c_str(), "rb");

	if (!pFile)
		return false;

	CTransform* pTr = m_pGameObject->GetTransform();
	pTr->Load_Local(pFile);
	SAFE_RELEASE(pTr);
	fclose(pFile);

	return true;
}

bool CEffect::CreateEffectCollider()
{
	CMesh *pMesh = m_pRenderer->GetMesh();

	if (!pMesh)
		return false;

	/* Collider */
	CTransform *pTr = m_pGameObject->GetTransform();
	pTr->SetWorldPos(50.f / 2.f, 0.f, 50.f / 2.f);

	Vector3 vMin, vMax, vCenter;
	vMin = (pMesh->GetMin()).TransformCoord(pTr->GetLocalMatrix().mat);
	vMax = (pMesh->GetMax()).TransformCoord(pTr->GetLocalMatrix().mat);
	vCenter = (pMesh->GetCenter()).TransformCoord(pTr->GetLocalMatrix().mat);

	float fRadius;
	fRadius = pMesh->GetRadius() * pTr->GetLocalScale().x;

	CColliderSphere* pCollider = m_pGameObject->AddComponent<CColliderSphere>("Collider");
	pCollider->SetSphere(vCenter, fRadius);
	pCollider->SetColliderRenderCheck(true);
	SAFE_RELEASE(pCollider);

	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pMesh);

	return true;
}

void CEffect::SetEffectTexture(const string & name, const string & fullPath)
{
	m_pRenderer->SetRenderState(CULLING_NONE);
	m_pRenderer->SetRenderState(ALPHA_BLEND);
	m_pRenderer->AlphaEnable(true);

	CMaterial *pMaterial = m_pRenderer->GetMaterial();
	pMaterial->SetDiffuseTexInfoFromFullPath(SAMPLER_LINEAR, name, 0, 0, fullPath.c_str());
	SAFE_RELEASE(pMaterial);
}

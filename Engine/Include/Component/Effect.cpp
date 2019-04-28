#include "Effect.h"
#include "Renderer.h"
#include "../GameObject/GameObject.h"
#include "../Scene/Scene.h"
#include "Transform.h"
#include "Material.h"
#include "Renderer.h"
#include "../Resources/Mesh.h"
#include "ColliderSphere.h"

#include "../Core/Timer.h"
#include "../Core/TimerManager.h"

PG_USING

CEffect::CEffect()
{
	m_eType = CT_EFFECT;
}

CEffect::CEffect(const CEffect & effect) :
	CComponent(effect)
{
	m_pRenderer = FindComponentFromType<CRenderer>(CT_RENDERER);
}

CEffect::~CEffect()
{
	SAFE_RELEASE(m_pRenderer);
	Safe_Delete_VecList(m_vecAssist);
}

CEffectAssist * CEffect::GetAssistFromType(CEffectAssist::ASSIST_TYPE type)
{
	for (auto& assist : m_vecAssist)
	{
		if (assist->GetType() == type)
		{
			return assist;
		}
	}

	return nullptr;
}

void CEffect::SetOperationCheck(bool check)
{
	m_OperationCheck = check;

	if (m_OperationCheck == false)
	{
		for (auto& assist : m_vecAssist)
		{
			assist->SetStartCheck(false);
			assist->Update(m_pGameObject, 0.f);
		}
	}
	else
	{
		for (auto& assist : m_vecAssist)
		{
			assist->SetStartCheck(true);
		}
	}
}

void CEffect::SetOperationCheckPart(CEffectAssist::ASSIST_TYPE type, bool check)
{
	for (auto& assist : m_vecAssist)
	{
		if (assist->GetType() == type)
		{
			assist->SetStartCheck(check);
			break;
		}
	}
}

bool CEffect::Init()
{
	m_pRenderer = m_pGameObject->AddComponent<CRenderer>("Renderer");
	
	/* Alpha용 상수 버퍼 */
	m_pRenderer->CreateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_PIXEL);

	SHARECBUFFER tShareBuffer = {};
	tShareBuffer.fAlphaFadeIn = 0.f;
	tShareBuffer.fAlphaFadeOut = 0.f;
	tShareBuffer.vColor = Vector4{ 0.f, 0.f, 0.f, 0.f };
	
	m_pRenderer->UpdateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_PIXEL, &tShareBuffer);

	CMaterial *pMaterial = m_pRenderer->CreateMaterial();
	SAFE_RELEASE(pMaterial);

	return true;
}

void CEffect::Input(float fTime)
{
}

int CEffect::Update(float fTime)
{
	if (m_OperationCheck == false)
	{
		m_Timer = 0.f;
	}
	else
	{
		m_Timer += fTime;

		if (m_Timer >= m_MainEndTime)
		{
			for (auto& assist : m_vecAssist)
			{
				assist->SetStartCheck(false);
			}

			SHARECBUFFER tShareBuffer = {};
			tShareBuffer.fAlphaFadeIn = 0.f;
			tShareBuffer.fAlphaFadeOut = 0.f;
			tShareBuffer.vColor = Vector4{ 0.f, 0.f, 0.f, 0.f };

			m_pRenderer->UpdateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_PIXEL, &tShareBuffer);

			m_Timer = 0.f;
			m_OperationCheck = false;
		}
	}

	for (auto& assist : m_vecAssist)
	{
		assist->Update(m_pGameObject, fTime);
	}

	return 0;
}

int CEffect::LateUpdate(float fTime)
{
	/*  */

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
	//pTr->SetWorldPos(50.f / 2.f, 0.f, 50.f / 2.f);

	Vector3 vMin, vMax, vCenter;
	vMin = (pMesh->GetMin()).TransformCoord(pTr->GetLocalMatrix().mat);
	vMax = (pMesh->GetMax()).TransformCoord(pTr->GetLocalMatrix().mat);
	vCenter = (pMesh->GetCenter()).TransformCoord(pTr->GetLocalMatrix().mat);

	float fRadius;
	fRadius = pMesh->GetRadius() * pTr->GetLocalScale().x;

	CColliderSphere* pCollider = m_pGameObject->AddComponent<CColliderSphere>("Collider");
	pCollider->SetSphere(vCenter, fRadius);
	pCollider->SetColliderRenderCheck(false);
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

void CEffect::AddPatternScale(const int& easeType, const float & start, const float & end, const float & powX, const float & powY, const float & powZ, const int & repeat)
{
	CEffectAssist *pAssistData = nullptr;

	for (auto& assist : m_vecAssist)
	{
		if (assist->GetType() == CEffectAssist::ASSIST_SCALE)
		{
			pAssistData = assist;
			pAssistData->SetStartTime(start);
			pAssistData->SetEndTime(end);
			pAssistData->SetPowerX(powX);
			pAssistData->SetPowerY(powY);
			pAssistData->SetPowerZ(powZ);
			pAssistData->SetRepeat(repeat);
			pAssistData->Init(m_pGameObject, CEffectAssist::ASSIST_SCALE, (CEffectAssist::EASE_TYPE)easeType);
			return;
		}
	}

	if (pAssistData == nullptr)
	{
		pAssistData = new CEffectAssist;
		pAssistData->SetStartTime(start);
		pAssistData->SetEndTime(end);
		pAssistData->SetPowerX(powX);
		pAssistData->SetPowerY(powY);
		pAssistData->SetPowerZ(powZ);
		pAssistData->SetRepeat(repeat);
	}

	pAssistData->Init(m_pGameObject, CEffectAssist::ASSIST_SCALE, (CEffectAssist::EASE_TYPE)easeType);
	m_vecAssist.push_back(pAssistData);
}

void CEffect::AddPatternRotation(const int& easeType, const float & start, const float & end, const float & powX, const float & powY, const float & powZ, const int & repeat)
{
	CEffectAssist *pAssistData = nullptr;

	for (auto& assist : m_vecAssist)
	{
		if (assist->GetType() == CEffectAssist::ASSIST_ROT)
		{
			pAssistData = assist;
			pAssistData->SetStartTime(start);
			pAssistData->SetEndTime(end);
			pAssistData->SetPowerX(powX);
			pAssistData->SetPowerY(powY);
			pAssistData->SetPowerZ(powZ);
			pAssistData->SetRepeat(repeat);
			pAssistData->Init(m_pGameObject, CEffectAssist::ASSIST_ROT, (CEffectAssist::EASE_TYPE)easeType);
			return;
		}
	}

	if (pAssistData == nullptr)
	{
		pAssistData = new CEffectAssist;
		pAssistData->SetStartTime(start);
		pAssistData->SetEndTime(end);
		pAssistData->SetPowerX(powX);
		pAssistData->SetPowerY(powY);
		pAssistData->SetPowerZ(powZ);
		pAssistData->SetRepeat(repeat);
	}

	pAssistData->Init(m_pGameObject, CEffectAssist::ASSIST_ROT, (CEffectAssist::EASE_TYPE)easeType);
	m_vecAssist.push_back(pAssistData);
}

void CEffect::AddFadeIn(const float & start, const float & end, const float & degree)
{
	CEffectAssist *pAssistData = nullptr;

	for (auto& assist : m_vecAssist)
	{
		if (assist->GetType() == CEffectAssist::ASSIST_FADE_IN)
		{
			pAssistData = assist;
			pAssistData->SetStartTime(start);
			pAssistData->SetEndTime(end);
			pAssistData->SetDegree(degree);
			pAssistData->Init(m_pGameObject, CEffectAssist::ASSIST_FADE_IN, CEffectAssist::EASE_SINE_OUT);
			return;
		}
	}

	if (pAssistData == nullptr)
	{
		pAssistData = new CEffectAssist;
		pAssistData->SetStartTime(start);
		pAssistData->SetEndTime(end);
		pAssistData->SetDegree(degree);
	}

	pAssistData->Init(m_pGameObject, CEffectAssist::ASSIST_FADE_IN, CEffectAssist::EASE_SINE_OUT);
	m_vecAssist.push_back(pAssistData);
}

void CEffect::AddFadeOut(const float & start, const float & end, const float & degree)
{
	CEffectAssist *pAssistData = nullptr;

	for (auto& assist : m_vecAssist)
	{
		if (assist->GetType() == CEffectAssist::ASSIST_FADE_OUT)
		{
			pAssistData = assist;
			pAssistData->SetStartTime(start);
			pAssistData->SetEndTime(end);
			pAssistData->SetDegree(degree);
			pAssistData->Init(m_pGameObject, CEffectAssist::ASSIST_FADE_OUT, CEffectAssist::EASE_SINE_OUT);
			return;
		}
	}

	if (pAssistData == nullptr)
	{
		pAssistData = new CEffectAssist;
		pAssistData->SetStartTime(start);
		pAssistData->SetEndTime(end);
		pAssistData->SetDegree(degree);
	}

	pAssistData->Init(m_pGameObject, CEffectAssist::ASSIST_FADE_OUT, CEffectAssist::EASE_SINE_OUT);
	m_vecAssist.push_back(pAssistData);
}

void CEffect::DeleteAssistEffectFromType(CEffectAssist::ASSIST_TYPE type)
{
	auto& assistEffect = remove_if(m_vecAssist.begin(), m_vecAssist.end(),
		[&](CEffectAssist* assist) {return type == assist->GetType(); });

	if (assistEffect == m_vecAssist.end())
		return;

	m_vecAssist.erase(assistEffect, m_vecAssist.end());
}

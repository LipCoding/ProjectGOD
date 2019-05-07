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

#include "../Component/Animation2D.h"
#include "../Core/PathManager.h"

PG_USING

CEffect::CEffect()
{
	m_eType = CT_EFFECT;
}

CEffect::CEffect(const CEffect & effect) :
	CComponent(effect)
{
	m_MainStartTime = effect.m_MainStartTime;
	m_MainEndTime = effect.m_MainEndTime;

	EffectName = effect.EffectName;
	FileName = effect.FileName;

	MeshPath = effect.MeshPath;
	LocalPath = effect.LocalPath;
	TextureFullPath = effect.TextureFullPath;
	TexturePath = effect.TexturePath;

	m_tshareBuffer = {};

	m_pRenderer = effect.m_pRenderer;
	effect.m_pRenderer->AddRef();

	for(const auto& assist : effect.m_vecAssist)
	{
		switch (assist->GetType())
		{
		case CEffectAssist::ASSIST_SCALE:
		{
			AddPatternScale(assist->GetEaseType(),
				assist->GetStartTime(), assist->GetEndTime(),
				assist->GetPowerX(), assist->GetPowerY(), assist->GetPowerZ(),
				assist->GetRepeat());
			break;
		}
		case CEffectAssist::ASSIST_ROT:
		{
			AddPatternRotation(assist->GetEaseType(),
				assist->GetStartTime(), assist->GetEndTime(),
				assist->GetPowerX(), assist->GetPowerY(), assist->GetPowerZ(),
				assist->GetRepeat());
			break;
		}
		case CEffectAssist::ASSIST_FADE_IN:
		{
			AddFadeIn(assist->GetStartTime(), assist->GetEndTime(), assist->GetDegree());
			break;
		}
		case CEffectAssist::ASSIST_FADE_OUT:
		{
			AddFadeOut(assist->GetStartTime(), assist->GetEndTime(), assist->GetDegree());
			break;
		}
		case CEffectAssist::ASSIST_UV_ANI:
		{
			AddUVAnimation(assist->GetStartTime(), assist->GetEndTime(),
				assist->GetNum(), assist->GetRepeat());
			break;
		}
		case CEffectAssist::ASSIST_UV_MOVE:
		{
			AddUVMovement(assist->GetStartTime(), assist->GetEndTime(),
				assist->GetMoveUV_X(), assist->GetMoveUV_Y());
			break;
		}
		default :
			break;
		}
	}
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

void CEffect::SetErase(bool check)
{
	m_EraseCheck = check;
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

	m_tshareBuffer = {};
	m_tshareBuffer.fAlphaFadeIn = 0.f;
	m_tshareBuffer.fAlphaFadeOut = 0.f;
	m_tshareBuffer.vColor = Vector4{ 0.f, 0.f, 0.f, 0.f };

	m_pRenderer->UpdateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_PIXEL, &m_tshareBuffer);

	/* Animation용 상수 버퍼 */
	m_pRenderer->CreateCBuffer("Animation2D", 10, sizeof(ANIMATION2DBUFFER),
		SCT_VERTEX | SCT_PIXEL);

	CMaterial *pMaterial = m_pRenderer->CreateMaterial();
	SAFE_RELEASE(pMaterial);

	return true;
}

void CEffect::Input(float fTime)
{
}

int CEffect::Update(float fTime)
{
	if (!m_pRenderer)
	{
		m_pRenderer = m_pGameObject->FindComponentFromType<CRenderer>(CT_RENDERER);
		m_pRenderer->UpdateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_PIXEL, &m_tshareBuffer);
	}

	if (m_OperationCheck == false)
	{
		m_Timer = 0.f;
	}
	else
	{
		m_Timer += fTime;

		if (m_Timer >= m_MainEndTime)
		{
			if (m_EraseCheck)
			{
				/* 이펙트가 끝나면 삭제한다. */
				m_pGameObject->Die();
				CGameObject::EraseObj(m_pGameObject);

				return 0;
			}
			else
			{
				for (auto& assist : m_vecAssist)
				{
					assist->SetStartCheck(false);
				}

				m_tshareBuffer.fAlphaFadeIn = 0.f;
				m_tshareBuffer.fAlphaFadeOut = 0.f;
				m_tshareBuffer.vColor = Vector4{ 0.f, 0.f, 0.f, 0.f };
				m_tshareBuffer.fMoveUV_X = 0.f;
				m_tshareBuffer.fMoveUV_Y = 0.f;

				m_pRenderer->UpdateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_PIXEL, &m_tshareBuffer);
			}

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
	MeshPath = tag;
	wstring wTag;

	wTag.assign(tag.begin(), tag.end());

	if (!m_pGameObject)
		return false;

	// 확장자명 제거
	for (int i = strlen(MeshPath.c_str()) - 1; i >= 0; i--)
	{
		if (MeshPath[i] == '.')
		{
			MeshPath.erase(i, strlen(MeshPath.c_str()) - 1);
			break;
		}
	}

	// 앞쪽 Mesh_Path 제거
	size_t meshPathSize = strlen(GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH)) - 1;
	MeshPath.erase(0, meshPathSize);

	m_pRenderer->SetMeshFromFullPath(fileName, wTag.c_str());
	
	CMesh *pMesh = m_pRenderer->GetMesh();

	/* Effect용 쉐이더로 변경 */
	m_pRenderer->SetShader(STANDARD_EFFECT_TEX_NORMAL_SHADER);

	if (!pMesh)
		return false;

	SAFE_RELEASE(pMesh);

	return true;
}

bool CEffect::LoadEffectLocalInfo(const string & filePath)
{
	string tag = filePath + ".dat";
	LocalPath = tag;

	if (!m_pGameObject)
		return false;

	// 확장자명 제거
	for (int i = strlen(LocalPath.c_str()) - 1; i >= 0; i--)
	{
		if (LocalPath[i] == '.')
		{
			LocalPath.erase(i, strlen(LocalPath.c_str()) - 1);
			break;
		}
	}

	// 앞쪽 Mesh_Path 제거
	size_t meshPathSize = strlen(GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH)) - 1;
	LocalPath.erase(0, meshPathSize);

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
	TextureFullPath = fullPath;
	TexturePath = TextureFullPath;

	// 확장자명 제거
	for (int i = strlen(TexturePath.c_str()) - 1; i >= 0; i--)
	{
		if (TexturePath[i] == '.')
		{
			TexturePath.erase(i, strlen(TexturePath.c_str()) - 1);
			break;
		}
	}

	// 앞쪽 Texture_Path 제거
	size_t texPathSize = strlen(GET_SINGLE(CPathManager)->FindPathToMultiByte(TEXTURE_PATH)) - 1;
	TexturePath.erase(0, texPathSize);
			
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
			pAssistData->SetShareBuffer(&m_tshareBuffer);
			return;
		}
	}

	if (pAssistData == nullptr)
	{
		pAssistData = new CEffectAssist;
		pAssistData->SetStartTime(start);
		pAssistData->SetEndTime(end);
		pAssistData->SetDegree(degree);
		pAssistData->SetShareBuffer(&m_tshareBuffer);
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
			pAssistData->SetShareBuffer(&m_tshareBuffer);
			return;
		}
	}

	if (pAssistData == nullptr)
	{
		pAssistData = new CEffectAssist;
		pAssistData->SetStartTime(start);
		pAssistData->SetEndTime(end);
		pAssistData->SetDegree(degree);
		pAssistData->SetShareBuffer(&m_tshareBuffer);
	}

	pAssistData->Init(m_pGameObject, CEffectAssist::ASSIST_FADE_OUT, CEffectAssist::EASE_SINE_OUT);
	m_vecAssist.push_back(pAssistData);
}

void CEffect::AddUVAnimation(const float & start, const float & end, const int & num, const int & repeat)
{
	CEffectAssist *pAssistData = nullptr;

	for (auto& assist : m_vecAssist)
	{
		if (assist->GetType() == CEffectAssist::ASSIST_UV_ANI)
		{
			pAssistData = assist;
			pAssistData->SetStartTime(start);
			pAssistData->SetEndTime(end);
			pAssistData->SetNum(num);
			pAssistData->Init(m_pGameObject, CEffectAssist::ASSIST_UV_ANI);
			pAssistData->SetShareBuffer(&m_tshareBuffer);
			return;
		}
	}

	if (pAssistData == nullptr)
	{
		pAssistData = new CEffectAssist;
		pAssistData->SetStartTime(start);
		pAssistData->SetEndTime(end);
		pAssistData->SetNum(num);
		pAssistData->SetShareBuffer(&m_tshareBuffer);
	}

	CAnimation2D*	pEffectAnimation = nullptr;

	pEffectAnimation = m_pGameObject->FindComponentFromTag<CAnimation2D>("EffectAnimation");

	if (!pEffectAnimation)
	{
		pEffectAnimation = m_pGameObject->AddComponent<CAnimation2D>("EffectAnimation");

		pEffectAnimation->SetRenderer2DEnable(false);

		wstring wPath;
		wPath.assign(TexturePath.begin(), TexturePath.end());
		wPath += L".png";

		wchar_t	strPath[MAX_PATH] = {};
		wsprintf(strPath, wPath.c_str());

		if (!pEffectAnimation->CreateClip("Default", A2D_FRAME, A2DO_LOOP,
			1, 1, 1, 1, 0, 0, 0, 0.f, "Default",
			strPath))
		{
			SAFE_RELEASE(pEffectAnimation);
			return;
		}
		pEffectAnimation->SetDefaultAnim("Default");

		/* Sprite */
		vector<wstring>	vecTextures;
		for (int i = 1; i <= num; ++i)
		{
			wstring wPath;
			wPath.assign(TexturePath.begin(), TexturePath.end());

			wPath += L"%d.png";

			wchar_t	strPath[MAX_PATH] = {};
			wsprintf(strPath, wPath.c_str(), i);

			vecTextures.push_back(strPath);
		}
		if (!pEffectAnimation->CreateClip("Effect", A2D_FRAME, A2DO_TIME_RETURN,
			num, 1, num, 1, 0, end - start, 0, 0.f, "Effect",
			&vecTextures))
		{
			SAFE_RELEASE(pEffectAnimation);
			return;
		}
		SAFE_RELEASE(pEffectAnimation);
	}

	pAssistData->Init(m_pGameObject, CEffectAssist::ASSIST_UV_ANI);
	m_vecAssist.push_back(pAssistData);
	/*  */
}

void CEffect::AddUVMovement(const float & start, const float & end, const float & moveX, const float & moveY)
{
	CEffectAssist *pAssistData = nullptr;

	for (auto& assist : m_vecAssist)
	{
		if (assist->GetType() == CEffectAssist::ASSIST_UV_MOVE)
		{
			pAssistData = assist;
			pAssistData->SetStartTime(start);
			pAssistData->SetEndTime(end);
			pAssistData->SetMoveUV_X(moveX);
			pAssistData->SetMoveUV_Y(moveY);
			pAssistData->Init(m_pGameObject, CEffectAssist::ASSIST_UV_MOVE);
			pAssistData->SetShareBuffer(&m_tshareBuffer);
			return;
		}
	}

	if (pAssistData == nullptr)
	{
		pAssistData = new CEffectAssist;
		pAssistData->SetStartTime(start);
		pAssistData->SetEndTime(end);
		pAssistData->SetMoveUV_X(moveX);
		pAssistData->SetMoveUV_Y(moveY);
		pAssistData->SetShareBuffer(&m_tshareBuffer);
	}

	pAssistData->Init(m_pGameObject, CEffectAssist::ASSIST_UV_MOVE);
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

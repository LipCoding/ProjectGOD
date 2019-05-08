#include "RenderManager.h"
#include "ShaderManager.h"
#include "Shader.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"
#include "../GameObject/GameObject.h"
#include "../Component/Renderer.h"
#include "../Component/Renderer2D.h"
#include "../Component/UI.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "../Core/Input.h"
#include "../GameObject/GameObject.h"
#include "RenderingTarget.h"
#include "../Device.h"
#include "../Component/Light.h"
#include "../Resources/ResourcesManager.h"
#include "../Resources/Sampler.h"
#include "../Component/Camera.h"
#include "../Component/Transform.h"
#include "../Component/UI.h"
#include "../Component/Animation.h"
#include "../Component/AnimationClip.h"
#include "../Component/Material.h"
#include "../Resources/Mesh.h"
#include"../Component/Collider.h"
#include "../Component/ColliderRect.h"
#include "../Component/ColliderSphere.h"
#include "../Component/AxisLine.h"
#include "../Component/ColliderAABB.h"
PG_USING

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager()	:
	m_pLightAccShader(NULL),
	m_pLightBlendShader(NULL),
	m_pDeferredShader(NULL),
	m_pPointSmp(NULL)
{
}

CRenderManager::~CRenderManager()
{
	SAFE_RELEASE(m_pDeferredShader);
	SAFE_RELEASE(m_pLightBlendShader);
	SAFE_RELEASE(m_pDepthDisable);
	SAFE_RELEASE(m_pAccBlend);
	SAFE_RELEASE(m_pPointSmp);
	SAFE_RELEASE(m_pLightAccShader);
	Safe_Delete_Map(m_mapMRT);
	Safe_Release_Map(m_mapTarget);

	for (int i = 0; i < RGT_END; ++i)
	{
		SAFE_DELETE_ARRAY(m_tRenderGroup[i].pRenderObj);
	}

	Safe_Release_Map(m_mapRenderState);
	DESTROY_SINGLE(CShaderManager);
}

bool CRenderManager::Init()
{
	if (!GET_SINGLE(CShaderManager)->Init())
		return false;

	{
		AddBlendTargetDesc(false);

		CBlendState*	pState = CreateBlendState(ALPHA_BLEND_DISABLE, FALSE, FALSE);

		SAFE_RELEASE(pState);
	}

	AddBlendTargetDesc();

	CBlendState*	pState = CreateBlendState(ALPHA_BLEND, FALSE, FALSE);

	SAFE_RELEASE(pState); 

	AddBlendTargetDesc(true, D3D11_BLEND_ONE, D3D11_BLEND_ONE);
	pState = CreateBlendState(ONE_BLEND, FALSE, FALSE);

	SAFE_RELEASE(pState);

	CDepthStencilState*	pDepthDisable = CreateDepthStencilState(
		DEPTH_DISABLE, false);

	SAFE_RELEASE(pDepthDisable);

	pDepthDisable = CreateDepthStencilState(
		DEPTH_LESS_EQUAL, true, D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_LESS_EQUAL);

	SAFE_RELEASE(pDepthDisable);

	CRasterizerState*	pRS = CreateRasterizerState(WIRE_FRAME,
		D3D11_FILL_WIREFRAME);

	SAFE_RELEASE(pRS);

	pRS = CreateRasterizerState(CULLING_BACK, D3D11_FILL_SOLID,
		D3D11_CULL_BACK);

	SAFE_RELEASE(pRS);

	pRS = CreateRasterizerState(CULLING_NONE, D3D11_FILL_SOLID,
		D3D11_CULL_NONE);

	SAFE_RELEASE(pRS);

	CRenderingTarget* pTarget = CreateRenderTarget("Shadow",
		DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight,
		DXGI_FORMAT_R32G32B32A32_FLOAT, Vector4::Black, DXGI_FORMAT_D24_UNORM_S8_UINT);
	//DXGI_FORMAT_R8G8B8A8_UNORM
	//pTarget->SetDebugEnable(true);
	pTarget->SetDebugInfo(Vector3(0.f, 600.f, 0.f), Vector3(100.f, 100.f, 1.f));

	SAFE_RELEASE(pTarget);

	// 렌더링 타겟 생성
	pTarget = CreateRenderTarget("Albedo",
		DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight,
		DXGI_FORMAT_R32G32B32A32_FLOAT);

	//pTarget->SetDebugEnable(true);
	pTarget->SetDebugInfo(Vector3::Zero, Vector3(100.f, 100.f, 1.f));

	SAFE_RELEASE(pTarget);

	pTarget = CreateRenderTarget("Normal",
		DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight,
		DXGI_FORMAT_R32G32B32A32_FLOAT);

	//pTarget->SetDebugEnable(true);
	pTarget->SetDebugInfo(Vector3(0.f, 100.f, 0.f), Vector3(100.f, 100.f, 1.f));

	SAFE_RELEASE(pTarget);

	pTarget = CreateRenderTarget("Depth",
		DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight,
		DXGI_FORMAT_R32G32B32A32_FLOAT, Vector4(1.f, 1.f, 1.f, 0.f));

	//pTarget->SetDebugEnable(true);
	pTarget->SetDebugInfo(Vector3(0.f, 200.f, 0.f), Vector3(100.f, 100.f, 1.f));

	SAFE_RELEASE(pTarget);

	pTarget = CreateRenderTarget("Specular",
		DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight,
		DXGI_FORMAT_R32G32B32A32_FLOAT);

	//pTarget->SetDebugEnable(true);
	pTarget->SetDebugInfo(Vector3(0.f, 300.f, 0.f), Vector3(100.f, 100.f, 1.f));

	SAFE_RELEASE(pTarget);

	pTarget = CreateRenderTarget("Tangent",
		DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight,
		DXGI_FORMAT_R32G32B32A32_FLOAT);

	//pTarget->SetDebugEnable(true);
	pTarget->SetDebugInfo(Vector3(0.f, 400.f, 0.f), Vector3(100.f, 100.f, 1.f));

	SAFE_RELEASE(pTarget);

	pTarget = CreateRenderTarget("Binormal",
		DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight,
		DXGI_FORMAT_R32G32B32A32_FLOAT);

	//pTarget->SetDebugEnable(true);
	pTarget->SetDebugInfo(Vector3(0.f, 500.f, 0.f), Vector3(100.f, 100.f, 1.f));

	SAFE_RELEASE(pTarget);

	pTarget = CreateRenderTarget("ShadowDepth",
		DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight,
		DXGI_FORMAT_R32G32B32A32_FLOAT, Vector4(0.f, 0.f, 0.f, 0.f), DXGI_FORMAT_D24_UNORM_S8_UINT);

	//pTarget->SetDebugEnable(true); // 매쉬 찾고 , 셰이더 찾고 , 정점구조 찾고
	pTarget->SetDebugInfo(Vector3(500.f, 0.f, 0.f), Vector3(100.f, 100.f, 1.f)); // 위치랑 크기 넣고

	SAFE_RELEASE(pTarget);

	AddRenderTargetToMRT("GBuffer", "Albedo");
	AddRenderTargetToMRT("GBuffer", "Normal");
	AddRenderTargetToMRT("GBuffer", "Depth");
	AddRenderTargetToMRT("GBuffer", "Specular");
	AddRenderTargetToMRT("GBuffer", "Tangent");
	AddRenderTargetToMRT("GBuffer", "Binormal");
	//SetDepthToMRT("GBuffer", "Albedo");

	/*pTarget = CreateRenderTarget("DecalNormal",
		DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight,
		DXGI_FORMAT_R32G32B32A32_FLOAT);

	pTarget->SetDebugEnable(true);
	pTarget->SetDebugInfo(Vector3(300.f, 0.f, 0.f), Vector3(100.f, 100.f, 1.f));

	SAFE_RELEASE(pTarget);

	pTarget = CreateRenderTarget("DecalSpecular",
		DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight,
		DXGI_FORMAT_R32G32B32A32_FLOAT);

	pTarget->SetDebugEnable(true);
	pTarget->SetDebugInfo(Vector3(300.f, 100.f, 0.f), Vector3(100.f, 100.f, 1.f));

	SAFE_RELEASE(pTarget);*/

	AddRenderTargetToMRT("Decal", "Albedo");
	AddRenderTargetToMRT("Decal", "Normal");
	AddRenderTargetToMRT("Decal", "Specular");


	pTarget = CreateRenderTarget("LightDif",
		DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		Vector4::Zero, DXGI_FORMAT_D24_UNORM_S8_UINT);

	//pTarget->SetDebugEnable(true);
	pTarget->SetDebugInfo(Vector3(100.f, 0.f, 0.f), Vector3(100.f, 100.f, 1.f));

	SAFE_RELEASE(pTarget);

	pTarget = CreateRenderTarget("LightSpc",
		DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight,
		DXGI_FORMAT_R32G32B32A32_FLOAT);

	//pTarget->SetDebugEnable(true);
	pTarget->SetDebugInfo(Vector3(100.f, 100.f, 0.f), Vector3(100.f, 100.f, 1.f));

	SAFE_RELEASE(pTarget);

	AddRenderTargetToMRT("LightAcc", "LightDif");
	AddRenderTargetToMRT("LightAcc", "LightSpc");
	SetDepthToMRT("LightAcc", "LightDif");

	pTarget = CreateRenderTarget("LightBlend",
		DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		Vector4::Zero, DXGI_FORMAT_D24_UNORM_S8_UINT);

	//pTarget->SetDebugEnable(true);
	pTarget->SetDebugInfo(Vector3(200.f, 0.f, 0.f), Vector3(100.f, 100.f, 1.f));

	SAFE_RELEASE(pTarget);

	// 조명 누적 버퍼용 셰이더
	m_pLightAccShader = GET_SINGLE(CShaderManager)->FindShader(LIGHT_ACC_SHADER);
	m_pPointSmp = GET_SINGLE(CResourcesManager)->FindSampler(SAMPLER_POINT);
	m_pDepthDisable = FindRenderState(DEPTH_DISABLE);
	m_pAccBlend = FindRenderState(ONE_BLEND);

	m_pLightBlendShader = GET_SINGLE(CShaderManager)->FindShader(LIGHT_BLEND_SHADER);
	m_pDeferredShader = GET_SINGLE(CShaderManager)->FindShader(DEFERRED_SHADER);

	return true;
}

bool CRenderManager::AddBlendTargetDesc(bool bEnable, D3D11_BLEND eSrc, 
	D3D11_BLEND eDest, D3D11_BLEND_OP eOp, D3D11_BLEND eSrcAlpha, 
	D3D11_BLEND eDestAlpha, D3D11_BLEND_OP eAlphaOp, UINT8 iWriteMask)
{
	if (m_vecDesc.size() == 8)
		return false;

	D3D11_RENDER_TARGET_BLEND_DESC	tDesc = {};

	tDesc.BlendEnable = bEnable;
	tDesc.SrcBlend = eSrc;
	tDesc.DestBlend = eDest;
	tDesc.BlendOp = eOp;
	tDesc.SrcBlendAlpha = eSrcAlpha;
	tDesc.DestBlendAlpha = eDestAlpha;
	tDesc.BlendOpAlpha = eAlphaOp;
	tDesc.RenderTargetWriteMask = iWriteMask;

	m_vecDesc.push_back(tDesc);

	return true;
}

CBlendState * CRenderManager::CreateBlendState(const string & strKey, 
	bool bAlphaCoverage, bool bIndependent)
{
	CBlendState*	pState = (CBlendState*)FindRenderState(strKey);

	if (pState)
		return pState;

	pState = new CBlendState;

	for (size_t i = 0; i < m_vecDesc.size(); ++i)
	{
		pState->AddDesc(m_vecDesc[i]);
	}

	m_vecDesc.clear();

	if (!pState->CreateBlendState(strKey, bAlphaCoverage, bIndependent))
	{
		SAFE_RELEASE(pState);
		return NULL;
	}

	pState->SetTag(strKey);

	pState->AddRef();

	m_mapRenderState.insert(make_pair(strKey, pState));
	
	return pState;
}

CDepthStencilState * CRenderManager::CreateDepthStencilState(
	const string & strKey, bool bDepth,
	D3D11_DEPTH_WRITE_MASK eDepthWriteMask,
	D3D11_COMPARISON_FUNC eDepthFunc, bool bStencil,
	UINT8 iReadMask, UINT8 iWriteMask,
	D3D11_DEPTH_STENCILOP_DESC tFrontDesc,
	D3D11_DEPTH_STENCILOP_DESC tBackDesc)
{
	CDepthStencilState*	pState = (CDepthStencilState*)FindRenderState(strKey);

	if (pState)
		return pState;

	pState = new CDepthStencilState;

	if (!pState->CreateDepthStencilState(strKey, bDepth, eDepthWriteMask,
		eDepthFunc, bStencil, iReadMask, iWriteMask,
		tFrontDesc, tBackDesc))
	{
		SAFE_RELEASE(pState);
		return NULL;
	}

	pState->SetTag(strKey);

	pState->AddRef();

	m_mapRenderState.insert(make_pair(strKey, pState));

	return pState;
}

CRasterizerState * CRenderManager::CreateRasterizerState(
	const string & strKey, D3D11_FILL_MODE eFill,
	D3D11_CULL_MODE eCull, bool bClockWise, int iDepthBias, 
	float fDepthBiasClamp, float fSlopeScale, bool bDepthClipEnable,
	bool bScissorEnable, bool bMultisampleEnable, 
	bool bAntialisedLineEnable)
{
	CRasterizerState*	pState = (CRasterizerState*)FindRenderState(strKey);

	if (pState)
		return pState;

	pState = new CRasterizerState;

	if (!pState->CreateRasterizerState(strKey, eFill, eCull,
		bClockWise, iDepthBias, fDepthBiasClamp, fSlopeScale,
		bDepthClipEnable, bScissorEnable, bMultisampleEnable,
		bAntialisedLineEnable))
	{
		SAFE_RELEASE(pState);
		return NULL;
	}

	pState->SetTag(strKey);

	pState->AddRef();

	m_mapRenderState.insert(make_pair(strKey, pState));

	return pState;
}

CRenderState * CRenderManager::FindRenderState(const string & strKey)
{
	unordered_map<string, class CRenderState*>::iterator	iter = m_mapRenderState.find(strKey);

	if (iter == m_mapRenderState.end())
		return NULL;

	iter->second->AddRef();

	return iter->second;
}

CRenderingTarget * CRenderManager::CreateRenderTarget(const string & strKey, 
	unsigned int iW, unsigned int iH, DXGI_FORMAT eFmt,
	const Vector4& vClearColor, DXGI_FORMAT eDepthFmt)
{
	CRenderingTarget*	pTarget = FindRenderTarget(strKey);

	if (pTarget)
		return pTarget;

	pTarget = new CRenderingTarget;

	if (!pTarget->CreateTarget(iW, iH, eFmt, vClearColor, eDepthFmt))
	{
		SAFE_RELEASE(pTarget);
		return NULL;
	}

	pTarget->AddRef();
	m_mapTarget.insert(make_pair(strKey, pTarget));

	return pTarget;
}

CRenderingTarget * CRenderManager::FindRenderTarget(const string & strKey)
{
	unordered_map<string, CRenderingTarget*>::iterator	iter = m_mapTarget.find(strKey);

	if (iter == m_mapTarget.end())
		return NULL;

	iter->second->AddRef();

	return iter->second;
}

void CRenderManager::AddRenderTargetToMRT(const string & strMRTKey, 
	const string & strTargetKey)
{
	PMRT	pMRT = FindMRT(strMRTKey);

	CRenderingTarget*	pTarget = FindRenderTarget(strTargetKey);

	if (!pTarget)
		return;

	if (!pMRT)
	{
		pMRT = new MRT;

		m_mapMRT.insert(make_pair(strMRTKey, pMRT));
	}

	if (pMRT->vecTargets.size() == 8)
		return;

	pMRT->vecTargets.push_back(pTarget);

	SAFE_RELEASE(pTarget);
}

void CRenderManager::SetDepthToMRT(const string & strMRTKey, 
	const string & strDepthKey)
{
	PMRT	pMRT = FindMRT(strMRTKey);

	CRenderingTarget*	pTarget = FindRenderTarget(strDepthKey);

	if (!pTarget)
		return;

	else if (!pTarget->GetDepthStencilView())
	{
		SAFE_RELEASE(pTarget);
		return;
	}

	if (!pMRT)
	{
		pMRT = new MRT;

		m_mapMRT.insert(make_pair(strMRTKey, pMRT));
	}

	pMRT->pDepth = pTarget->GetDepthStencilView();

	SAFE_RELEASE(pTarget);
}

void CRenderManager::ClearMRT(const string & strKey)
{
	PMRT	pMRT = FindMRT(strKey);

	if (!pMRT)
		return;

	for (size_t i = 0; i < pMRT->vecTargets.size(); ++i)
	{
		pMRT->vecTargets[i]->ClearTarget();
	}
}

void CRenderManager::SetMRT(const string & strKey)
{
	PMRT	pMRT = FindMRT(strKey);

	if (!pMRT)
		return;

	vector<ID3D11RenderTargetView*>	vecTargets;
	for (size_t i = 0; i < pMRT->vecTargets.size(); ++i)
	{
		vecTargets.push_back(pMRT->vecTargets[i]->GetRenderTargetView());
	}

	CONTEXT->OMGetRenderTargets(vecTargets.size(), pMRT->pOldTarget, &pMRT->pOldDepth);

	ID3D11DepthStencilView*	pDepth = pMRT->pOldDepth;

	if (pMRT->pDepth)
		pDepth = pMRT->pDepth;

	CONTEXT->OMSetRenderTargets(vecTargets.size(), &vecTargets[0], pDepth);
}

void CRenderManager::ResetMRT(const string & strKey)
{
	PMRT	pMRT = FindMRT(strKey);

	if (!pMRT)
		return;

	CONTEXT->OMSetRenderTargets(pMRT->vecTargets.size(), pMRT->pOldTarget, pMRT->pOldDepth);
	SAFE_RELEASE(pMRT->pOldDepth);

	for (size_t i = 0; i < pMRT->vecTargets.size(); ++i)
	{
		SAFE_RELEASE(pMRT->pOldTarget[i]);
	}
}

PMRT CRenderManager::FindMRT(const string & strKey)
{
	unordered_map<string, PMRT>::iterator	iter = m_mapMRT.find(strKey);

	if (iter == m_mapMRT.end())
		return NULL;

	return iter->second;
}

void CRenderManager::AddRenderObject(CGameObject * pObj)
{
	if (pObj->GetCulling())
		return;

	if (false == pObj->GetAlive())
		return;
	// UI인지 아닌지 구분
	if (pObj->CheckComponentFromType(CT_UI))
	{
		if (m_tRenderGroup[RGT_UI].iSize == m_tRenderGroup[RGT_UI].iCapasity)
		{
			m_tRenderGroup[RGT_UI].iCapasity *= 1.5f;
			CGameObject**	pList = new CGameObject*[m_tRenderGroup[RGT_UI].iCapasity];
			memcpy(pList, m_tRenderGroup[RGT_UI].pRenderObj,
				sizeof(CGameObject*) * m_tRenderGroup[RGT_UI].iSize);

			delete[]	m_tRenderGroup[RGT_UI].pRenderObj;
			m_tRenderGroup[RGT_UI].pRenderObj = pList;
		}

		m_tRenderGroup[RGT_UI].pRenderObj[m_tRenderGroup[RGT_UI].iSize] =
			pObj;
		++m_tRenderGroup[RGT_UI].iSize;
	}
	else if (pObj->CheckComponentFromType(CT_PARTICLE_MULTIPLE))
	{
		if (true == pObj->GetEnable())
		{
			if (m_tRenderGroup[RGT_PARTICLE].iSize == m_tRenderGroup[RGT_PARTICLE].iCapasity)
			{
				m_tRenderGroup[CT_PARTICLE_MULTIPLE].iCapasity *= 1.5f;
				CGameObject**	pList = new CGameObject*[m_tRenderGroup[RGT_PARTICLE].iCapasity];
				memcpy(pList, m_tRenderGroup[RGT_PARTICLE].pRenderObj,
					sizeof(CGameObject*) * m_tRenderGroup[RGT_PARTICLE].iSize);

				delete[]	m_tRenderGroup[RGT_PARTICLE].pRenderObj;
				m_tRenderGroup[RGT_PARTICLE].pRenderObj = pList;
			}

			m_tRenderGroup[RGT_PARTICLE].pRenderObj[m_tRenderGroup[RGT_PARTICLE].iSize] = pObj;
			++m_tRenderGroup[RGT_PARTICLE].iSize;
		}
	}

	else
	{
		RENDER_GROUP_TYPE	rgt;

		CRenderer*	pRenderer = pObj->FindComponentFromType<CRenderer>(CT_RENDERER);

		if (pRenderer)
		{
			if (pRenderer->GetAlphaEnable())
				rgt = RGT_ALPHA;

			else if (pObj->CheckComponentFromType(CT_LANDSCAPE))
				rgt = RGT_LANDSCAPE;

			else if (pObj->CheckComponentFromType(CT_DECAL))
				rgt = RGT_DECAL;

			else
				rgt = RGT_DEFAULT;

			if (m_tRenderGroup[rgt].iSize == m_tRenderGroup[rgt].iCapasity)
			{
				m_tRenderGroup[rgt].iCapasity *= 1.5f;
				CGameObject**	pList = new CGameObject*[m_tRenderGroup[rgt].iCapasity];
				memcpy(pList, m_tRenderGroup[rgt].pRenderObj,
					sizeof(CGameObject*) * m_tRenderGroup[rgt].iSize);

				delete[]	m_tRenderGroup[rgt].pRenderObj;
				m_tRenderGroup[rgt].pRenderObj = pList;
			}

			m_tRenderGroup[rgt].pRenderObj[m_tRenderGroup[rgt].iSize] =
				pObj;
			++m_tRenderGroup[rgt].iSize;

			SAFE_RELEASE(pRenderer);
		}
	}
}

void CRenderManager::Render(float fTime)
{
	// 출력할 오브젝트를 정렬한다.
	for (int i = 0; i <= RGT_DEFAULT; ++i)
	{
		if (m_tRenderGroup[i].iSize >= 2)
		{
			sort(&m_tRenderGroup[i].pRenderObj[0],
				&m_tRenderGroup[i].pRenderObj[m_tRenderGroup[i].iSize],
				CRenderManager::SortZ);
		}
	}

	if (m_tRenderGroup[RGT_ALPHA].iSize >= 2)
	{
		sort(&m_tRenderGroup[RGT_ALPHA].pRenderObj[0],
			&m_tRenderGroup[RGT_ALPHA].pRenderObj[m_tRenderGroup[RGT_ALPHA].iSize],
			CRenderManager::SortAlpha);
	}

	if (m_tRenderGroup[RGT_UI].iSize >= 2)
	{
		sort(&m_tRenderGroup[RGT_UI].pRenderObj[0],
			&m_tRenderGroup[RGT_UI].pRenderObj[m_tRenderGroup[RGT_UI].iSize],
			CRenderManager::SortUI);
	}

	RenderShadowMap(fTime);

	// GBuffer를 만든다.
	RenderGBuffer(fTime);

	// Decal을 출력한다.
	RenderDecal(fTime);

	// 조명 누적버퍼를 만든다.
	RenderLightAcc(fTime);
	// 2path - n path

	// 조명 누적버퍼를 이용하여 Albedo에 조명을 처리한다.
	RenderLightBlend(fTime);

	// 최종 조명처리된 타겟을 화면에 출력한다.
	RenderDeferredTarget(fTime);

	//CRenderingTarget*	pTarget = FindRenderTarget("Albedo");

	//pTarget->SetTarget();
	m_pPointSmp->SetSampler(11, SCT_PIXEL);

	CRenderingTarget*	pDepthTarget = FindRenderTarget("Depth");



//<<<<<<< HEAD
//		for (int j = 0; j < m_tRenderGroup[i].iSize; ++j)
//		{
//			m_tRenderGroup[i].pRenderObj[j]->Render(fTime);
//		}
//
//		if (i == RGT_ALPHA)
//			continue;
//
//		if(i != RGT_UI)
//			m_tRenderGroup[i].iSize = 0;
//	}
//=======
//	//for (int i = RGT_ALPHA; i < RGT_END; ++i)
//	//{
//	//	if (i == RGT_PARTICLE)
//	//		continue;
//
//	//	for (int j = 0; j < m_tRenderGroup[i].iSize; ++j)
//	//	{
//	//		m_tRenderGroup[i].pRenderObj[j]->Render(fTime);
//	//	}
//	//	if(i != RGT_UI)
//	//		m_tRenderGroup[i].iSize = 0;
//	//}
//>>>>>>> a8d55e1292ef2b43b13cba5647f35826a6e61645

	for (int i = RGT_LANDSCAPE; i <= RGT_DEFAULT; ++i)
	{
		for (int j = 0; j < m_tRenderGroup[i].iSize; ++j)
		{
			//CCollider* pCollider = nullptr;
			//pCollider = m_tRenderGroup[i].pRenderObj[j]->FindComponentFromType<CCollider>(CT_COLLIDER);
			//if (pCollider != nullptr)
			//{
			//	pCollider->ColliderRender(fTime);
			//	SAFE_RELEASE(pCollider);
			//}

			const auto& pComponentLists = m_tRenderGroup[i].pRenderObj[j]->FindComponentsFromType(CT_COLLIDER);
			if(nullptr != pComponentLists)
			{
				auto begin = pComponentLists->begin();
				auto end = pComponentLists->end();
				for(;begin != end; ++begin)
				{
					static_cast<CCollider*>((*begin))->ColliderRender(fTime);
					//SAFE_RELEASE((*begin));
				}
			}


			m_pDepthDisable->SetState();
			CAxisLine* pAxis = m_tRenderGroup[i].pRenderObj[j]->FindComponentFromType<CAxisLine>(CT_AXIS);
			if (pAxis != nullptr)
			{
				pAxis->AxisRender(fTime);
				SAFE_RELEASE(pAxis);
			}
			m_pDepthDisable->ResetState();
		}
	}
	pDepthTarget->SetShader(13);
	for (int i = 0; i < m_tRenderGroup[RGT_ALPHA].iSize; ++i)
	{
		m_tRenderGroup[RGT_ALPHA].pRenderObj[i]->Render(fTime);
	}


	for (int i = 0; i < m_tRenderGroup[RGT_PARTICLE].iSize; ++i)
	{
		m_tRenderGroup[RGT_PARTICLE].pRenderObj[i]->Render(fTime);
	}
	m_tRenderGroup[RGT_PARTICLE].iSize = 0;

	for (int i = 0; i < m_tRenderGroup[RGT_UI].iSize; ++i)
	{
		m_tRenderGroup[RGT_UI].pRenderObj[i]->Render(fTime);
	}

	/*
		m_strKey = strKey;

	D3D11_BLEND_DESC	tDesc = {};

	tDesc.AlphaToCoverageEnable = bAlphaCoverage;
	tDesc.IndependentBlendEnable = bIndependent;

	for (size_t i = 0; i < m_vecDesc.size(); ++i)
	{
		tDesc.RenderTarget[i] = m_vecDesc[i];
	}

	if (FAILED(DEVICE->CreateBlendState(&tDesc, (ID3D11BlendState**)&m_pState)))
		return false;

	memset(m_fBlendFactor, 0, sizeof(float) * 4);
	m_iSampleMask = 0xffffffff;

	*/
	//	CONTEXT->OMSetBlendState((ID3D11BlendState*)m_pState, m_fBlendFactor, m_iSampleMask);

	//CRenderState*	pState = GET_SINGLE(CRenderManager)->FindRenderState(ALPHA_BLEND_DISABLE);
	//pState->SetState();

	for (int i = RGT_LANDSCAPE; i <= RGT_DEFAULT; ++i)
	{
		for (int j = 0; j < m_tRenderGroup[i].iSize; ++j)
		{
			CCollider* pCollider = nullptr;
			pCollider = m_tRenderGroup[i].pRenderObj[j]->FindComponentFromType<CCollider>(CT_COLLIDER);
			if (pCollider != nullptr)
			{
				pCollider->ColliderRender(fTime);
				SAFE_RELEASE(pCollider);
			}
		}
		m_tRenderGroup[i].iSize = 0;
	}

	for(int i = 0; i < m_tRenderGroup[RGT_UI].iSize; ++i)
	{
			
		CCollider* pCollider = nullptr;
		pCollider = m_tRenderGroup[RGT_UI].pRenderObj[i]->FindComponentFromType<CCollider>(CT_COLLIDER);
		if (pCollider != nullptr)
		{
			pCollider->ColliderRender(fTime);
			SAFE_RELEASE(pCollider);
		}
	}
	for (int i = 0; i < m_tRenderGroup[RGT_ALPHA].iSize; ++i)
	{
		CCollider* pCollider = nullptr;
		pCollider = m_tRenderGroup[RGT_ALPHA].pRenderObj[i]->FindComponentFromType<CCollider>(CT_COLLIDER);
		if (pCollider != nullptr)
		{
			pCollider->ColliderRender(fTime);
			SAFE_RELEASE(pCollider);
		}
	}
	m_tRenderGroup[RGT_ALPHA].iSize = 0;

	m_tRenderGroup[RGT_UI].iSize = 0;


	pDepthTarget->ResetShader(13);

	SAFE_RELEASE(pDepthTarget);



	//pTarget->ResetTarget();

	//if (GetAsyncKeyState(VK_F9) & 0x8000)
	//{
	//	PMRT	pMRT = FindMRT("GBuffer");

	//	const wchar_t*	pFile[4] = { L"Albedo.png", L"Normal.png", L"Depth.png", L"Specular.png" };
	//	for (size_t i = 0; i < pMRT->vecTargets.size(); ++i)
	//	{
	//		pMRT->vecTargets[i]->SaveFile(pFile[i]);
	//	}
	//}

	// 디버깅용 타겟을 출력한다.
	unordered_map<string, CRenderingTarget*>::iterator	iter;
	unordered_map<string, CRenderingTarget*>::iterator	iterEnd = m_mapTarget.end();

	for (iter = m_mapTarget.begin(); iter != iterEnd; ++iter)
	{
		if (!iter->second->GetDebugEnable())
			continue;

		iter->second->DebugRender(fTime);
	}

	CScene*	pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();

	//CGameObject*	pMouseObj = GET_SINGLE(CInput)->GetMouseObj();

	//pMouseObj->SetScene(pScene);

	//pMouseObj->Render(fTime);

	//SAFE_RELEASE(pMouseObj);

	SAFE_RELEASE(pScene);
}

void CRenderManager::RenderShadowMap(float fTime)
{
	CRenderingTarget* pShadowDepth = FindRenderTarget("ShadowDepth");

	pShadowDepth->ClearTarget();
	pShadowDepth->SetTarget();

	for (int i = RGT_LANDSCAPE; i <= RGT_DEFAULT; ++i)
	{
		for (int j = 0; j < m_tRenderGroup[i].iSize; ++j)
		{
			m_tRenderGroup[i].pRenderObj[j]->RenderShadowMap(fTime);
		}
	}

	pShadowDepth->ResetTarget();

	SAFE_RELEASE(pShadowDepth);
}

void CRenderManager::RenderDecal(float fTime)
{
	SetMRT("Decal");

	CRenderingTarget*	pDepthTarget = FindRenderTarget("Depth");
	//CRenderingTarget*	pNormalTarget = FindRenderTarget("Normal");
	CRenderingTarget*	pTangentTarget = FindRenderTarget("Tangent");
	CRenderingTarget*	pBinormalTarget = FindRenderTarget("Binormal");
	//CRenderingTarget*	pSpecularTarget = FindRenderTarget("Specular");

	m_pPointSmp->SetSampler(11, SCT_PIXEL);
	pDepthTarget->SetShader(13);
	//pNormalTarget->SetShader(14);
	pTangentTarget->SetShader(15);
	pBinormalTarget->SetShader(16);
	//pSpecularTarget->SetShader(17);

	for (int j = 0; j < m_tRenderGroup[RGT_DECAL].iSize; ++j)
	{
		m_tRenderGroup[RGT_DECAL].pRenderObj[j]->Render(fTime);
	}

	m_tRenderGroup[RGT_DECAL].iSize = 0;

	pDepthTarget->ResetShader(13);
	//pNormalTarget->ResetShader(14);
	pTangentTarget->ResetShader(15);
	pBinormalTarget->ResetShader(16);
	//pSpecularTarget->ResetShader(17);

	//SAFE_RELEASE(pSpecularTarget);
	SAFE_RELEASE(pDepthTarget);
	//SAFE_RELEASE(pNormalTarget);
	SAFE_RELEASE(pTangentTarget);
	SAFE_RELEASE(pBinormalTarget);

	ResetMRT("Decal");
}

void CRenderManager::RenderGBuffer(float fTime)
{
	CRenderingTarget*	pDepthTarget = FindRenderTarget("Shadow");
	pDepthTarget->SetTarget();
	pDepthTarget->ClearTarget();
	pDepthTarget->SetShader(4);

	CRenderingTarget* pShadowDepth = FindRenderTarget("ShadowDepth");

	pShadowDepth->SetShader(18);

	CScene*	pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLight*	pLight = pScene->GetGlobalLight("GlobalLight");

	if (pLight)
	{
		pLight->SetShader();

		SAFE_RELEASE(pLight);
	}

	for (int i = 0; i < m_tRenderGroup[RGT_LANDSCAPE].iSize; ++i)
	{
		CRenderer* pRenderer = m_tRenderGroup[RGT_LANDSCAPE].pRenderObj[i]->FindComponentFromType<CRenderer>(CT_RENDERER);

		pRenderer->UpdateTransform();

		unordered_map<int, PRENDERERCBUFFER>::iterator	iter;
		unordered_map<int, PRENDERERCBUFFER>::iterator	iterEnd = pRenderer->m_mapCBuffer.end();

		for (iter = pRenderer->m_mapCBuffer.begin(); iter != iterEnd; ++iter)
		{
			GET_SINGLE(CShaderManager)->UpdateCBuffer(iter->second->strName,
				iter->second->pData, iter->second->iShaderType);
		}

		// 렌더상태 적용
		for (int i = 0; i < RST_END; ++i)
		{
			if (pRenderer->m_pRenderState[i])
				pRenderer->m_pRenderState[i]->SetState();
		}

		// 본 텍스쳐를 얻어온다.
		CAnimation*	pAnimation = pRenderer->FindComponentFromType<CAnimation>(CT_ANIMATION);

		if (pAnimation)
		{
			ID3D11ShaderResourceView*	pBoneSRV = pAnimation->GetBoneTexture();
			CONTEXT->VSSetShaderResources(7, 1, &pBoneSRV);
		}

		//pRenderer->m_pShader->SetShader();
		CShader* pShader = GET_SINGLE(CShaderManager)->FindShader(SHADOWMAP_SHADER);
		pShader->SetShader();
		SAFE_RELEASE(pShader);
		CONTEXT->IASetInputLayout(pRenderer->m_pInputLayout);

		//CShader* pShader = GET_SINGLE(CShaderManager)->FindShader(SHADOWMAP_SHADER);
		//pShader->SetShader();
		//SAFE_RELEASE(pShader);
		//ID3D11InputLayout* pInputLayet = GET_SINGLE(CShaderManager)->FindInputLayout("ShadowVertexPos");
		//CONTEXT->IASetInputLayout(pInputLayet);

		for (size_t i = 0; i < pRenderer->m_vecMaterials.size(); ++i)
		{
			for (size_t j = 0; j < pRenderer->m_vecMaterials[i].size(); ++j)
			{
				pRenderer->m_vecMaterials[i][j]->SetMaterial(SCT_VERTEX | SCT_PIXEL);
				pRenderer->m_pMesh->Render(fTime, i, j);
				pRenderer->m_vecMaterials[i][j]->ResetMaterial(SCT_VERTEX | SCT_PIXEL);
			}
		}

		if (pAnimation)
		{
			ID3D11ShaderResourceView*	pBoneSRV = NULL;
			CONTEXT->VSSetShaderResources(7, 1, &pBoneSRV);
			SAFE_RELEASE(pAnimation);
		}

		// 렌더상태 복원
		for (int i = 0; i < RST_END; ++i)
		{
			if (pRenderer->m_pRenderState[i])
				pRenderer->m_pRenderState[i]->ResetState();
		}

		SAFE_RELEASE(pRenderer);
	}


	for (int i = 0; i < m_tRenderGroup[RGT_DEFAULT].iSize; ++i)
	{
		CRenderer* pRenderer = m_tRenderGroup[RGT_DEFAULT].pRenderObj[i]->FindComponentFromType<CRenderer>(CT_RENDERER);

		pRenderer->UpdateTransform();

		unordered_map<int, PRENDERERCBUFFER>::iterator	iter;
		unordered_map<int, PRENDERERCBUFFER>::iterator	iterEnd = pRenderer->m_mapCBuffer.end();

		for (iter = pRenderer->m_mapCBuffer.begin(); iter != iterEnd; ++iter)
		{
			GET_SINGLE(CShaderManager)->UpdateCBuffer(iter->second->strName,
				iter->second->pData, iter->second->iShaderType);
		}

		// 렌더상태 적용
		for (int i = 0; i < RST_END; ++i)
		{
			if (pRenderer->m_pRenderState[i])
				pRenderer->m_pRenderState[i]->SetState();
		}

		// 본 텍스쳐를 얻어온다.

		CAnimation*	pAnimation = pRenderer->FindComponentFromType<CAnimation>(CT_ANIMATION);

		if (pAnimation)
		{
			ID3D11ShaderResourceView*	pBoneSRV = pAnimation->GetBoneTexture();
			CONTEXT->VSSetShaderResources(7, 1, &pBoneSRV);
		}

		if (pAnimation != nullptr)
		{
			CShader* pShader = GET_SINGLE(CShaderManager)->FindShader(SHADOWCHARACTER_SHADER);
			pShader->SetShader();
			SAFE_RELEASE(pShader);
			CONTEXT->IASetInputLayout(pRenderer->m_pInputLayout);
		}
		else
		{
			pRenderer->SetShadowShader(SHADOWSTATICOBJECT_SHADER);
			CShader* pShader = GET_SINGLE(CShaderManager)->FindShader(SHADOWSTATICOBJECT_SHADER);
			pShader->SetShader();
			SAFE_RELEASE(pShader);
			CONTEXT->IASetInputLayout(pRenderer->m_pInputLayout);
		}
		//CShader* pShader = GET_SINGLE(CShaderManager)->FindShader(SHADOWCHARACTER_SHADER);
		//pShader->SetShader();
		////SAFE_RELEASE(pShader);
		//ID3D11InputLayout* pInputLayet = GET_SINGLE(CShaderManager)->FindInputLayout("ShadowCharacterAnim");
		//CONTEXT->IASetInputLayout(pInputLayet);

		for (size_t i = 0; i < pRenderer->m_vecMaterials.size(); ++i)
		{
			for (size_t j = 0; j < pRenderer->m_vecMaterials[i].size(); ++j)
			{
				pRenderer->m_vecMaterials[i][j]->SetMaterial(SCT_VERTEX | SCT_PIXEL);
				pRenderer->m_pMesh->Render(fTime, i, j);
				pRenderer->m_vecMaterials[i][j]->ResetMaterial(SCT_VERTEX | SCT_PIXEL);
			}
		}

		if (pAnimation)
		{
			ID3D11ShaderResourceView*	pBoneSRV = NULL;
			CONTEXT->VSSetShaderResources(7, 1, &pBoneSRV);
			SAFE_RELEASE(pAnimation);
		}

		// 렌더상태 복원
		for (int i = 0; i < RST_END; ++i)
		{
			if (pRenderer->m_pRenderState[i])
				pRenderer->m_pRenderState[i]->ResetState();
		}

		SAFE_RELEASE(pRenderer);
	}

	SAFE_RELEASE(pScene);
	SAFE_RELEASE(pLight);

	pDepthTarget->ResetShader(4);
	pDepthTarget->ResetTarget();

	SAFE_RELEASE(pDepthTarget);

	ClearMRT("GBuffer");
	SetMRT("GBuffer");

	pDepthTarget = FindRenderTarget("Shadow");
	pDepthTarget->SetShader(4);
	m_pPointSmp->SetSampler(11, SCT_PIXEL);




	for (int i = RGT_LANDSCAPE; i <= RGT_DEFAULT; ++i)
	{
		for (int j = 0; j < m_tRenderGroup[i].iSize; ++j)
		{
			m_tRenderGroup[i].pRenderObj[j]->Render(fTime);
		}
	}

	/*for (int i = RGT_LANDSCAPE; i <= RGT_DEFAULT; ++i)
	{
		for (int j = 0; j < m_tRenderGroup[i].iSize; ++j)
		{
			CCollider* pCollider = nullptr;
			pCollider = m_tRenderGroup[i].pRenderObj[j]->FindComponentFromType<CCollider>(CT_COLLIDER);
			if (pCollider != nullptr)
			{
				pCollider->ColliderRender(fTime);
				SAFE_RELEASE(pCollider);
			}
		}
	}*/


	//for (int i = RGT_LANDSCAPE; i <= RGT_DEFAULT; ++i)
	//	m_tRenderGroup[i].iSize = 0;

	pDepthTarget->ResetShader(4);
	SAFE_RELEASE(pDepthTarget);
	ResetMRT("GBuffer");

	
}

void CRenderManager::RenderLightAcc(float fTime)
{
	ClearMRT("LightAcc");
	SetMRT("LightAcc");

	m_pDepthDisable->SetState();
	m_pAccBlend->SetState();

	CScene*	pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	const list<CGameObject*>*	pLightList = pScene->GetLightList();

	list<CGameObject*>::const_iterator	iter;
	list<CGameObject*>::const_iterator	iterEnd = pLightList->end();

	m_pPointSmp->SetSampler(11, SCT_PIXEL);

	// GBuffer MRT 를 얻어온다.
	PMRT	pGBuffer = FindMRT("GBuffer");

	pGBuffer->vecTargets[1]->SetShader(12);
	pGBuffer->vecTargets[2]->SetShader(13);
	pGBuffer->vecTargets[3]->SetShader(14);

	CRenderingTarget* pShadowDepth = FindRenderTarget("ShadowDepth");

	pShadowDepth->SetShader(18);

	TRANSFORMCBUFFER	tBuffer = {};

	XMMATRIX	matView, matProj;

	CCamera*	pCamera = pScene->GetMainCamera();

	matView = pCamera->GetViewMatrix().mat;
	matProj = pCamera->GetProjMatrix().mat;

	SAFE_RELEASE(pCamera);

	tBuffer.matView = matView;
	tBuffer.matProj = matProj;
	tBuffer.matWV = tBuffer.matWorld * tBuffer.matView;
	tBuffer.matWVP = tBuffer.matWV * tBuffer.matProj;
	tBuffer.matWP = tBuffer.matWorld * tBuffer.matProj;
	tBuffer.matVP = matView * matProj;
	tBuffer.matInvProj = XMMatrixInverse(&XMMatrixDeterminant(matProj),
		matProj);

	tBuffer.matView = XMMatrixTranspose(tBuffer.matView.mat);
	tBuffer.matProj = XMMatrixTranspose(tBuffer.matProj.mat);
	tBuffer.matWV = XMMatrixTranspose(tBuffer.matWV.mat);
	tBuffer.matWVP = XMMatrixTranspose(tBuffer.matWVP.mat);
	tBuffer.matWP = XMMatrixTranspose(tBuffer.matWP.mat);
	tBuffer.matVP = XMMatrixTranspose(tBuffer.matVP.mat);
	tBuffer.matInvProj = XMMatrixTranspose(tBuffer.matInvProj.mat);

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform",
		&tBuffer, SCT_PIXEL);

	m_pLightAccShader->SetShader();

	for (iter = pLightList->begin(); iter != iterEnd; ++iter)
	{
		if (!(*iter)->GetEnable())
			continue;

		CLight*	pLight = (*iter)->FindComponentFromType<CLight>(CT_LIGHT);

		if (!pLight->GetEnable())
		{
			SAFE_RELEASE(pLight);
			continue;
		}

		pLight->SetShader();

		SAFE_RELEASE(pLight);
		UINT	iOffset = 0;
		CONTEXT->IASetInputLayout(NULL);
		CONTEXT->IASetVertexBuffers(0, 0, NULL, 0, &iOffset);
		CONTEXT->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		CONTEXT->IASetIndexBuffer(NULL, DXGI_FORMAT_UNKNOWN, 0);
		CONTEXT->Draw(4, 0);
	}

	SAFE_RELEASE(pScene);

	pGBuffer->vecTargets[1]->ResetShader(12);
	pGBuffer->vecTargets[2]->ResetShader(13);
	pGBuffer->vecTargets[3]->ResetShader(14);

	pShadowDepth->ResetShader(18);
	SAFE_RELEASE(pShadowDepth);

	m_pDepthDisable->ResetState();
	m_pAccBlend->ResetState();

	ResetMRT("LightAcc");
}

void CRenderManager::RenderLightBlend(float fTime)
{
	CRenderingTarget*	pTarget = FindRenderTarget("LightBlend");

	pTarget->SetTarget();
	pTarget->ClearTarget();

	CRenderingTarget*	pAlbedo = FindRenderTarget("Albedo");
	CRenderingTarget*	pLightDif = FindRenderTarget("LightDif");
	CRenderingTarget*	pLightSpc = FindRenderTarget("LightSpc");
	
	pAlbedo->SetShader(11);
	pLightDif->SetShader(15);
	pLightSpc->SetShader(16);

	m_pLightBlendShader->SetShader();

	UINT	iOffset = 0;
	CONTEXT->IASetInputLayout(NULL);
	CONTEXT->IASetVertexBuffers(0, 0, NULL, 0, &iOffset);
	CONTEXT->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CONTEXT->IASetIndexBuffer(NULL, DXGI_FORMAT_UNKNOWN, 0);
	CONTEXT->Draw(4, 0);

	pAlbedo->ResetShader(11);
	pLightDif->ResetShader(15);
	pLightSpc->ResetShader(16);

	pTarget->ResetTarget();

	SAFE_RELEASE(pAlbedo);
	SAFE_RELEASE(pLightDif);
	SAFE_RELEASE(pLightSpc);
	SAFE_RELEASE(pTarget);

}

void CRenderManager::RenderDeferredTarget(float fTime)
{
	m_pDepthDisable->SetState();

	CRenderingTarget*	pDeferred = FindRenderTarget("LightBlend");

	pDeferred->SetShader(17);

	m_pDeferredShader->SetShader();

	UINT	iOffset = 0;
	CONTEXT->IASetInputLayout(NULL);
	CONTEXT->IASetVertexBuffers(0, 0, NULL, 0, &iOffset);
	CONTEXT->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CONTEXT->IASetIndexBuffer(NULL, DXGI_FORMAT_UNKNOWN, 0);
	CONTEXT->Draw(4, 0);

	pDeferred->ResetShader(17);

	SAFE_RELEASE(pDeferred);

	m_pDepthDisable->ResetState();
}

bool CRenderManager::SortZ(CGameObject * pSrc, CGameObject * pDest)
{
	CTransform*	pSrcTr = pSrc->GetTransform();
	CTransform*	pDestTr = pDest->GetTransform();

	CScene*	pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CCamera*	pCamera = pScene->GetMainCamera();

	SAFE_RELEASE(pScene);

	Matrix	matView = pCamera->GetViewMatrix();

	Vector3	vSrcPos = pSrcTr->GetWorldPos();
	Vector3	vDestPos = pDestTr->GetWorldPos();

	vSrcPos = vSrcPos.TransformCoord(matView.mat);
	vDestPos = vDestPos.TransformCoord(matView.mat);

	SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pSrcTr);
	SAFE_RELEASE(pDestTr);

	return vSrcPos.z < vDestPos.z;
}

bool CRenderManager::SortAlpha(CGameObject * pSrc, CGameObject * pDest)
{
	CTransform*	pSrcTr = pSrc->GetTransform();
	CTransform*	pDestTr = pDest->GetTransform();

	CScene*	pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CCamera*	pCamera = pScene->GetMainCamera();

	SAFE_RELEASE(pScene);

	Matrix	matView = pCamera->GetViewMatrix();

	Vector3	vSrcPos = pSrcTr->GetWorldPos();
	Vector3	vDestPos = pDestTr->GetWorldPos();

	vSrcPos = vSrcPos.TransformCoord(matView.mat);
	vDestPos = vDestPos.TransformCoord(matView.mat);

	SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pSrcTr);
	SAFE_RELEASE(pDestTr);

	return vSrcPos.z > vDestPos.z;
}

bool CRenderManager::SortUI(CGameObject * pSrc, CGameObject * pDest)
{
	CUI*	pSrcUI = pSrc->FindComponentFromType<CUI>(CT_UI);
	CUI*	pDestUI = pDest->FindComponentFromType<CUI>(CT_UI);

	int	iSrcZ = pSrcUI->GetZOrder();
	int	iDestZ = pDestUI->GetZOrder();

	SAFE_RELEASE(pSrcUI);
	SAFE_RELEASE(pDestUI);

	return iSrcZ < iDestZ;
}

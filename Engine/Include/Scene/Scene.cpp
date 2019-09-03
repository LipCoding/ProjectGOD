#include "Scene.h"
#include "Layer.h"
#include "../GameObject/GameObject.h"
#include "SceneScript.h"
#include "../Component/Camera.h"
#include "../Component/Transform.h"
#include "../Device.h"
#include "../Component/LightDir.h"
#include "../Component/LightPoint.h"
#include "../Component/LightSpot.h"
#include "../Component/Renderer.h"
#include "../Component/Material.h"
#include "../Core/Input.h"
#include "../Core/PathManager.h"
#include "../Resources/ResourcesManager.h"
#include "../Component/Material.h"
#include "../Core/QuadTreeManager.h"
#include "../Component/Arm.h"
#include "../Component/Picking.h"
#include "../Resources/Mesh.h"
#include "../Component/Renderer.h"
#include "../Component/LandScape.h"
#include "../Core/SoundManager.h"

PG_USING

CScene::CScene() :
	m_pMainCameraObj(NULL),
	m_pMainCamera(NULL),
	m_pMainCameraTr(NULL),
	m_pUICameraObj(NULL),
	m_pUICamera(NULL),
	m_pUICameraTr(NULL),
	m_pSkyObject(NULL),
	m_pLightCameraObj(NULL),
	m_pLightCameraTr(NULL),
	m_pLightCamera(NULL)
{
	SetTag("Scene");
	SetTypeName("CScene");
	SetTypeID<CScene>();
	m_vecLayer.reserve(10);
}

CScene::~CScene()
{
	CGameObject::EraseObj(m_pSkyObject);
	SAFE_RELEASE(m_pSkyObject);
	Safe_Release_VecList(m_LightList);
	//CGameObject::EraseObj(m_pMainCameraObj);
	//CGameObject::EraseObj(m_pUICameraObj);
	SAFE_RELEASE(m_pMainCameraTr);
	SAFE_RELEASE(m_pMainCamera);
	SAFE_RELEASE(m_pMainCameraObj);
	SAFE_RELEASE(m_pUICameraTr);
	SAFE_RELEASE(m_pUICamera);
	SAFE_RELEASE(m_pUICameraObj);

	unordered_map<string, CGameObject*>::iterator	iter;
	unordered_map<string, CGameObject*>::iterator	iterEnd = m_mapCamera.end();

	for (iter = m_mapCamera.begin(); iter != iterEnd; ++iter)
	{
		CGameObject::EraseObj(iter->second);
	}

	Safe_Release_Map(m_mapCamera);
	//CGameObject::EraseObj();
	CGameObject::ErasePrototype(this);
	Safe_Release_VecList(m_vecSceneScript);
	Safe_Release_VecList(m_vecLayer);
}

CGameObject * CScene::CreatePrototype(const string & strKey)
{
	return CGameObject::CreatePrototype(strKey, this);
}

CGameObject * CScene::CreateCamera(const string & strKey,
	const Vector3 & vPos, float fViewAngle, float fAspect,
	float fNear, float fFar)
{
	CGameObject*	pCameraObj = FindCamera(strKey);

	if (pCameraObj)
		return pCameraObj;

	pCameraObj = CGameObject::CreateObject(strKey);

	CTransform*	pTransform = pCameraObj->GetTransform();

	pTransform->SetWorldPos(vPos);

	SAFE_RELEASE(pTransform);

	CCamera* pCamera = pCameraObj->AddComponent<CCamera>("Camera");

	pCamera->SetPerspectiveProj(fViewAngle, fAspect, fNear, fFar);
	pCamera->SetShadowPerspectiveProj(fViewAngle, fAspect, fNear, fFar);

	SAFE_RELEASE(pCamera);

	pCameraObj->SetScene(this);

	pCameraObj->AddRef();
	m_mapCamera.insert(make_pair(strKey, pCameraObj));

	return pCameraObj;
}

CGameObject * CScene::CreateOrthoCamera(const string & strKey,
	const Vector3 & vPos, const RESOLUTION & tRS, float fNear, float fFar)
{
	CGameObject*	pCameraObj = FindCamera(strKey);

	if (pCameraObj)
		return pCameraObj;

	pCameraObj = CGameObject::CreateObject(strKey);

	CTransform*	pTransform = pCameraObj->GetTransform();

	pTransform->SetWorldPos(vPos);

	SAFE_RELEASE(pTransform);

	CCamera* pCamera = pCameraObj->AddComponent<CCamera>("Camera");

	pCamera->SetOrthoProj(tRS, fNear, fFar);

	SAFE_RELEASE(pCamera);

	pCameraObj->AddRef();
	m_mapCamera.insert(make_pair(strKey, pCameraObj));

	return pCameraObj;
}

void CScene::ChangeMainCamera(const string & strKey)
{
	SAFE_RELEASE(m_pMainCameraObj);
	SAFE_RELEASE(m_pMainCamera);
	SAFE_RELEASE(m_pMainCameraTr);

	m_pMainCameraObj = FindCamera(strKey);
	m_pMainCameraTr = m_pMainCameraObj->GetTransform();
	m_pMainCamera = m_pMainCameraObj->FindComponentFromType<CCamera>(CT_CAMERA);
}

CGameObject * CScene::FindCamera(const string & strKey)
{
	unordered_map<string, CGameObject*>::iterator	iter = m_mapCamera.find(strKey);

	if (iter == m_mapCamera.end())
		return NULL;

	iter->second->AddRef();

	return iter->second;
}

CGameObject * CScene::GetMainCameraObj() const
{
	m_pMainCameraObj->AddRef();
	return m_pMainCameraObj;
}

CTransform * CScene::GetMainCameraTr() const
{
	m_pMainCameraTr->AddRef();
	return m_pMainCameraTr;
}

CCamera * CScene::GetMainCamera() const
{
	m_pMainCamera->AddRef();
	return m_pMainCamera;
}

CGameObject * CScene::GetUICameraObj() const
{
	m_pUICameraObj->AddRef();
	return m_pUICameraObj;
}

CTransform * CScene::GetUICameraTr() const
{
	m_pUICameraTr->AddRef();
	return m_pUICameraTr;
}

CCamera * CScene::GetUICamera() const
{
	m_pUICamera->AddRef();
	return m_pUICamera;
}

CGameObject * CScene::GetLightCameraObj() const
{
	return m_pLightCameraObj;
}

CTransform * CScene::GetLightCameraTr() const
{
	return m_pLightCameraTr;
}

CCamera * CScene::GetLightCamera() const
{
	return m_pLightCamera;
}

CLight * CScene::CreateLight(const string & strTag, LIGHT_TYPE eType)
{
	CGameObject*	pLightObj = new CGameObject;

	pLightObj->SetTag(strTag);

	if (!pLightObj->Init())
	{
		SAFE_RELEASE(pLightObj);
		return NULL;
	}

	CLight*	pLight = NULL;

	switch (eType)
	{
	case LT_DIR:
		pLight = pLightObj->AddComponent<CLightDir>(strTag);
		break;
	case LT_POINT:
		pLight = pLightObj->AddComponent<CLightPoint>(strTag);
		break;
	case LT_SPOT:
		pLight = pLightObj->AddComponent<CLightSpot>(strTag);
		break;
	}

	m_LightList.push_back(pLightObj);

	return pLight;
}

CLight * CScene::GetGlobalLight(const string & strTag)
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_LightList.end();

	for (iter = m_LightList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			CLight*	pLight = (*iter)->FindComponentFromType<CLight>(CT_LIGHT);
			return pLight;
		}
	}

	return NULL;
}

CTransform * CScene::GetGlobalLightTransform(const string & strTag)
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_LightList.end();

	for (iter = m_LightList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			return (*iter)->GetTransform();
		}
	}

	return NULL;
}

const list<class CGameObject*>* CScene::GetLightList()
{
	return &m_LightList;
}

void CScene::DeleteLight(const string & strTag)
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_LightList.end();

	for (iter = m_LightList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			(*iter)->Die();
			SAFE_RELEASE((*iter));
			iter = m_LightList.erase(iter);
		}
	}
}

bool CScene::Init()
{
	CLayer*	pLayer = CreateLayer("Default");

	SAFE_RELEASE(pLayer);

	pLayer = CreateLayer("UI", UI_LAYER);

	SAFE_RELEASE(pLayer);

	pLayer = CreateLayer("Effect");

	SAFE_RELEASE(pLayer);

	// 메인 카메라 생성
	m_pMainCameraObj = CreateCamera("MainCamera",
		Vector3(0.f, 5.f, -5.f), XMConvertToRadians(60.f),
		(float)DEVICE_RESOLUTION.iWidth / (float)DEVICE_RESOLUTION.iHeight, 1.f, 1000.f);

	m_pMainCamera = m_pMainCameraObj->FindComponentFromTypeID<CCamera>();
	m_pMainCameraTr = m_pMainCameraObj->GetTransform();


	// 라이트 카메라
	/*m_pLightCameraObj = CreateCamera("LightCamera",
		Vector3(512.f / 2.f, 260.f, 512.f / 2.f), XMConvertToRadians(90.f),
		(float)DEVICE_RESOLUTION.iWidth / (float)DEVICE_RESOLUTION.iWidth, 200.f, 261.f);
	m_pLightCamera = m_pLightCameraObj->FindComponentFromTypeID<CCamera>();
	m_pLightCameraTr = m_pLightCameraObj->GetTransform();*/

	m_pLightCameraObj = CreateCamera("LightCamera",
		Vector3(-512.f / 8.f, 400.f, -512.f / 8.f), XMConvertToRadians(90.f),
	(float)DEVICE_RESOLUTION.iWidth / (float)DEVICE_RESOLUTION.iHeight, 200.f, 261.f);
	m_pLightCamera = m_pLightCameraObj->FindComponentFromTypeID<CCamera>();
	m_pLightCameraTr = m_pLightCameraObj->GetTransform();

	/*m_pLightCameraObj = CreateCamera("LightCamera",
		Vector3(512.f / 2.f, 10.f, 512.f / 2.f), XMConvertToRadians(90.f),
		(float)DEVICE_RESOLUTION.iWidth / (float)DEVICE_RESOLUTION.iHeight, 1.f, 100.f);
	m_pLightCamera = m_pLightCameraObj->FindComponentFromTypeID<CCamera>();
	m_pLightCameraTr = m_pLightCameraObj->GetTransform();*/

	//m_pLightCameraTr->SetWorldRotX(PG_PI / 2.f);
	//m_pLightCameraTr->SetWorldRotY(PG_PI / 2.f);

	//// UI 카메라 생성
	m_pUICameraObj = CreateOrthoCamera("UICamera",
		Vector3(0.f, 0.f, 0.f), DEVICE_RESOLUTION, 0.f, 5000.f);
	m_pUICamera = m_pUICameraObj->FindComponentFromTypeID<CCamera>();
	m_pUICameraTr = m_pUICameraObj->GetTransform();
	//m_pMainCameraTr->SetWorldRotX(PG_PI / -2.f);

	// 전역 조명을 생성한다.
	/*CLight*	pGlobalLight = CreateLight("GlobalLight", LT_DIR);

	CTransform*	pLightTr = pGlobalLight->GetTransform();

	pLightTr->SetWorldRot(PG_PI / 4.f, PG_PI / 2.f, 0.f);

	SAFE_RELEASE(pLightTr);

	SAFE_RELEASE(pGlobalLight);*/

	/*pGlobalLight->SetLightColor(Vector4(0.2f, 0.2f, 0.2f, 1.f), Vector4(0.1f, 0.1f, 0.1f, 1.f),
	Vector4(0.f, 0.f, 0.f, 1.f));*/

	CLight*	pGlobalLight = CreateLight("GlobalLight", LT_POINT);

	pGlobalLight->SetLightRange(100000.f);

	//pGlobalLight->SetLightColor()

	CTransform*	pLightTr = pGlobalLight->GetTransform();
	pLightTr->SetWorldPos(512.f / 2.f, 950.f, 512.f / 2.f);

	SAFE_RELEASE(pLightTr);

	SAFE_RELEASE(pGlobalLight);

	// 스카이박스
	m_pSkyObject = CGameObject::CreateObject("Sky");

	m_pSkyObject->SetScene(this);

	CTransform*	pSkyTr = m_pSkyObject->GetTransform();

	pSkyTr->SetWorldScale(50000.f, 50000.f, 50000.f);

	SAFE_RELEASE(pSkyTr);

	CRenderer*	pRenderer = m_pSkyObject->AddComponent<CRenderer>("SkyRenderer");

	pRenderer->SetMesh("Sphere");
	pRenderer->SetShader(SKY_SHADER);
	pRenderer->SetInputLayout("VertexPos");

	CMaterial*	pMaterial = pRenderer->CreateMaterial();

	pMaterial->SetDiffuseTexInfo(SAMPLER_LINEAR, "Sky", 0, 0, L"Skybox\\alps.dds");

	SAFE_RELEASE(pMaterial);

	pRenderer->SetRenderState(CULLING_NONE);
	pRenderer->SetRenderState(DEPTH_LESS_EQUAL);

	SAFE_RELEASE(pRenderer);

	return true;
}

void CScene::Input(float fTime)
{
	vector<CSceneScript*>::iterator	iter1;
	vector<CSceneScript*>::iterator	iter1End = m_vecSceneScript.end();

	for (iter1 = m_vecSceneScript.begin(); iter1 != iter1End;)
	{
		if (!(*iter1)->GetEnable())
		{
			++iter1;
			continue;
		}

		(*iter1)->Input(fTime);

		if (!(*iter1)->GetAlive())
		{
			SAFE_RELEASE((*iter1));
			iter1 = m_vecSceneScript.erase(iter1);
		}

		else
			++iter1;
	}

	vector<CLayer*>::iterator	iter;
	vector<CLayer*>::iterator	iterEnd = m_vecLayer.end();

	for (iter = m_vecLayer.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Input(fTime);

		if (!(*iter)->GetAlive())
		{
			SAFE_RELEASE((*iter));
			iter = m_vecLayer.erase(iter);
		}

		else
			++iter;
	}

	m_pMainCameraObj->Input(fTime);
	
	if(m_pLightCameraObj)
		m_pLightCameraObj->Input(fTime);
}

int CScene::Update(float fTime)
{
	vector<CSceneScript*>::iterator	iter1;
	vector<CSceneScript*>::iterator	iter1End = m_vecSceneScript.end();

	for (iter1 = m_vecSceneScript.begin(); iter1 != iter1End;)
	{
		if (!(*iter1)->GetEnable())
		{
			++iter1;
			continue;
		}

		(*iter1)->Update(fTime);

		if (!(*iter1)->GetAlive())
		{
			SAFE_RELEASE((*iter1));
			iter1 = m_vecSceneScript.erase(iter1);
		}

		else
			++iter1;
	}

	vector<CLayer*>::iterator	iter;
	vector<CLayer*>::iterator	iterEnd = m_vecLayer.end();

	for (iter = m_vecLayer.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Update(fTime);

		if (!(*iter)->GetAlive())
		{
			SAFE_RELEASE((*iter));
			iter = m_vecLayer.erase(iter);
		}

		else
			++iter;
	}

	m_pMainCameraObj->Update(fTime);

	// Terrain Cam Collide
	Vector3 vMainCamPos = m_pMainCameraTr->GetWorldPos();

	// Static Object Cam Collide
	CArm *pArm = m_pMainCameraObj->FindComponentFromType<CArm>(CT_ARM);
	if (pArm)
	{
		CPicking *pPicking = m_pMainCameraObj->FindComponentFromTag<CPicking>("Picking");

		if (pPicking == nullptr)
		{
			pPicking = m_pMainCameraObj->AddComponent<CPicking>("Picking");
		}

		float		fNearDist = 10000.f;
		Vector3		vNearRayCollidePos;

		CGameObject *collideCamTargetObj = nullptr;
		Vector3 vCameraRay = -m_pMainCameraTr->GetWorldAxis(AXIS_Z);

		float fDistCamToTarget = vMainCamPos.Distance(pArm->GetTargetPos());

		// z
		float fTerrainHeight = GET_SINGLE(CQuadTreeManager)->GetY(m_pMainCameraTr->GetWorldPos());
		if (vMainCamPos.y < fTerrainHeight + 1.5f)
			m_pMainCameraTr->SetWorldPos(vMainCamPos.x, fTerrainHeight + 1.5f, vMainCamPos.z);

		// Objects
		CLayer *pLayer = GetLayer("RayCollide");
		if (pLayer)
		{
			for (const auto& obj : pLayer->getObjectList())
			{
				// 보이는 애 중에서
				if (!obj->GetRenderEnable())
					continue;

				// 근처에 있는 애, 혹은 NearDist보다 작은 그니까 부딪힌게 가장 가까이 있는지
				CTransform *pObjTr = obj->GetTransform();
				Vector3 vObjPos = pObjTr->GetWorldPos();

				float fDistObjToTarget = vObjPos.Distance(pArm->GetTargetPos());

				if (fDistObjToTarget > 500.f)
					continue;

				// 메쉬 정점 정보 가져옴
				CRenderer *pRenderer = obj->FindComponentFromType<CRenderer>(CT_RENDERER);
				if (pRenderer == nullptr)
					continue;
				CMesh *pMesh = pRenderer->GetMesh();

				vector<Vector3> vecObjPos;
				vector<int> vecObjIndex;
				pMesh->GetMeshWorldPosition(&vecObjPos, pObjTr->GetLocalMatrix() * pObjTr->GetWorldMatrix());
				pMesh->GetMeshIndex(&vecObjIndex);

				// 충돌하는지 검사
				Vector3 vRayCollidePos = Vector3(0.f, 0.f, 0.f);
				bool collideCheck = pPicking->Picking_ToBuffer(&vRayCollidePos, pArm->GetTargetPos(), vCameraRay, vecObjPos, vecObjIndex);

				float fDistRayToTarget = vRayCollidePos.Distance(pArm->GetTargetPos());

				// 충돌한 것이 NearDist보다 작으면 저장
				// 그리고 카메라 길이보다 작아야..
				if (collideCheck)
				{
					if (fDistRayToTarget < fNearDist &&
						fDistRayToTarget < fDistCamToTarget)
					{
						fNearDist = fDistRayToTarget;
						collideCamTargetObj = obj;
						vNearRayCollidePos = vRayCollidePos + pArm->GetLookAt() + -vCameraRay * 1.25f ;
					}
				}

				SAFE_RELEASE(pRenderer);
				SAFE_RELEASE(pMesh);
				SAFE_RELEASE(pObjTr);
			}

			// 한계치 검사
			float fLimitDist = pArm->GetTargetPos().Distance(m_pMainCameraTr->GetWorldPos());
			if (fLimitDist > 5.f)
			{
				// 가장 가까이 있는 애가 존재하면
				if (collideCamTargetObj)
				{
					// vCollidePos로 Set
					m_pMainCameraTr->SetWorldPos(vNearRayCollidePos);
				}
			}		
			SAFE_RELEASE(pLayer);
		}

		

		// Light Culling
		Vector3 vTargetPos = pArm->GetTargetPos();
		vTargetPos.y = 0.f;

		for (const auto& light : m_LightList)
		{
			CLight *pLight = light->FindComponentFromType<CLight>(CT_LIGHT);

			if (pLight->GetLightInfo().iType != LT_POINT || 
				light->GetTag() == "GlobalLight")
			{
				SAFE_RELEASE(pLight);
				continue;
			}

			CTransform *pLightTr = pLight->GetTransform();
			Vector3 vLightPos = pLightTr->GetWorldPos();
			vLightPos.y = 0.f;

			if (vTargetPos.Distance(vLightPos) > 150)
			{
				light->SetCulling(true);
			}
			else
			{
				light->SetCulling(false);
			}

			SAFE_RELEASE(pLightTr);
			SAFE_RELEASE(pLight);
		}

		SAFE_RELEASE(pPicking);
		SAFE_RELEASE(pArm);
	}

	if (m_pLightCameraObj)
		m_pLightCameraObj->Update(fTime);
	
	// 조명 업데이트
	list<CGameObject*>::iterator	iterL;
	list<CGameObject*>::iterator	iterLEnd = m_LightList.end();

	for (iterL = m_LightList.begin(); iterL != iterLEnd;)
	{
		if (!(*iterL)->GetAlive())
		{
			SAFE_RELEASE((*iterL));
			iterL = m_LightList.erase(iterL);
			continue;
		}

		else if (!(*iterL)->GetEnable())
		{
			++iterL;
			continue;
		}

		(*iterL)->Update(fTime);
		++iterL;
	}

	if (m_pSkyObject)
		m_pSkyObject->Update(fTime);

	// QuadTree Culling
	//GET_SINGLE(CQuadTreeManager)->CheckRenderingChild();

	// Sound
	GET_SINGLE(SoundManager)->Update(fTime);

	return 0;
}

int CScene::LateUpdate(float fTime)
{
	vector<CSceneScript*>::iterator	iter1;
	vector<CSceneScript*>::iterator	iter1End = m_vecSceneScript.end();

	for (iter1 = m_vecSceneScript.begin(); iter1 != iter1End;)
	{
		if (!(*iter1)->GetEnable())
		{
			++iter1;
			continue;
		}

		(*iter1)->LateUpdate(fTime);

		if (!(*iter1)->GetAlive())
		{
			SAFE_RELEASE((*iter1));
			iter1 = m_vecSceneScript.erase(iter1);
		}

		else
			++iter1;
	}

	vector<CLayer*>::iterator	iter;
	vector<CLayer*>::iterator	iterEnd = m_vecLayer.end();

	for (iter = m_vecLayer.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->LateUpdate(fTime);

		if (!(*iter)->GetAlive())
		{
			SAFE_RELEASE((*iter));
			iter = m_vecLayer.erase(iter);
		}

		else
			++iter;
	}

	m_pMainCameraObj->LateUpdate(fTime);
	if (m_pLightCameraObj)
		m_pLightCameraObj->LateUpdate(fTime);
	// 조명 업데이트
	list<CGameObject*>::iterator	iterL;
	list<CGameObject*>::iterator	iterLEnd = m_LightList.end();

	for (iterL = m_LightList.begin(); iterL != iterLEnd;)
	{
		if (!(*iterL)->GetAlive())
		{
			SAFE_RELEASE((*iterL));
			iterL = m_LightList.erase(iterL);
			continue;
		}

		else if (!(*iterL)->GetEnable())
		{
			++iterL;
			continue;
		}

		(*iterL)->LateUpdate(fTime);
		++iterL;
	}

	if (m_pSkyObject)
		m_pSkyObject->LateUpdate(fTime);

	return 0;
}

void CScene::Collision(float fTime)
{
	vector<CSceneScript*>::iterator	iter1;
	vector<CSceneScript*>::iterator	iter1End = m_vecSceneScript.end();

	for (iter1 = m_vecSceneScript.begin(); iter1 != iter1End;)
	{
		if (!(*iter1)->GetEnable())
		{
			++iter1;
			continue;
		}

		(*iter1)->Collision(fTime);

		if (!(*iter1)->GetAlive())
		{
			SAFE_RELEASE((*iter1));
			iter1 = m_vecSceneScript.erase(iter1);
		}

		else
			++iter1;
	}

	vector<CLayer*>::iterator	iter;
	vector<CLayer*>::iterator	iterEnd = m_vecLayer.end();

	for (iter = m_vecLayer.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Collision(fTime);

		if (!(*iter)->GetAlive())
		{
			SAFE_RELEASE((*iter));
			iter = m_vecLayer.erase(iter);
		}

		else
			++iter;
	}
}

void CScene::Render(float fTime)
{
	vector<CSceneScript*>::iterator	iter1;
	vector<CSceneScript*>::iterator	iter1End = m_vecSceneScript.end();

	for (iter1 = m_vecSceneScript.begin(); iter1 != iter1End;)
	{
		if (!(*iter1)->GetEnable())
		{
			++iter1;
			continue;
		}

		(*iter1)->Render(fTime);

		if (!(*iter1)->GetAlive())
		{
			SAFE_RELEASE((*iter1));
			iter1 = m_vecSceneScript.erase(iter1);
		}

		else
			++iter1;
	}

	if (m_pSkyObject)
		m_pSkyObject->Render(fTime);

	vector<CLayer*>::iterator	iter;
	vector<CLayer*>::iterator	iterEnd = m_vecLayer.end();

	for (iter = m_vecLayer.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Render(fTime);

		if (!(*iter)->GetAlive())
		{
			SAFE_RELEASE((*iter));
			iter = m_vecLayer.erase(iter);
		}

		else
			++iter;
	}
}

void CScene::Save(char * pFileName, const string & strPathKey)
{
}

void CScene::Save(FILE * pFile)
{
}

void CScene::SaveFromFullPath(const char * pFullPath)
{
}

void CScene::Load(char * pFileName, const string & strPathKey)
{
}

void CScene::Load(FILE * pFile)
{
}

void CScene::LoadFromFullPath(const char * pFullPath)
{
}

void CScene::LoadSky(const wstring & strFileName)
{
	// SkyBox
	wstring FileName = strFileName + L".dds";

	CGameObject* pSky = CGameObject::FindObject("Sky");
	if (pSky)
	{
		CRenderer*   pRenderer = pSky->FindComponentFromTag<CRenderer>("SkyRenderer");
		CMaterial*   pMaterial = pRenderer->GetMaterial();

		pMaterial->ResetTextureInfo();
		GET_SINGLE(CResourcesManager)->FindAndDeleteTexture("Sky");
		pMaterial->SetDiffuseTexInfo(SAMPLER_LINEAR, "Sky", 0, 0, (L"Skybox\\" + FileName).c_str());

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pSky);
	}
	else {
		m_pSkyObject = CGameObject::CreateObject("Sky");

		m_pSkyObject->SetScene(this);

		CTransform*	pSkyTr = m_pSkyObject->GetTransform();

		pSkyTr->SetWorldScale(50000.f, 50000.f, 50000.f);

		SAFE_RELEASE(pSkyTr);

		CRenderer*	pRenderer = m_pSkyObject->AddComponent<CRenderer>("SkyRenderer");

		pRenderer->SetMesh("Sphere");
		pRenderer->SetShader(SKY_SHADER);
		pRenderer->SetInputLayout("VertexPos");

		CMaterial*	pMaterial = pRenderer->CreateMaterial();
		wstring FileName = strFileName + L".dds";
		pMaterial->SetDiffuseTexInfo(SAMPLER_LINEAR, "Sky", 0, 0, (L"Skybox\\" + FileName).c_str());

		SAFE_RELEASE(pMaterial);

		pRenderer->SetRenderState(CULLING_NONE);
		pRenderer->SetRenderState(DEPTH_LESS_EQUAL);

		SAFE_RELEASE(pRenderer);
	}
}

void CScene::LoadGlobLight(const string & strFileName, const Vector4& ambient)
{
	string path = "Light\\Glob\\" + strFileName + ".bin";
	
	ifstream mainFile;	

	mainFile.open(GET_SINGLE(CPathManager)->FindPathToMultiByte(DATA_PATH)
		 + path, ios::in);

	if (!mainFile.is_open())
		return;

	float fRange = 0.f;
	Vector4 vLightColor = Vector4(1.f, 1.f, 1.f, 1.f);
	Vector3 vCamPos = Vector3(0.f, 0.f, 0.f);
	Vector3 vCamLookPos = Vector3(0.f, 0.f, 0.f);

	mainFile >> fRange;
	mainFile >> vLightColor.x >> vLightColor.y >> vLightColor.z;
	mainFile >> vCamPos.x >> vCamPos.y >> vCamPos.z;
	mainFile >> vCamLookPos.x >> vCamLookPos.y >> vCamLookPos.z;

	mainFile.close();

	CLight*	pGlobalLight = GetGlobalLight("GlobalLight");

	m_pLightCamera->SetLightRange(fRange);
	m_pLightCameraTr->SetWorldPos(vCamPos);
	m_pLightCamera->SetLightCenterPos(vCamLookPos);
	m_pLightCamera->SetDistLookAtToEye();
	pGlobalLight->SetLightColor(vLightColor, ambient);

	SAFE_RELEASE(pGlobalLight);
}

void CScene::LoadPointLight(const string & strFileName, const Vector4& ambient)
{
	string path = "Light\\Point\\" + strFileName + ".bin";

	ifstream mainFile;

	mainFile.open(GET_SINGLE(CPathManager)->FindPathToMultiByte(DATA_PATH)
		+ path, ios::in);

	if (!mainFile.is_open())
		return;

	// 라이트 갯수
	int count = 0;
	mainFile >> count;

	string strLightName = "";
	Vector3 vPos = Vector3(0.f, 0.f, 0.f);
	float   fRange = 0.f;
	Vector4 vColor = Vector4(1.f, 1.f, 1.f, 1.f);

	CLight *pPointLight = nullptr;
	CTransform *pTr = nullptr;

	for (int i = 0; i < count; ++i)
	{
		mainFile >>strLightName;
		mainFile >>vPos.x >> vPos.y >> vPos.z;
		mainFile >>fRange;
		mainFile >>vColor.x >> vColor.y >> vColor.z >> vColor.w;

		pPointLight = CreateLight(strLightName, LT_POINT);
		pTr = pPointLight->GetTransform();
		pPointLight->SetLightRange(fRange);
		pPointLight->SetLightColor(vColor, ambient);
		pTr->SetWorldPos(vPos);

		if (fRange <= 50.f)
		{
			CGameObject *pLightGameObject = pPointLight->GetGameObject();
			GET_SINGLE(CQuadTreeManager)->CheckAndAddChild(pLightGameObject);
			SAFE_RELEASE(pLightGameObject);
		}
			

		SAFE_RELEASE(pTr);
		SAFE_RELEASE(pPointLight);
	}

	mainFile.close();
}

CLayer * CScene::CreateLayer(const string & strTag, int iZOrder)
{
	CLayer*	pLayer = new CLayer;

	pLayer->SetTag(strTag);
	pLayer->SetZOrder(iZOrder);
	pLayer->SetScene(this);

	if (!pLayer->Init())
	{
		SAFE_RELEASE(pLayer);
		return NULL;
	}

	pLayer->AddRef();
	m_vecLayer.push_back(pLayer);

	if (m_vecLayer.size() > 1)
		sort(m_vecLayer.begin(), m_vecLayer.end(), CScene::SortZ);

	return pLayer;
}

CLayer * CScene::GetLayer(const string & strTag)
{
	vector<CLayer*>::iterator	iter;
	vector<CLayer*>::iterator	iterEnd = m_vecLayer.end();

	for (iter = m_vecLayer.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			(*iter)->AddRef();
			return *iter;
		}
	}

	return NULL;
}

bool CScene::SortZ(CLayer * p1, CLayer * p2)
{
	return p1->GetZOrder() < p2->GetZOrder();
}

#include "TestScene.h"
#include "Scene/Layer.h"
#include "GameObject/GameObject.h"
#include "Core/Input.h"
#include "Device.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/Renderer.h"
#include "Component/Transform.h"
#include "../ObjectScript/Player.h"
#include "../ObjectScript/Golem.h"
#include "../ObjectScript/Effect.h"
#include "Component/ColliderAABB.h"
#include "Component/ColliderSphere.h"
#include "Component/Material.h"
#include "Component/ThirdCamera.h"
#include "Component/Arm.h"
#include "Core/CollisionManager.h"
#include "Component/LandScape.h"
#include "Component/UIPanel.h"
#include "Component/ParticleSingle.h"
#include "Component/Animation2D.h"
#include "Component/Navigation3D.h"
#include "../ObjectScript/CharacterPanel.h"
#include "../ObjectScript/Gun.h"
#include "Component/Decal.h"
#include "Core/NavigationManager.h"
#include "Core/NavigationMesh.h"
#include "Resources/Mesh.h"
#include "../NetworkManager.h"
#include "Core.h"
#include "Device.h"
#include "Component/Renderer2D.h"
#include "Component/UIButton.h"
#include "Component/ColliderRect.h"
#include "Core/PathManager.h"
#include "Core/QuadTreeManager.h"
#include "Core/NaviManager.h"
#include "Component/Cell.h"
#include "../ObjectScript/Player_Test.h"
#include "../ObjectScript/Sword.h"
#include "Core/EffectManager.h"


CTestScene::CTestScene()
{

}

CTestScene::~CTestScene()
{

}

bool CTestScene::Init()
{
	{
		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer* pLayer = pScene->GetLayer("Default");

#pragma region Terrain
		// Load Terrain
		CGameObject* pLandScapeObj = CGameObject::CreateObject("LandScape", pLayer);
		CLandScape* pLandScape = pLandScapeObj->AddComponent<CLandScape>("LandScape");
		pLandScape->Load_Terrain("Main_Scene_1");
		SAFE_RELEASE(pLandScape);
		SAFE_RELEASE(pLandScapeObj);
#pragma endregion

#pragma region SkyAndLight
		// SkyBox, Light
		pScene->LoadSky(L"Skybox_2");
		pScene->LoadGlobLight("Shadow_Test");
		pScene->LoadPointLight("Night_Test");
#pragma endregion

		CGameObject::LoadEnvObjects(L"Main_Scene_1", pLayer);

#pragma region Navigation
		GET_SINGLE(CNaviManager)->CreateNaviMeshFromFile("Main_Scene_1");
		GET_SINGLE(CNaviManager)->SetCurNaviName("Main_Scene_1");
		GET_SINGLE(CNaviManager)->SetRenderCheck(false);
#pragma endregion

#pragma region Player
		CGameObject*	pPlayerObj = CGameObject::CreateObject("PlayerCharacter", pLayer);
		CPlayer_Test*		pPlayer = pPlayerObj->AddComponent<CPlayer_Test>("Player");

		SAFE_RELEASE(pPlayer);
		CTransform*	pTr = pPlayerObj->GetTransform();

		CRenderer*	pRenderer = pPlayerObj->FindComponentFromTag<CRenderer>("PlayerRenderer");

		Vector3 vFirstNodeCenterPos = (*GET_SINGLE(CNaviManager)->GetNaviCells("Main_Scene_1"))[0]->Get_CenterPos();

		pTr->SetWorldPos(vFirstNodeCenterPos);
		pTr->SetWorldScale(1.5f, 1.5f, 1.5f);
		pTr->SetWorldRot(0.f, 0.0f, 0.f);	

		/* Cam */
		CGameObject* pCameraObj = pScene->GetMainCameraObj();
		CThirdCamera* pThirdCam = pCameraObj->AddComponent<CThirdCamera>("ThirdCamera");
		CArm* pArm = pCameraObj->AddComponent<CArm>("Arm");

		pArm->SetTarget(pPlayerObj);
		pArm->SetLookAtDist(Vector3(0.f, 1.f, 0.f));

		//SAFE_RELEASE(pAnimation);
		SAFE_RELEASE(pArm);
		SAFE_RELEASE(pTr);
		SAFE_RELEASE(pPlayerObj);
#pragma endregion

#pragma region Sword
		// 따로 Class 필요
		CGameObject *pSwordObj = CGameObject::CreateObject("Sword_Player", pLayer);
		CSword	*pSword = pSwordObj->AddComponent<CSword>("Sword");
		pSword->initialize();
		SAFE_RELEASE(pSword);
		SAFE_RELEASE(pSwordObj);
#pragma endregion

		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);


#pragma region Effect
		GET_SINGLE(CEffectManager)->AddEffect("Portal", "Effect\\Portal.bin");
		GET_SINGLE(CEffectManager)->OperateEffect("Portal", nullptr, Vector3(29.f, 0.f, 271.f));
		GET_SINGLE(CEffectManager)->OperateEffect("Portal", nullptr, Vector3(78.f, 0.f, 95.f));
#pragma endregion
	}



	return true;
}

int CTestScene::Update(float fTime)
{
	if (m_isFirstCheck)
	{
		/*GET_SINGLE(CEffectManager)->OperateEffect("Portal", nullptr, Vector3(29.f, 0.f, 271.f));
		GET_SINGLE(CEffectManager)->OperateEffect("Portal", nullptr, Vector3(78.f, 0.f, 95.f));*/
		m_isFirstCheck = false;
	}
	
	return 0;
}

void CTestScene::Input(float fTime)
{
	if (KEYDOWN("F1"))
	{
		CGameObject* pLandScapeObj = CGameObject::FindObject("LandScape");

		if (pLandScapeObj)
		{
			CLandScape* pLandScape = pLandScapeObj->FindComponentFromTag<CLandScape>("LandScape");
			list<QUADTREENODE*>* nodes = pLandScape->GetAllNodes();
			if (!m_isCheckColliderQuadTree)
			{
				for (auto& iter : *nodes)
				{
					CColliderAABB *pCollider = iter->pGameObject->FindComponentFromTag<CColliderAABB>("Collider");
					pCollider->SetColliderRenderCheck(true);
					SAFE_RELEASE(pCollider);
				}
				m_isCheckColliderQuadTree = true;
			}
			else
			{
				for (auto& iter : *nodes)
				{
					CColliderAABB *pCollider = iter->pGameObject->FindComponentFromTag<CColliderAABB>("Collider");
					pCollider->SetColliderRenderCheck(false);
					SAFE_RELEASE(pCollider);
				}

				m_isCheckColliderQuadTree = false;
			}
			SAFE_RELEASE(pLandScape);
			SAFE_RELEASE(pLandScapeObj);
		}

		//GET_SINGLE(CEffectManager)->OperateEffect("Portal", nullptr, Vector3(78.f, 0.f, 95.f));
	}

	if (KEYDOWN("F2"))
	{
		if (!m_isCheckColliderNaviMesh)
		{
			GET_SINGLE(CNaviManager)->SetRenderCheck(true);
			m_isCheckColliderNaviMesh = true;
		}
		else
		{
			GET_SINGLE(CNaviManager)->SetRenderCheck(false);
			m_isCheckColliderNaviMesh = false;
		}
	}
}

int CTestScene::LateUpdate(float fTime)
{
	return 0;
}

void CTestScene::Render(float fTime)
{
}

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
#include "Core/SoundManager.h"

#define DUNGEON
//#define SCENE_1
//#define SCENE_2

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

#ifdef SCENE_1
		pLandScape->Load_Terrain("Main_Scene_1");
#endif

#ifdef SCENE_2
		pLandScape->Load_Terrain("Main_Scene_2_DY");
#endif

#ifdef DUNGEON
		pLandScape->Load_Terrain("dungeon_scene");
#endif
		SAFE_RELEASE(pLandScape);
		SAFE_RELEASE(pLandScapeObj);
#pragma endregion

#pragma region SkyAndLight
		// SkyBox, Light

#ifdef SCENE_1
		pScene->LoadSky(L"Skybox_6");
		pScene->LoadGlobLight("Main_Scene_1");
#endif

#ifdef SCENE_2
		pScene->LoadSky(L"Skybox_18");
		pScene->LoadGlobLight("Main_Scene_2_DY");
		pScene->LoadPointLight("Main_Scene_2_DY_Lights");
#endif

#ifdef DUNGEON
		pScene->LoadSky(L"Skybox_2");
		pScene->LoadGlobLight("dungeon_scene", Vector4(0.2f, 0.2f, 0.2f, 1.f));
		pScene->LoadPointLight("dungeon_scene", Vector4(0.2f, 0.2f, 0.2f, 1.f));
#endif
		
		
#pragma endregion

#ifdef SCENE_1
		CGameObject::LoadEnvObjects(L"Main_Scene_1", pLayer);
#endif

#ifdef SCENE_2
		CGameObject::LoadEnvObjects(L"Main_Scene_2_DY", pLayer);
#endif
	
#ifdef DUNGEON
		CLayer* pLayer2 = pScene->CreateLayer("RayCollide");
		CGameObject::LoadEnvObjects(L"dungeon_scene_collision", pLayer2, false);
		CGameObject::LoadEnvObjects(L"dungeon_scene", pLayer);
#endif

#pragma region Navigation
		string naviName = "";

#ifdef SCENE_1
		naviName = "Main_Scene_1";
#endif

#ifdef SCENE_2
		naviName = "Main_Scene_2_DY";
#endif

#ifdef DUNGEON
		naviName = "dungeon_scene";
#endif

		GET_SINGLE(CNaviManager)->CreateNaviMeshFromFile(naviName);
		GET_SINGLE(CNaviManager)->SetCurNaviName(naviName);
		GET_SINGLE(CNaviManager)->SetRenderCheck(false);
#pragma endregion

#pragma region Player
		CGameObject*	pPlayerObj = CGameObject::CreateObject("PlayerCharacter", pLayer);
		CPlayer_Test*		pPlayer = pPlayerObj->AddComponent<CPlayer_Test>("Player");

		SAFE_RELEASE(pPlayer);
		CTransform*	pTr = pPlayerObj->GetTransform();

		//
		GET_SINGLE(SoundManager)->SetPlayerTr(pTr);

		CRenderer*	pRenderer = pPlayerObj->FindComponentFromTag<CRenderer>("PlayerRenderer");

		Vector3 vFirstNodeCenterPos = (*GET_SINGLE(CNaviManager)->GetNaviCells(naviName))[0]->Get_CenterPos();

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

#ifdef SCENE_1
		GET_SINGLE(CEffectManager)->AddEffect("Portal", "Effect\\Portal.bin");
		GET_SINGLE(CEffectManager)->OperateEffect("Portal", nullptr, Vector3(29.f, 0.f, 271.f));
		GET_SINGLE(CEffectManager)->OperateEffect("Portal", nullptr, Vector3(78.f, 0.f, 95.f));
#endif
		
#ifdef DUNGEON
		GET_SINGLE(CEffectManager)->AddEffect("Fire_Tall_Dark", "Effect\\Common\\Fire_tall_dark.bin");
		GET_SINGLE(CEffectManager)->OperateEffect("Fire_Tall_Dark", nullptr, Vector3(33.8f, 14.9f, 19.5f));
		GET_SINGLE(CEffectManager)->OperateEffect("Fire_Tall_Dark", nullptr, Vector3(14.75f, 14.9f, 18.49f));
		GET_SINGLE(CEffectManager)->OperateEffect("Fire_Tall_Dark", nullptr, Vector3(34.22f, 14.9f, 74.74f));
		GET_SINGLE(CEffectManager)->OperateEffect("Fire_Tall_Dark", nullptr, Vector3(15.82f, 14.9f, 76.95f));
		GET_SINGLE(CEffectManager)->OperateEffect("Fire_Tall_Dark", nullptr, Vector3(46.81f, 14.9f, 136.96f));
		GET_SINGLE(CEffectManager)->OperateEffect("Fire_Tall_Dark", nullptr, Vector3(47.67f, 14.9f, 110.71f));
		GET_SINGLE(CEffectManager)->OperateEffect("Fire_Tall_Dark", nullptr, Vector3(71.53f, 14.9f, 136.65f));
		GET_SINGLE(CEffectManager)->OperateEffect("Fire_Tall_Dark", nullptr, Vector3(72.16f, 14.9f, 110.67f));
		GET_SINGLE(CEffectManager)->OperateEffect("Fire_Tall_Dark", nullptr, Vector3(102.12f, 14.9f, 144.09f));
		GET_SINGLE(CEffectManager)->OperateEffect("Fire_Tall_Dark", nullptr, Vector3(124.04f, 14.9f, 162.20f));
		GET_SINGLE(CEffectManager)->OperateEffect("Fire_Tall_Dark", nullptr, Vector3(150.75f, 14.9f, 189.65f));
		GET_SINGLE(CEffectManager)->OperateEffect("Fire_Tall_Dark", nullptr, Vector3(173.61f, 14.9f, 153.99f));
		GET_SINGLE(CEffectManager)->OperateEffect("Fire_Tall_Dark", nullptr, Vector3(179.18f, 14.9f, 243.04f));
		GET_SINGLE(CEffectManager)->OperateEffect("Fire_Tall_Dark", nullptr, Vector3(206.35f, 14.9f, 243.01f));
		GET_SINGLE(CEffectManager)->OperateEffect("Fire_Tall_Dark", nullptr, Vector3(194.30f, 14.9f, 205.99f));
		GET_SINGLE(CEffectManager)->OperateEffect("Fire_Tall_Dark", nullptr, Vector3(226.91f, 14.9f, 207.47f));
		GET_SINGLE(CEffectManager)->OperateEffect("Fire_Tall_Dark", nullptr, Vector3(227.09f, 14.9f, 193.04f));
		GET_SINGLE(CEffectManager)->OperateEffect("Fire_Tall_Dark", nullptr, Vector3(260.92f, 14.9f, 207.50f));
		GET_SINGLE(CEffectManager)->OperateEffect("Fire_Tall_Dark", nullptr, Vector3(261.69f, 14.9f, 193.25f));
		GET_SINGLE(CEffectManager)->OperateEffect("Fire_Tall_Dark", nullptr, Vector3(324.48f, 14.9f, 226.13f));
		GET_SINGLE(CEffectManager)->OperateEffect("Fire_Tall_Dark", nullptr, Vector3(369.84f, 14.9f, 225.68f));
		GET_SINGLE(CEffectManager)->OperateEffect("Fire_Tall_Dark", nullptr, Vector3(369.94f, 14.9f, 173.79f));
		GET_SINGLE(CEffectManager)->OperateEffect("Fire_Tall_Dark", nullptr, Vector3(324.01, 14.9f, 174.36f));

		GET_SINGLE(CEffectManager)->AddEffect("Big_Fire", "Effect\\Common\\BigFire01.bin");
		GET_SINGLE(CEffectManager)->OperateEffect("Big_Fire", nullptr, Vector3(17.4f, 2.25f, 131.64f));
		GET_SINGLE(CEffectManager)->OperateEffect("Big_Fire", nullptr, Vector3(65.f, 2.4f, 183.87f));

		GET_SINGLE(CEffectManager)->AddEffect("Fire_Bright", "Effect\\Common\\Fire_Bright.bin");
		GET_SINGLE(CEffectManager)->OperateEffect("Fire_Bright", nullptr, Vector3(93.61f, 6.75f, 138.01f));
		GET_SINGLE(CEffectManager)->OperateEffect("Fire_Bright", nullptr, Vector3(161.83f, 6.75f, 139.08f));
		GET_SINGLE(CEffectManager)->OperateEffect("Fire_Bright", nullptr, Vector3(133.80f, 6.75f, 173.73f));
		GET_SINGLE(CEffectManager)->OperateEffect("Fire_Bright", nullptr, Vector3(124.08f, 6.75f, 188.72f));
#endif

#pragma endregion
	}


#pragma region sound
#ifdef SCENE_1
	GET_SINGLE(SoundManager)->LoadSound("TownBGM", true, "Main_Scene_1_Town.mp3");
	GET_SINGLE(SoundManager)->AddSoundArea("TownBGM", Vector3(335.f, 0.f, 358.f), 10.f);
	GET_SINGLE(SoundManager)->LoadSound("FieldBGM", true, "Main_Scene_1_Field.mp3");
	GET_SINGLE(SoundManager)->AddSoundArea("FieldBGM", Vector3(316.f, 0.f, 343.f), 10.f);
	GET_SINGLE(SoundManager)->Play("TownBGM", SC_BGM);
#endif

#ifdef SCENE_2
	GET_SINGLE(SoundManager)->LoadSound("Scene2BGM", true, "Main_Scene_2.mp3");
	GET_SINGLE(SoundManager)->Play("Scene2BGM", SC_BGM);
#endif

#ifdef DUNGEON
	GET_SINGLE(SoundManager)->LoadSound("DungeonBGM", true, "Dungeon_Scene.mp3");
	GET_SINGLE(SoundManager)->LoadSound("BossBGM", true, "Boss.mp3");
	GET_SINGLE(SoundManager)->AddSoundArea("BossBGM", Vector3(295.f, 0.f, 200.f), 22.5f);
	GET_SINGLE(SoundManager)->Play("DungeonBGM", SC_BGM);
#endif
#pragma endregion

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

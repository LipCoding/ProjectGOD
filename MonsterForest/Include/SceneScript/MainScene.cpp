#include "MainScene.h"
#include "Scene/Layer.h"
#include "GameObject/GameObject.h"
#include "Core/Input.h"
#include "Device.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "TownScene.h"
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

CMainScene::CMainScene()
{
}

CMainScene::~CMainScene()
{
}

bool CMainScene::Init()
{
	CLayer*	pLayer = m_pScene->GetLayer("Default");
	{
		//Lamp
		CGameObject*    pLampObj = CGameObject::CreateObject("Lamp", pLayer);
		CTransform*     pLampTr = pLampObj->GetTransform();

		pLampTr->SetPivot(0.f, 0.f, 0.f);
		pLampTr->RotateLocalX(-PG_PI / 2.f);
		pLampTr->SetLocalScale(0.2f, 0.2f, 0.2f);
		pLampTr->RotateLocalY(PG_PI / (rand() % 5));
		pLampTr->SetWorldPos(Vector3{ 15.f, 0.f, 50.f });

		SAFE_RELEASE(pLampTr);

		CRenderer* pLampRenderer = pLampObj->AddComponent<CRenderer>("LampRenderer");
		CNavigation3D* pLampNavigation = pLampObj->AddComponent<CNavigation3D>("Navigation");

		pLampRenderer->SetMesh("Lamp", L"Lamp.FBX");
		SAFE_RELEASE(pLampRenderer);
		SAFE_RELEASE(pLampObj);
		SAFE_RELEASE(pLampNavigation);
	}

	CGameObject*	pPlayerObj = CGameObject::CreateObjectDontDestroy("Player", pLayer);

	CTransform*	pTr = pPlayerObj->GetTransform();

	pTr->SetWorldPos(159.f, 0.f, 136.f);
	pTr->SetWorldScale(2.f, 2.f, 2.f);
	pTr->SetWorldRot(0.f, 0.7f, 0.f);
	SAFE_RELEASE(pTr);

	CRenderer*	pRenderer = pPlayerObj->AddComponent<CRenderer>("PlayerRenderer");
	
	pRenderer->SetMesh("Player", L"Player2.msh");
	pRenderer->SetForwardShader();

	CMaterial*	pMaterial = NULL;

	SAFE_RELEASE(pRenderer);

	CPlayer*	pPlayer = pPlayerObj->AddComponent<CPlayer>("Player");

	SAFE_RELEASE(pPlayer);

	CColliderSphere*	pSphere = pPlayerObj->AddComponent<CColliderSphere>("PlayerBody");
	pSphere->SetSphere(Vector3(0.f, 1.f, 0.f), 1.5f);

	SAFE_RELEASE(pSphere);

	CGameObject*	pCameraObj = m_pScene->GetMainCameraObj();

	CThirdCamera*	pThirdCam = pCameraObj->AddComponent<CThirdCamera>("ThirdCamera");

	SAFE_RELEASE(pThirdCam);

	CArm*	pArm = pCameraObj->AddComponent<CArm>("Arm");

	pArm->SetTarget(pPlayerObj);
	pArm->SetLookAtDist(Vector3(0.f, 1.f, 0.f));

	SAFE_RELEASE(pArm);

	SAFE_RELEASE(pCameraObj);

	////무기
	//CGameObject*    pSwordObj = CGameObject::CreateObject("Sword", pLayer);
	//CTransform*     pSwordTr = pSwordObj->GetTransform();

	//pSwordTr->SetLocalScale(Vector3(3.f, 3.f, 3.f));
	//pSwordTr->SetWorldPos(Vector3(66.f, 0.f, 170.f));

	//SAFE_RELEASE(pSwordTr);

	//CRenderer* pSwordRenderer = pSwordObj->AddComponent<CRenderer>("SwordRenderer");
	//CNavigation3D* pSwordNavigation = pSwordObj->AddComponent<CNavigation3D>("Navigation");

	//pSwordRenderer->SetMesh("Sword", L"Sword.FBX");

	////CColliderSphere*	pSphere = pPlayerObj->AddComponent<CColliderSphere>("PlayerBody");

	//
	//SAFE_RELEASE(pSwordRenderer);
	//SAFE_RELEASE(pSwordNavigation);
	//SAFE_RELEASE(pSwordObj);
	///
	//CGameObject*	pGolemPrototype = CGameObject::CreatePrototypeDontDestroy(
	//	"BigGoblin", m_pScene);

	//pTr = pGolemPrototype->GetTransform();

	////pTr->SetWorldRot(PG_PI / 4.f, PG_PI / 3.f, 0.f);
	////pTr->SetWorldPos(-3.f, 2.f, 3.f);
	//pTr->SetLocalRotY(PG_PI / 2.f);
	//pTr->SetWorldPos(20.f, 0.f, 20.f);

	//SAFE_RELEASE(pTr);

	//pRenderer = pGolemPrototype->AddComponent<CRenderer>("BigGoblinRenderer");

	//pRenderer->SetMesh("Monster", L"Monster.msh");

	//SAFE_RELEASE(pRenderer);

	//Golem*	pGolem = pGolemPrototype->AddComponent<Golem>("Golem");

	//pGolem->GetAnimation()->Load("Player.anm");
	//pGolem->SetTarget("Player");

	//SAFE_RELEASE(pGolem);

	//pSphere = pGolemPrototype->AddComponent<CColliderSphere>("GoblinBody1");

	//pSphere->SetSphere(Vector3(0.f, 0.f, 0.f), 0.8f);

	//SAFE_RELEASE(pSphere);

	//SAFE_RELEASE(pGolemPrototype);

	GET_SINGLE(CCollisionManager)->ModifyGroup("Default", 10, 10, 10, Vector3(10.f, 10.f, 10.f));

#pragma region Terrain
	CGameObject*	pLandScapeObj = CGameObject::CreateObject("LandScape", pLayer);

	CTransform*	pLandTr = pLandScapeObj->GetTransform();

	SAFE_RELEASE(pLandTr);

	CLandScape*	pLandScape = pLandScapeObj->AddComponent<CLandScape>("LandScape");

	pLandScape->CreateLandScape("LandScapeMesh", 513, false, "LandScape",
		L"LandScape/GRASS_00+SAND.dds",
		L"LandScape/GRASS_00+SAND_NRM.png", 
		L"LandScape/GRASS_00+SAND_SPEC.png",
		"LandScape/Height2.bmp");

	vector<wstring>	vecSplatting;

	wchar_t	strSplatPath[MAX_PATH] = {};

	wsprintf(strSplatPath, L"LandScape/BD_Terrain_Cliff05.dds");
	vecSplatting.push_back(strSplatPath);

	memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);
	wsprintf(strSplatPath, L"LandScape/Terrain_Pebbles_01.dds");
	vecSplatting.push_back(strSplatPath);

	vecSplatting.clear();
	memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);
	wsprintf(strSplatPath, L"LandScape/Terrain_Cliff_15_Large.dds");
	vecSplatting.push_back(strSplatPath);

	pLandScape->SetDiffuseSplatting("Linear", "SplatDif", &vecSplatting);

	vecSplatting.clear();
	memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);
	wsprintf(strSplatPath, L"LandScape/BD_Terrain_Cliff05_NRM.bmp");
	vecSplatting.push_back(strSplatPath);

	memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);
	wsprintf(strSplatPath, L"LandScape/Terrain_Pebbles_01_NRM.bmp");
	vecSplatting.push_back(strSplatPath);

	memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);
	wsprintf(strSplatPath, L"LandScape/Terrain_Cliff_15_Large_NRM.bmp");
	vecSplatting.push_back(strSplatPath);

	pLandScape->SetNormalSplatting("Linear", "SplatNormal", &vecSplatting);

	vecSplatting.clear();
	memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);
	wsprintf(strSplatPath, L"LandScape/BD_Terrain_Cliff05_SPEC.bmp");
	vecSplatting.push_back(strSplatPath);

	memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);
	wsprintf(strSplatPath, L"LandScape/Terrain_Pebbles_01_SPEC.bmp");
	vecSplatting.push_back(strSplatPath);

	memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);
	wsprintf(strSplatPath, L"LandScape/Terrain_Cliff_15_Large_SPEC.bmp");
	vecSplatting.push_back(strSplatPath);

	pLandScape->SetSpecularSplatting("Linear", "SplatSpecular", &vecSplatting);

	vecSplatting.clear();
	memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);
	wsprintf(strSplatPath, L"LandScape/RoadAlpha.bmp");
	vecSplatting.push_back(strSplatPath);

	memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);
	wsprintf(strSplatPath, L"LandScape/SandBaseAlpha.bmp");
	vecSplatting.push_back(strSplatPath);

	memset(strSplatPath, 0, sizeof(wchar_t) * MAX_PATH);
	wsprintf(strSplatPath, L"LandScape/WaterBaseAlpha.bmp");
	vecSplatting.push_back(strSplatPath);

	pLandScape->SetSplattingAlpha("Linear", "SplatAlpha", 15, 11, &vecSplatting);

	SAFE_RELEASE(pLandScape);

	pRenderer = pLandScapeObj->FindComponentFromType<CRenderer>(CT_RENDERER);

	pMaterial = pRenderer->GetMaterial();


	// 지형 메쉬의 정점 정보를 이용해서 네비게이션 메쉬를 구성한다.
	CNavigationMesh*	pNaviMesh = GET_SINGLE(CNavigationManager)->CreateNavigationMesh("LandScape");
	//pNaviMesh->Load("MonsterNavigation.nav");
#pragma endregion

#pragma region EnviornmentObj
	vector<Vector3> tmp_vec_pos = 
	{
		Vector3{111.f, 0.f , 71.f},
		Vector3{148.f, 0.f, 87.f},
		Vector3{194.f, 0.f, 142.f},
		Vector3{179.f, 0.f, 287.f},
		Vector3{185.f, 0.f, 190.f},
		Vector3{225.f, 0.f, 230.f},
		Vector3{205.f, 0.f, 338.f},
		Vector3{277.f, 0.f, 332.f}
	};

	pLayer = m_pScene->GetLayer("Default");
	for (int i = 0; i < tmp_vec_pos.size(); i++)
	{
		int house_type = rand() % 2;

		//house
		CGameObject*    pHouseObj = CGameObject::CreateObject("House", pLayer);
		CTransform*     pHouseTr = pHouseObj->GetTransform();

		CRenderer* pHouseRenderer = pHouseObj->AddComponent<CRenderer>("HouseRenderer");
		CNavigation3D* pHouseNavigation = pHouseObj->AddComponent<CNavigation3D>("Navigation");

		switch (house_type)
		{
		case 0:
			pHouseTr->SetPivot(0.f, 0.f, 0.f);
			pHouseTr->SetLocalScale(2.5f, 2.5f, 2.5f);
			pHouseTr->SetLocalRotX(-PG_PI / 2.f);
			pHouseRenderer->SetMesh("House1", L"House_1.FBX");
			break;
		case 1:
			pHouseTr->SetPivot(0.f, 0.f, 0.f);
			pHouseTr->SetLocalScale(0.2f, 0.2f, 0.2f);
			pHouseRenderer->SetMesh("House", L"House.FBX");
			break;
			
		}

		
		pHouseTr->RotateLocalY(PG_PI / (rand() % 9));
		pHouseTr->SetWorldPos(tmp_vec_pos[i]);

		SAFE_RELEASE(pHouseTr);
		SAFE_RELEASE(pHouseRenderer);
		SAFE_RELEASE(pHouseObj);
		SAFE_RELEASE(pHouseNavigation);
	}

	for (int i = 0; i < 100; i++)
	{
		int tree_type = rand() % 2;

		//Tree
		CGameObject*    pTreeObj = CGameObject::CreateObject("Tree", pLayer);
		CTransform*     pTreeTr = pTreeObj->GetTransform();

		CRenderer* pTreeRenderer = pTreeObj->AddComponent<CRenderer>("TreeRenderer");
		CNavigation3D* pTreeNavigation = pTreeObj->AddComponent<CNavigation3D>("Navigation");
		
		switch (tree_type)
		{
		case 0:
			pTreeTr->SetPivot(0.f, 0.f, 0.f);
			pTreeTr->SetLocalScale(0.05f, 0.05f, 0.05f);
			pTreeRenderer->SetMesh("Tree1", L"Tree_1.FBX");
			break;
		case 1:
			pTreeTr->SetPivot(0.f, -0.65f, 0.f);
			pTreeTr->SetLocalScale(0.04f, 0.04f, 0.04f);
			pTreeRenderer->SetMesh("Tree2", L"Tree_2.FBX");
			break;
		/*case 2:
			pTreeTr->SetPivot(0.f, -0.5f, 0.f);
			pTreeTr->SetLocalScale(0.25f, 0.25f, 0.25f);
			pTreeRenderer->SetMesh("Tree3", L"Tree_3.FBX");
			break;*/
		}

		pTreeTr->SetWorldPos(80 + rand() % (360 + 1), 0.f, 80 + rand() % (360 + 1));
		pTreeTr->RotateLocalY(PG_PI / (rand() % 9));

		SAFE_RELEASE(pTreeTr);
		SAFE_RELEASE(pTreeRenderer);
		SAFE_RELEASE(pTreeObj);
		SAFE_RELEASE(pTreeNavigation);
	}

	vector<Vector3> tmp_vec_pos2 =
	{
		Vector3{109.f, 0.f , 52.f},
		Vector3{120.f, 0.f, 111.f},
		Vector3{154.f, 0.f, 147.f},
		Vector3{212.f, 0.f, 189.f},
		Vector3{174.f, 0.f, 257.f},
		Vector3{217.f, 0.f, 285.f},
		Vector3{261.f, 0.f, 229.f}
	};

	for (int i = 0; i < tmp_vec_pos2.size(); i++)
	{
		//Lamp
		CGameObject*    pLampObj = CGameObject::CreateObject("Lamp", pLayer);
		CTransform*     pLampTr = pLampObj->GetTransform();

		pLampTr->SetPivot(0.f, 0.f, 0.f);
		pLampTr->RotateLocalX(-PG_PI / 2.f);
		pLampTr->SetLocalScale(0.2f, 0.2f, 0.2f);
		pLampTr->RotateLocalY(PG_PI / (rand() % 5));
		pLampTr->SetWorldPos(tmp_vec_pos2[i]);

		SAFE_RELEASE(pLampTr);

		CRenderer* pLampRenderer = pLampObj->AddComponent<CRenderer>("LampRenderer");
		CNavigation3D* pLampNavigation = pLampObj->AddComponent<CNavigation3D>("Navigation");

		pLampRenderer->SetMesh("Lamp", L"Lamp.FBX");

		SAFE_RELEASE(pLampRenderer);
		SAFE_RELEASE(pLampObj);
		SAFE_RELEASE(pLampNavigation);
	}

	vector<Vector3> tmp_vec_pos3 =
	{
		Vector3{133.f, -1.78f , 67.f},
		Vector3{133.f, -1.67f, 72.f},
		Vector3{133.f, -1.67f, 79.f},
		Vector3{135.f, 2.92f, 120.f},
		Vector3{144.f, 3.16f, 123.f},
		Vector3{158.f, 4.47f, 125.f},
		Vector3{171.f, 12.24f, 149.f},
		Vector3{167.f, 15.74f, 160.f},
		Vector3{176.f, 15.51f, 162.f},
		Vector3{202.f, 11.34f, 162.f},
		Vector3{204.f, 11.54f, 172.f},
		Vector3{206.f, 11.39f, 179.f},
		Vector3{216.f, 9.80f, 214.f},
		Vector3{195.f, 8.30f, 207.f},
		Vector3{189.f, 8.18f, 177.f},
		Vector3{192.f, 8.13f, 187.f},
		Vector3{191.f, 14.08f, 181.f}
	};

	for (int i = 0; i < tmp_vec_pos3.size(); i++)
	{
		//wood wall
		CGameObject*    pWallObj = CGameObject::CreateObject("Wall", pLayer);
		CTransform*     pWallTr = pWallObj->GetTransform();

		pWallTr->SetLocalScale(3.25f, 3.25f, 3.25f);
		pWallTr->SetLocalRotY(tmp_vec_pos3[i].y);
		pWallTr->SetWorldPos(tmp_vec_pos3[i] - Vector3{0.f, tmp_vec_pos3[i].y,0.f});

		SAFE_RELEASE(pWallTr);

		CRenderer* pWallRenderer = pWallObj->AddComponent<CRenderer>("WallRenderer");
		CNavigation3D* pWallNavigation = pWallObj->AddComponent<CNavigation3D>("Navigation");

		pWallRenderer->SetMesh("Wall", L"Fence_Wood.msh");

		SAFE_RELEASE(pWallRenderer);
		SAFE_RELEASE(pWallObj);
		SAFE_RELEASE(pWallNavigation);
	}
#pragma endregion

#pragma region MonsterArea1
	{
		vector<Vector3> initial_monster_pos_cont =
		{
			Vector3{ 270.f, CNavigationManager::GetInst()->GetY(Vector3{ 270.f, 0.f, 170.f }), 170.f },
		/*	Vector3{ 280.f, CNavigationManager::GetInst()->GetY(Vector3{ 280.f, 0.f, 175.f }), 175.f },
			Vector3{ 270.f, CNavigationManager::GetInst()->GetY(Vector3{ 270.f, 0.f, 172.f }), 172.f },
			Vector3{ 275.f, CNavigationManager::GetInst()->GetY(Vector3{ 275.f, 0.f, 170.f }), 170.f }*/
		};
		int index = 0;
		char str[64];
		for (const auto& pos : initial_monster_pos_cont)
		{
			string appendTag = _itoa(index, str, 10);
			string objectTag = "Golem" + appendTag;
			CGameObject*	pBigGoblinObj = CGameObject::CreateObject(
				objectTag, pLayer);

			pTr = pBigGoblinObj->GetTransform();
			pTr->SetLocalScale(1.f, 1.f, 1.f);
			pTr->SetLocalRotX(-PG_PI / 2.f);
			//pTr->SetWorldPos(200.f, 0.f, 200.f);
			pTr->SetWorldPos(pos.x, 0.f, pos.z);
			SAFE_RELEASE(pTr);

			pRenderer = pBigGoblinObj->AddComponent<CRenderer>("GolemRenderer");

			pRenderer->SetMesh(objectTag, L"Golem.msh");

			SAFE_RELEASE(pRenderer);

			auto pGolem = pBigGoblinObj->AddComponent<Golem>(objectTag);
			pGolem->setInitialPos(pos);
			pGolem->setRoamingPos(Vector3{ pos.x + (rand() % 25 - 25), pos.y, pos.z + (rand() % 25 - 25) });
			pGolem->SetTarget("Player");
			pGolem->GetAnimation()->Load("Golem.anm");

			SAFE_RELEASE(pGolem);

			pSphere = pBigGoblinObj->AddComponent<CColliderSphere>("GolemBody");

			//pSphere->SetAABB(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));
			pSphere->SetSphere(Vector3(0.f, 1.f, 0.f), 3.f);

			SAFE_RELEASE(pSphere);

			SAFE_RELEASE(pBigGoblinObj);
			index++;
		}
	}
#pragma endregion

#pragma region MonsterArea2
	{
		vector<Vector3> initial_monster_pos_cont =
		{
			Vector3{ 170.f, CNavigationManager::GetInst()->GetY(Vector3{ 170.f, 0.f, 270.f }), 270.f },
		/*	Vector3{ 180.f, CNavigationManager::GetInst()->GetY(Vector3{ 180.f, 0.f, 275.f }), 275.f },
			Vector3{ 170.f, CNavigationManager::GetInst()->GetY(Vector3{ 170.f, 0.f, 272.f }), 272.f },
			Vector3{ 175.f, CNavigationManager::GetInst()->GetY(Vector3{ 175.f, 0.f, 270.f }), 270.f }*/
		};
		int index = 0;
		char str[64];
		for (const auto& pos : initial_monster_pos_cont)
		{
			string appendTag = _itoa(index, str, 10);
			string objectTag = "Golem" + appendTag;
			CGameObject*	pBigGoblinObj = CGameObject::CreateObject(
				objectTag, pLayer);

			pTr = pBigGoblinObj->GetTransform();
			pTr->SetLocalScale(1.f, 1.f, 1.f);
			pTr->SetLocalRotX(-PG_PI / 2.f);
			//pTr->SetWorldPos(200.f, 0.f, 200.f);
			pTr->SetWorldPos(pos.x, 0.f, pos.z);
			SAFE_RELEASE(pTr);

			pRenderer = pBigGoblinObj->AddComponent<CRenderer>("GolemRenderer");

			pRenderer->SetMesh(objectTag, L"Golem.msh");

			SAFE_RELEASE(pRenderer);

			auto pGolem = pBigGoblinObj->AddComponent<Golem>(objectTag);
			pGolem->setInitialPos(pos);
			pGolem->setRoamingPos(Vector3{ pos.x + (rand() % 25 - 25), pos.y, pos.z + (rand() % 25 - 25) });
			pGolem->SetTarget("Player");
			pGolem->GetAnimation()->Load("Golem.anm");

			SAFE_RELEASE(pGolem);

			pSphere = pBigGoblinObj->AddComponent<CColliderSphere>("GolemBody");

			//pSphere->SetAABB(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));
			pSphere->SetSphere(Vector3(0.f, 1.f, 0.f), 3.f);

			SAFE_RELEASE(pSphere);

			SAFE_RELEASE(pBigGoblinObj);
			index++;
		}
	}
#pragma endregion

#pragma region MonsterArea3
	{
		vector<Vector3> initial_monster_pos_cont =
		{
			Vector3{ 400.f, CNavigationManager::GetInst()->GetY(Vector3{ 400.f, 0.f, 387.f }), 387.f },
		/*	Vector3{ 390.f, CNavigationManager::GetInst()->GetY(Vector3{ 390.f, 0.f, 375.f }), 375.f },
			Vector3{ 395.f, CNavigationManager::GetInst()->GetY(Vector3{ 395.f, 0.f, 382.f }), 382.f },
			Vector3{ 405.f, CNavigationManager::GetInst()->GetY(Vector3{ 405.f, 0.f, 370.f }), 370.f }*/
		};
		int index = 0;
		char str[64];
		for (const auto& pos : initial_monster_pos_cont)
		{
			string appendTag = _itoa(index, str, 10);
			string objectTag = "Golem" + appendTag;
			CGameObject*	pBigGoblinObj = CGameObject::CreateObject(
				objectTag, pLayer);

			pTr = pBigGoblinObj->GetTransform();
			pTr->SetLocalScale(1.f, 1.f, 1.f);
			pTr->SetLocalRotX(-PG_PI / 2.f);
			//pTr->SetWorldPos(200.f, 0.f, 200.f);
			pTr->SetWorldPos(pos.x, 0.f, pos.z);
			SAFE_RELEASE(pTr);

			pRenderer = pBigGoblinObj->AddComponent<CRenderer>("GolemRenderer");

			pRenderer->SetMesh(objectTag, L"Golem.msh");

			SAFE_RELEASE(pRenderer);

			auto pGolem = pBigGoblinObj->AddComponent<Golem>(objectTag);
			pGolem->setInitialPos(pos);
			pGolem->setRoamingPos(Vector3{ pos.x + (rand() % 25 - 25), pos.y, pos.z + (rand() % 25 - 25) });
			pGolem->SetTarget("Player");
			pGolem->GetAnimation()->Load("Golem.anm");

			SAFE_RELEASE(pGolem);

			pSphere = pBigGoblinObj->AddComponent<CColliderSphere>("GolemBody");

			//pSphere->SetAABB(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));
			pSphere->SetSphere(Vector3(0.f, 1.f, 0.f), 3.f);

			SAFE_RELEASE(pSphere);

			SAFE_RELEASE(pBigGoblinObj);
			index++;
		}
	}
#pragma endregion

#pragma region Effect
	// 클래스 따로 만드는게 나을듯
	CLayer*	pParticleLayer = m_pScene->CreateLayer("ParticleLayer", 10);

	//for (int i = 0; i < 10; ++i)
	//{
	//	CGameObject*	pParticleObj = CGameObject::CreateObject(
	//		"ParticleObj", pParticleLayer);

	//	CTransform*	pParticleTr = pParticleObj->GetTransform();

	//	pParticleTr->SetWorldPos(66.f, 0.f, 170.f);
	//	
	//	SAFE_RELEASE(pParticleTr);

	//	Effect*	pEffect = pParticleObj->AddComponent<Effect>("ParticleObj");

	//	pEffect->SetGameObject(pParticleObj);

	//	CParticleSingle*	pParticleSingle = pParticleObj->AddComponent<CParticleSingle>("ParticleSingle");

	//	pParticleSingle->SetSize(0.f, 0.f);
	//	pEffect->SetParticleSingle(pParticleSingle);

	//	SAFE_RELEASE(pParticleSingle);

	//	CRenderer*	pParticleRenderer = pParticleObj->FindComponentFromType<CRenderer>(CT_RENDERER);

	//	pEffect->Init_Other();
	//	SAFE_RELEASE(pEffect);
	//	/*pMaterial = pParticleRenderer->GetMaterial();

	//	pMaterial->SetDiffuseTex()

	//	SAFE_RELEASE(pMaterial);*/

	//	pParticleRenderer->CreateCBuffer("Animation2D", 10, sizeof(ANIMATION2DBUFFER),
	//		SCT_VERTEX | SCT_PIXEL);

	//	CAnimation2D*	pParticleAnimation = pParticleObj->AddComponent<CAnimation2D>("ParticleAnimation");

	//	pParticleAnimation->SetRenderer2DEnable(false);

	//	vector<wstring>	vecExplosion;

	//	for (int i = 1; i <= 1; ++i)
	//	{
	//		wchar_t	strPath[MAX_PATH] = {};
	//		wsprintf(strPath, L"Hit/Hit%d.png", i);

	//		vecExplosion.push_back(strPath);
	//	}

	//	pParticleAnimation->CreateClip("Explosion", A2D_FRAME, A2DO_LOOP,
	//		1, 1, 1, 1, 0, 0.2f, 0, 0.f, "Explosion",
	//		&vecExplosion);

	//	CNavigation3D* pParticleNavigation = pParticleObj->AddComponent<CNavigation3D>("Navigation");

	//	SAFE_RELEASE(pParticleNavigation);
	//	SAFE_RELEASE(pParticleAnimation);

	//	pParticleRenderer->SetRenderState(ALPHA_BLEND);

	//	SAFE_RELEASE(pParticleRenderer);

	//	SAFE_RELEASE(pParticleObj);
	//}
#pragma endregion

	SAFE_RELEASE(pMaterial);

	SAFE_RELEASE(pRenderer);

	SAFE_RELEASE(pLandScapeObj);

	SAFE_RELEASE(pLayer);

	return true;
}

int CMainScene::Update(float fTime)
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		/*CScene*	pScene = GET_SINGLE(CSceneManager)->CreateNextScene("TownScene");

		CTownScene*	pStartScene = pScene->CreateSceneScript<CTownScene>("TownScene");

		SAFE_RELEASE(pStartScene);

		SAFE_RELEASE(pScene);*/
	}

	return 0;
}

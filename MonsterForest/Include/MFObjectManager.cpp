#include "MFObjectManager.h"
#include "Core/PathManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/ParticleSingle.h"
#include <random>
MFObjectManager::MFObjectManager()
{
}

MFObjectManager::MFObjectManager(const MFObjectManager &)
{
}

MFObjectManager::MFObjectManager(MFObjectManager &&)
{
}

void MFObjectManager::operator=(const MFObjectManager &)
{
}

void MFObjectManager::operator=(MFObjectManager &&)
{
}

bool MFObjectManager::initialize(const wstring& scene_name)
{
	protoTypeSetting();
	staticObjectSetting(scene_name);

	return true;
}

void MFObjectManager::protoTypeSetting()
{
	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
#pragma region PlayerPrototype
	{
		CLayer*	pLayer = pScene->GetLayer("Default");
		CGameObject*	pPlayerObj = CGameObject::CreatePrototypeDontDestroy("PlayerCharacter", pScene);
		CTransform*	pTr = pPlayerObj->GetTransform();

		pTr->SetWorldPos(250.f, 0.f, 250.f);
		pTr->SetWorldScale(1.5f, 1.5f, 1.5f);
		pTr->SetWorldRot(0.f, 0.0f, 0.f);

		CRenderer*	pRenderer = pPlayerObj->AddComponent<CRenderer>("PlayerRenderer");
		pRenderer->SetMesh("Player", L"99.Dynamic_Mesh\\00.Player\\Tanker\\Tanker.msh");

		string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);
		string transformPath = meshBasePath + "99.Dynamic_Mesh\\00.Player\\Tanker\\Tanker.dat";
		FILE* pFile_Player = nullptr;

		fopen_s(&pFile_Player, transformPath.c_str(), "rb");

		if (!pFile_Player)
			return;

		pTr->Load_Local(pFile_Player);

		fclose(pFile_Player);
	}
#pragma endregion

#pragma region GOLEM_MONSTER_PROTOTYPE
	{
		CLayer*	pLayer = pScene->GetLayer("Default");
		CGameObject*	pGolemObj = CGameObject::CreatePrototypeDontDestroy("GolemObject", pScene);
		CTransform*	pTr = pGolemObj->GetTransform();

		pTr->SetWorldPos(250.f, 0.f, 250.f);
		pTr->SetWorldScale(2.f, 2.f, 2.f);
		pTr->SetWorldRot(0.f, 0.0f, 0.f);

		CRenderer*	pRenderer = pGolemObj->AddComponent<CRenderer>("PlayerRenderer");

		pRenderer->SetMesh("Golem", L"99.Dynamic_Mesh\\02.Monster\\Golem\\Golem.msh");

		string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);

		string transformPath = meshBasePath + "99.Dynamic_Mesh\\02.Monster\\Golem\\Golem.dat";

		FILE* pFile_Player = nullptr;

		fopen_s(&pFile_Player, transformPath.c_str(), "rb");

		if (!pFile_Player)
			return;

		pTr->Load_Local(pFile_Player);
		fclose(pFile_Player);
		SAFE_RELEASE(pTr);
		SAFE_RELEASE(pGolemObj);
		SAFE_RELEASE(pLayer);
	}
#pragma endregion

#pragma region MINO_MONSTER_PROTOTYPE
	{
		CLayer*	pLayer = pScene->GetLayer("Default");
		CGameObject*	pMinoObj = CGameObject::CreatePrototypeDontDestroy("MinoObject", pScene);
		CTransform*	pTr = pMinoObj->GetTransform();
		pTr->SetWorldScale(2.f, 2.f, 2.f);
		CRenderer*	pRenderer = pMinoObj->AddComponent<CRenderer>("PlayerRenderer");

		pRenderer->SetMesh("mino", L"99.Dynamic_Mesh\\02.Monster\\Mino\\Mino.msh");

		string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);

		string transformPath = meshBasePath + "99.Dynamic_Mesh\\02.Monster\\Mino\\Mino.dat";

		FILE* pFile_Player = nullptr;

		fopen_s(&pFile_Player, transformPath.c_str(), "rb");

		if (!pFile_Player)
			return;

		pTr->Load_Local(pFile_Player);

		fclose(pFile_Player);
		SAFE_RELEASE(pTr);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pMinoObj);
		SAFE_RELEASE(pLayer);
	}
#pragma endregion

#pragma region GREENLIZARD_MONSTER_PROTOTYPE
	{
		CLayer*	pLayer = pScene->GetLayer("Default");
		CGameObject*	pGreenLizardObj = CGameObject::CreatePrototypeDontDestroy("GreenLizard", pScene);
		CTransform*	pTr = pGreenLizardObj->GetTransform();

		CRenderer*	pRenderer = pGreenLizardObj->AddComponent<CRenderer>("PlayerRenderer");

		pRenderer->SetMesh("greenlizard", L"99.Dynamic_Mesh\\02.Monster\\GreenLizard\\GreenLizard.msh");

		string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);

		string transformPath = meshBasePath + "99.Dynamic_Mesh\\02.Monster\\GreenLizard\\GreenLizard.dat";

		FILE* pFile_Player = nullptr;

		fopen_s(&pFile_Player, transformPath.c_str(), "rb");

		if (!pFile_Player)
			return;

		pTr->Load_Local(pFile_Player);

		fclose(pFile_Player);
		SAFE_RELEASE(pTr);

		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pGreenLizardObj);
		SAFE_RELEASE(pLayer);
	}
#pragma endregion

#pragma region BlueLizard
	{
		CLayer*	pLayer = pScene->GetLayer("Default");
		CGameObject*	pBlueLizardObj = CGameObject::CreatePrototypeDontDestroy("BlueLizard", pScene);
		CTransform*	pTr = pBlueLizardObj->GetTransform();

		CRenderer*	pRenderer = pBlueLizardObj->AddComponent<CRenderer>("PlayerRenderer");

		pRenderer->SetMesh("bluelizard", L"99.Dynamic_Mesh\\02.Monster\\BlueLizard\\BlueLizard.msh");

		string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);

		string transformPath = meshBasePath + "99.Dynamic_Mesh\\02.Monster\\BlueLizard\\BlueLizard.dat";

		FILE* pFile_Player = nullptr;

		fopen_s(&pFile_Player, transformPath.c_str(), "rb");

		if (!pFile_Player)
			return;

		pTr->Load_Local(pFile_Player);

		fclose(pFile_Player);
		SAFE_RELEASE(pTr);

		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pBlueLizardObj);
		SAFE_RELEASE(pLayer);
	}
#pragma endregion

#pragma region Armored_GreenLizard
	{
		CLayer*	pLayer = pScene->GetLayer("Default");
		CGameObject*	pArmoredGreenLizard = CGameObject::CreatePrototypeDontDestroy("Armored_GreenLizard", pScene);
		CTransform*	pTr = pArmoredGreenLizard->GetTransform();

		CRenderer*	pRenderer = pArmoredGreenLizard->AddComponent<CRenderer>("PlayerRenderer");

		pRenderer->SetMesh("Armored_GreenLizard", L"99.Dynamic_Mesh\\02.Monster\\Armored_GreenLizard\\Armored_GreenLizard.msh");

		string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);

		string transformPath = meshBasePath + "99.Dynamic_Mesh\\02.Monster\\Armored_GreenLizard\\Armored_GreenLizard.dat";

		FILE* pFile_Player = nullptr;

		fopen_s(&pFile_Player, transformPath.c_str(), "rb");

		if (!pFile_Player)
			return;

		pTr->Load_Local(pFile_Player);

		fclose(pFile_Player);
		SAFE_RELEASE(pTr);

		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pArmoredGreenLizard);
		SAFE_RELEASE(pLayer);
	}
#pragma endregion

#pragma region Armored_BlueLizard
	{
		CLayer*	pLayer = pScene->GetLayer("Default");
		CGameObject*	pArmored_BlueLizardObj = CGameObject::CreatePrototypeDontDestroy("Armored_BlueLizard", pScene);
		CTransform*	pTr = pArmored_BlueLizardObj->GetTransform();

		CRenderer*	pRenderer = pArmored_BlueLizardObj->AddComponent<CRenderer>("PlayerRenderer");

		pRenderer->SetMesh("Armored_BlueLizard", L"99.Dynamic_Mesh\\02.Monster\\Armored_BlueLizard\\Armored_BlueLizard.msh");

		string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);

		string transformPath = meshBasePath + "99.Dynamic_Mesh\\02.Monster\\Armored_BlueLizard\\Armored_BlueLizard.dat";

		FILE* pFile_Player = nullptr;

		fopen_s(&pFile_Player, transformPath.c_str(), "rb");

		if (!pFile_Player)
			return;

		pTr->Load_Local(pFile_Player);

		fclose(pFile_Player);
		SAFE_RELEASE(pTr);

		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pArmored_BlueLizardObj);
		SAFE_RELEASE(pLayer);
	}
#pragma endregion

#pragma region DEMONLORD
	{
		CLayer*	pLayer = pScene->GetLayer("Default");
		CGameObject*	pSeuteompiObj = CGameObject::CreatePrototypeDontDestroy("DemonLordObjeect", pScene);
		CTransform*	pTr = pSeuteompiObj->GetTransform();

		CRenderer*	pRenderer = pSeuteompiObj->AddComponent<CRenderer>("DemonRnederer");

		pRenderer->SetMesh("DemonLord", L"99.Dynamic_Mesh\\02.Monster\\DemonLord\\DemonLord.msh");

		string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);

		string transformPath = meshBasePath + "99.Dynamic_Mesh\\02.Monster\\DemonLord\\DemonLord.dat";

		FILE* pFile_Player = nullptr;

		fopen_s(&pFile_Player, transformPath.c_str(), "rb");

		if (!pFile_Player)
			return;

		pTr->Load_Local(pFile_Player);

		fclose(pFile_Player);
		SAFE_RELEASE(pTr);

		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pSeuteompiObj);
		SAFE_RELEASE(pLayer);
	}
#pragma endregion


#pragma region VILLIGER1_PROTOTYPE
	{
		CLayer*	pLayer = pScene->GetLayer("Default");
		CGameObject*	pVillager1Object = CGameObject::CreatePrototypeDontDestroy("Villager1", pScene);
		CTransform*	pTr = pVillager1Object->GetTransform();
		pTr->SetWorldScale(2.f, 2.f, 2.f);
		CRenderer*	pRenderer = pVillager1Object->AddComponent<CRenderer>("PlayerRenderer");

		pRenderer->SetMesh("Villager1", L"99.Dynamic_Mesh\\01.NPC\\Villager1\\Villager1.msh");

		string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);

		string transformPath = meshBasePath + "99.Dynamic_Mesh\\01.NPC\\Villager1\\Villager1.dat";

		FILE* pFile_Player = nullptr;

		fopen_s(&pFile_Player, transformPath.c_str(), "rb");

		if (!pFile_Player)
			return;

		pTr->Load_Local(pFile_Player);

		fclose(pFile_Player);
		SAFE_RELEASE(pTr);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pVillager1Object);
		SAFE_RELEASE(pLayer);
	}
#pragma endregion

	SAFE_RELEASE(pScene);
}

void MFObjectManager::staticObjectSetting(const wstring& scene_name)
{
	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer* pLayer = pScene->GetLayer("Default");
	CGameObject::LoadEnvObjects(scene_name, pLayer);
	SAFE_RELEASE(pScene);
	SAFE_RELEASE(pLayer);
}

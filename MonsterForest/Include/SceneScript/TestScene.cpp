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
#include "Core/NetworkManager.h"
#include "Core.h"
#include "Device.h"
#include "Component/Renderer2D.h"
#include "Component/UIButton.h"
#include "Component/ColliderRect.h"
#include "Core/PathManager.h"
#include "Core/QuadTreeManager.h"
#include "Core/NaviManager.h"
#include "Component/Cell.h"


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

		pLandScape->Load_Terrain("Test_Scene_Navi");

		// QuadManager에 정보를 넘김
		list<QUADTREENODE*>* nodes = pLandScape->GetAllNodes();

		for (const auto& iter : *nodes)
		{
			GET_SINGLE(CQuadTreeManager)->AddQuadTreeInfo(iter->strNodeName,
				iter->iSizeX,
				iter->iSizeZ,
				iter->vMin,
				iter->vMax,
				iter->pGameObject);
		}

		SAFE_RELEASE(pLandScape);
		SAFE_RELEASE(pLandScapeObj);
#pragma endregion

#pragma region Navigation
		GET_SINGLE(CNaviManager)->CreateNaviMesh("Navi_Test_Scene");
		GET_SINGLE(CNaviManager)->SetRenderCheck(true);
#pragma endregion

#pragma region Player
		CGameObject*	pPlayerObj = CGameObject::CreateObject("PlayerCharacter", pLayer);
		CPlayer*		pPlayer = pPlayerObj->AddComponent<CPlayer>("Player");
		SAFE_RELEASE(pPlayer);
		CTransform*	pTr = pPlayerObj->GetTransform();

		Vector3 vFirstNodeCenterPos = (*GET_SINGLE(CNaviManager)->GetNaviCells())[0]->Get_CenterPos();

		pTr->SetWorldPos(vFirstNodeCenterPos);
		pTr->SetWorldScale(1.f, 1.f, 1.f);
		pTr->SetWorldRot(0.f, 0.0f, 0.f);

		CRenderer*	pRenderer = pPlayerObj->AddComponent<CRenderer>("PlayerRenderer");

		pRenderer->SetMesh("Player", L"99.Dynamic_Mesh\\00.Player\\Player.msh");
		pRenderer->SetForwardShader();

		pRenderer->CreateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_PIXEL);
		SHARECBUFFER tShareBuffer = {};

		tShareBuffer.vColor = Vector4{ 0.f, 0.f, 0.f, 1.f };
		tShareBuffer.fAlphaFadeIn = 0.f;
		tShareBuffer.vCameraPos = Vector3{ 0.f, 0.f, 0.f };

		pRenderer->UpdateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_PIXEL, &tShareBuffer);

		string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);

		string animPath = meshBasePath + "99.Dynamic_Mesh\\00.Player\\Player.anm";

		/*CAnimation* pAnimation = pPlayerObj->AddComponent<CAnimation>("PlayerAnimation");
		pAnimation->LoadFromFullPath(animPath.c_str());*/

		string transformPath = meshBasePath + "99.Dynamic_Mesh\\00.Player\\Player.dat";

		FILE* pFile_Player = nullptr;

		fopen_s(&pFile_Player, transformPath.c_str(), "rb");

		if (!pFile_Player)
			return false;

		pTr->Load_Local(pFile_Player);

		fclose(pFile_Player);

		CGameObject* pCameraObj = pScene->GetMainCameraObj();
		CThirdCamera* pThirdCam = pCameraObj->AddComponent<CThirdCamera>("ThirdCamera");
		CArm* pArm = pCameraObj->AddComponent<CArm>("Arm");

		pArm->SetTarget(pPlayerObj);
		pArm->SetLookAtDist(Vector3(0.f, 1.f, 0.f));

		//SAFE_RELEASE(pAnimation);
		SAFE_RELEASE(pArm);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pTr);
		SAFE_RELEASE(pPlayerObj);
#pragma endregion

#pragma region Sword
		//// 따로 Class 필요
		//FILE* pFile = nullptr;

		//string swordPath = meshBasePath + "99.Dynamic_Mesh\\00.Player\\Sword.dat";

		//fopen_s(&pFile, swordPath.c_str(), "rb");

		//if (!pFile)
		//	return false;

		//// 메쉬 파일 경로, 메쉬 이름
		//wstring wstrMeshPath = GET_SINGLE(CPathManager)->FindPath(MESH_PATH);

		//wchar_t wstrArmPath[MAX_PATH] = {};
		//size_t pathSize;
		//fread(&pathSize, sizeof(size_t), 1, pFile);
		//fread(&wstrArmPath, sizeof(wchar_t), pathSize, pFile);
		//wchar_t wstrArmName[MAX_PATH] = {};
		//size_t nameSize;
		//fread(&nameSize, sizeof(size_t), 1, pFile);
		//fread(&wstrArmName, sizeof(wchar_t), nameSize, pFile);

		//CGameObject *pSwordObj = CGameObject::CreatePrototypeDontDestroy("Sword",
		//	pScene);

		//pRenderer = pSwordObj->AddComponent<CRenderer>("SwordRenderer");

		//char strArmName[MAX_PATH] = {};
		//sprintf(strArmName, "%ws", wstrArmName);

		//pRenderer->SetMeshFromFullPath(strArmName, (wstrMeshPath + wstrArmPath).c_str());

		//// 뼈정보
		//char strBoneName[MAX_PATH] = {};
		//size_t boneSize;
		//fread(&boneSize, sizeof(size_t), 1, pFile);
		//fread(&strBoneName, sizeof(char), boneSize, pFile);

		//pPlayerObj = CGameObject::FindObject("PlayerCharacter");
		//CAnimation* pAnimation = pPlayerObj->FindComponentFromType<CAnimation>(CT_ANIMATION);
		//PBONE pBone = pAnimation->FindBone(strBoneName);

		//pAnimation->SetSocketTarget(pBone->strName, "Sword", pSwordObj);

		///*m_boneNameAttachTo = pBone->strName;
		//m_pBoneMatrix = pBone->matBone;*/

		//pTr = pSwordObj->GetTransform();
		//pTr->Load_Local(pFile);

		//Vector3 vecCurrentRot = pTr->GetLocalRot();

		//SAFE_RELEASE(pTr);
		//SAFE_RELEASE(pAnimation);
		//SAFE_RELEASE(pPlayerObj);
		//SAFE_RELEASE(pRenderer);
		//SAFE_RELEASE(pSwordObj);

		//fclose(pFile);
#pragma endregion

		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
	}



	return true;
}

int CTestScene::Update(float fTime)
{
	return 0;
}

void CTestScene::Input(float fTime)
{
}

int CTestScene::LateUpdate(float fTime)
{
	return 0;
}

void CTestScene::Render(float fTime)
{
}

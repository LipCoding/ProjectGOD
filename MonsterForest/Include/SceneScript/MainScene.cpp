#include "MainScene.h"
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
#include "Component/ColliderRect.h"
#include "Core/PathManager.h"
#include "Core/QuadTreeManager.h"
#include "Core/SoundManager.h"
#include "../Client.h"
#include "../UserInterfaceManager.h"
#include "../Chatting.h"
#include "../Item.h"
#include "../Inventory.h"
#include "../ObjectScript/Mino.h"
#include "../ObjectScript/Seuteompi.h"
#include "Component/Picking.h"
#include "Component/ParticleMultiple.h"
#include "Core/QuadTreeManager.h"
#include "Core/NaviManager.h"
#include "Component/Cell.h"
#include "Component/ColliderRay.h"
#include "../DropTableUI.h"
#include <random>
#include "Component/UIButton.h"
#include "../PGMessageBox.h"
#include "../TargetPlayerUI.h"
#include "Core/EffectManager.h"
#include "../ObjectScript/Sword.h"
#include "Component/ColliderAABB.h"

std::wstring strconv(const std::string& _src)
{
	USES_CONVERSION;
	return std::wstring(A2W(_src.c_str()));
};

std::string strconv(const std::wstring& _src)
{
	USES_CONVERSION;
	return std::string(W2A(_src.c_str()));
};

CMainScene::CMainScene()
{
}

CMainScene::~CMainScene()
{

}

void CMainScene::chat_callback(float fTime)
{
	//this->chat_write = true;
}

bool CMainScene::Init()
{
	/* Effect */
	//GET_SINGLE(CEffectManager)->AddEffect("Hit", "Effect\\hit_test.bin");
	//GET_SINGLE(CEffectManager)->AddEffect("Critical", "Effect\\critical_test.bin");
	//GET_SINGLE(CEffectManager)->AddEffect("Rune", "Effect\\rune_test.bin");
#pragma region Layer Setting
	{
		CLayer* pLayer = m_pScene->CreateLayer("UI+1", UI_LAYER + 1);
		SAFE_RELEASE(pLayer);
	}

	{
		CLayer* pLayer = m_pScene->CreateLayer("UI+2", UI_LAYER + 2);
		SAFE_RELEASE(pLayer);
	}

	{
		CLayer* pLayer = m_pScene->CreateLayer("UI+3", UI_LAYER + 3);
		SAFE_RELEASE(pLayer);
	}

	{
		CLayer*    pParticleLayer = m_pScene->CreateLayer("ParticleLayer", 2000);
		SAFE_RELEASE(pParticleLayer);
	}


#pragma endregion
	GET_SINGLE(UserInterfaceManager)->initialize();

#pragma region KeySetting
	GET_SINGLE(CInput)->CreateKey("Attack", 'X');
	GET_SINGLE(CInput)->CreateKey("QUICKSLOT-Q", 'Q');
	GET_SINGLE(CInput)->CreateKey("INVENTORY", 'I');
#pragma endregion

#pragma region Terrain
	{
		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer* pLayer = pScene->GetLayer("Default");

		// Load Terrain
		CGameObject* pLandScapeObj = CGameObject::CreateObject("LandScape_Stage1", pLayer);
		CLandScape* pLandScape = pLandScapeObj->AddComponent<CLandScape>("LandScape");

		pLandScape->Load_Terrain("Main_Scene_1");

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
	}
#pragma endregion

#pragma region StaticObject
	//// 경로 지정
	//wchar_t strPath[MAX_PATH] = {};
	//wcscpy_s(strPath, MAX_PATH, GET_SINGLE(CPathManager)->FindPath(DATA_PATH));
	//wcscat_s(strPath, MAX_PATH, L"Object\\Main_Scene_1.bin");

	//ifstream file;
	//file.open(strPath, ios::in);

	//if (!file.is_open())
	//	return false;

	//int iObjSize = 0;
	//file >> iObjSize;

	//for (int i = 0; i < iObjSize; i++)
	//{
	//	string objName = "ObjName_" + to_string(i);

	//	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	//	CLayer* pLayer = pScene->GetLayer("Default");
	//	CGameObject *pObj = CGameObject::CreateObject(objName, pLayer);

	//	string objTag;
	//	file >> objTag;

	//	// Mesh
	//	string meshPath, meshRestPath;
	//	meshPath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);
	//	meshRestPath = objTag;

	//	string meshDataPath;
	//	meshDataPath = meshPath + meshRestPath + ".msh";

	//	CRenderer* pRenderer = pObj->AddComponent<CRenderer>("Renderer");

	//	wstring wMeshDataPath;
	//	wMeshDataPath.assign(meshDataPath.begin(), meshDataPath.end());
	//	pRenderer->SetMeshFromFullPath(objTag, wMeshDataPath.c_str());

	//	SAFE_RELEASE(pRenderer);

	//	// Transform
	//	// Local Transform Data
	//	string localDataPath;

	//	localDataPath = meshPath + meshRestPath + ".dat";

	//	FILE* pFile = nullptr;

	//	fopen_s(&pFile, localDataPath.c_str(), "rb");

	//	if (!pFile)
	//		return false;

	//	CTransform* pTr = pObj->GetTransform();
	//	pTr->Load_Local(pFile);
	//	fclose(pFile);

	//	// World Transform Data
	//	Vector3 vScale, vRotation, vPos;
	//	file >> vScale.x >> vScale.y >> vScale.z;
	//	file >> vRotation.x >> vRotation.y >> vRotation.z;
	//	file >> vPos.x >> vPos.y >> vPos.z;

	//	pTr->SetWorldScale(vScale);
	//	pTr->SetWorldRot(vRotation);
	//	pTr->SetWorldPos(vPos);

	//	SAFE_RELEASE(pTr);
	//}
#pragma endregion

	{
		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer* pLayer = pScene->GetLayer("Default");

#pragma region PlayerPrototype
		{
			CLayer*	pLayer = m_pScene->GetLayer("Default");
			CGameObject*	pPlayerObj = CGameObject::CreatePrototypeDontDestroy("PlayerCharacter", m_pScene);
			CTransform*	pTr = pPlayerObj->GetTransform();

			pTr->SetWorldPos(250.f, 0.f, 250.f);
			pTr->SetWorldScale(1.f, 1.f, 1.f);
			pTr->SetWorldRot(0.f, 0.0f, 0.f);

			CRenderer*	pRenderer = pPlayerObj->AddComponent<CRenderer>("PlayerRenderer");
			pRenderer->SetMesh("Player", L"99.Dynamic_Mesh\\00.Player\\Tanker\\Tanker.msh");
			//pRenderer->SetForwardShader();

			string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);

			//string animPath = meshBasePath + "99.Dynamic_Mesh\\00.Player\\Player.anm";

			//CAnimation* pAnimation = pPlayerObj->AddComponent<CAnimation>("PlayerAnimation");
			//pAnimation->LoadFromFullPath(animPath.c_str());

			string transformPath = meshBasePath + "99.Dynamic_Mesh\\00.Player\\Tanker\\Tanker.dat";
			{
				CColliderSphere* pCollider = pPlayerObj->AddComponent<CColliderSphere>("collider1");
				pCollider->SetSphere(Vector3(0.f, 1.f, 0.f), 1.f);
				SAFE_RELEASE(pCollider);
			}

			{
				CColliderSphere* pCollider = pPlayerObj->AddComponent<CColliderSphere>("collider2");
				pCollider->SetSphere(Vector3(0.f, 2.f, 0.f), 1.f);
				SAFE_RELEASE(pCollider);
			}
			FILE* pFile_Player = nullptr;

			fopen_s(&pFile_Player, transformPath.c_str(), "rb");

			if (!pFile_Player)
				return false;

			pTr->Load_Local(pFile_Player);

			fclose(pFile_Player);
		}
#pragma endreigon

#pragma region GOLEM_MONSTER_PROTOTYPE
		{
			CLayer*	pLayer = m_pScene->GetLayer("Default");
			CGameObject*	pGolemObj = CGameObject::CreatePrototypeDontDestroy("GolemObject", m_pScene);
			CTransform*	pTr = pGolemObj->GetTransform();

			pTr->SetWorldPos(250.f, 0.f, 250.f);
			pTr->SetWorldScale(1.f, 1.f, 1.f);
			pTr->SetWorldRot(0.f, 0.0f, 0.f);

			{
				CColliderSphere* pCollider = pGolemObj->AddComponent<CColliderSphere>("collider1");
				pCollider->SetSphere(Vector3(0.f, 1.f, 0.f), 1.f);
				SAFE_RELEASE(pCollider);
			}
			CRenderer*	pRenderer = pGolemObj->AddComponent<CRenderer>("PlayerRenderer");

			pRenderer->SetMesh("golem", L"99.Dynamic_Mesh\\02.Monster\\Golem\\Golem.msh");

			string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);

			string transformPath = meshBasePath + "99.Dynamic_Mesh\\02.Monster\\Golem\\golem.dat";

			FILE* pFile_Player = nullptr;

			fopen_s(&pFile_Player, transformPath.c_str(), "rb");

			if (!pFile_Player)
				return false;

			pTr->Load_Local(pFile_Player);
			fclose(pFile_Player);
			SAFE_RELEASE(pTr);
			SAFE_RELEASE(pGolemObj);
			SAFE_RELEASE(pLayer);
		}
#pragma endregion

#pragma region MINO_MONSTER_PROTOTYPE
		{
			CLayer*	pLayer = m_pScene->GetLayer("Default");
			CGameObject*	pMinoObj = CGameObject::CreatePrototypeDontDestroy("MinoObject", m_pScene);
			CTransform*	pTr = pMinoObj->GetTransform();

			CRenderer*	pRenderer = pMinoObj->AddComponent<CRenderer>("PlayerRenderer");

			pRenderer->SetMesh("mino", L"99.Dynamic_Mesh\\02.Monster\\Mino\\Mino.msh");
			{
				CColliderSphere* pCollider = pMinoObj->AddComponent<CColliderSphere>("collider1");
				pCollider->SetSphere(Vector3(0.f, 1.f, 0.f), 1.f);
				SAFE_RELEASE(pCollider);
			}
			{
				CColliderSphere* pCollider = pMinoObj->AddComponent<CColliderSphere>("collider2");
				pCollider->SetSphere(Vector3(0.f, 2.f, 0.f), 1.f);
				SAFE_RELEASE(pCollider);
			}
			//{
			//	CColliderSphere* pCollider = pMinoObj->AddComponent<CColliderSphere>("collider2");
			//	pCollider->SetSphere(Vector3(0.f, 3.f, 0.f), 1.f);
			//	SAFE_RELEASE(pCollider);
			//}

			string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);

			string transformPath = meshBasePath + "99.Dynamic_Mesh\\02.Monster\\Mino\\Mino.dat";

			FILE* pFile_Player = nullptr;

			fopen_s(&pFile_Player, transformPath.c_str(), "rb");

			if (!pFile_Player)
				return false;

			pTr->Load_Local(pFile_Player);

			fclose(pFile_Player);
			SAFE_RELEASE(pTr);
			/*string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);

			string transformPath = meshBasePath + "99.Dynamic_Mesh\\02.Monster\\golem.dat";

			FILE* pFile_Player = nullptr;

			fopen_s(&pFile_Player, transformPath.c_str(), "rb");

			if (!pFile_Player)
				return false;

			pTr->Load_Local(pFile_Player);
			fclose(pFile_Player);*/
			SAFE_RELEASE(pRenderer);
			SAFE_RELEASE(pMinoObj);
			SAFE_RELEASE(pLayer);
		}
#pragma endregion

#pragma region SEUTEOMPI_MONSTER_PROTOTYPE
		{
			CLayer*	pLayer = m_pScene->GetLayer("Default");
			CGameObject*	pSeuteompiObj = CGameObject::CreatePrototypeDontDestroy("SeuteompiObject", m_pScene);
			CTransform*	pTr = pSeuteompiObj->GetTransform();

			CRenderer*	pRenderer = pSeuteompiObj->AddComponent<CRenderer>("PlayerRenderer");

			pRenderer->SetMesh("seuteompi", L"99.Dynamic_Mesh\\02.Monster\\Seuteompi\\Seuteompi.msh");

			{
				CColliderSphere* pCollider = pSeuteompiObj->AddComponent<CColliderSphere>("collider1");
				pCollider->SetSphere(Vector3(0.f, 1.f, 0.f), 1.f);
				SAFE_RELEASE(pCollider);
			}
			string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);

			string transformPath = meshBasePath + "99.Dynamic_Mesh\\02.Monster\\Seuteompi\\seuteompi.dat";

			FILE* pFile_Player = nullptr;

			fopen_s(&pFile_Player, transformPath.c_str(), "rb");

			if (!pFile_Player)
				return false;

			pTr->Load_Local(pFile_Player);

			fclose(pFile_Player);
			SAFE_RELEASE(pTr);

			SAFE_RELEASE(pRenderer);
			SAFE_RELEASE(pSeuteompiObj);
			SAFE_RELEASE(pLayer);
		}


		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
	}

	// mainserver로의 접속요청을 한다.
	{
		cs_packet_connect* pPacket = reinterpret_cast<cs_packet_connect*>(NetworkManager::getInstance()->getSendBuffer());
		pPacket->size = sizeof(cs_packet_connect);
		pPacket->type = CS_PACKET_MAINSERVER_CONNECT;

		NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_connect);
		DWORD iobyte;
		int ret = WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);
	}

	this->inputTime = high_resolution_clock::now();
	this->prevPos = Vector3::Zero;

	//{
	//	CLayer*	pLayer = m_pScene->GetLayer("UI");
	//	this->pUIChat = CGameObject::CreateObject("UIChat", pLayer);
	//
	//	CUIButton*	pButton = pUIChat->AddComponent<CUIButton>("ChatEdit");
	//	pButton->SetCallback(this, &CMainScene::chat_callback);
	//	SAFE_RELEASE(pButton);
	//
	//	CTransform*	pButtonTr = pUIChat->GetTransform();
	//
	//	//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
	//	pButtonTr->SetWorldScale(200.f, 50.f, 1.f);
	//	pButtonTr->SetWorldPos(0.f, DEVICE_RESOLUTION.iHeight - 120.f, 0.f);
	//
	//	SAFE_RELEASE(pButtonTr);
	//	CRenderer2D* pRenderer = pUIChat->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
	//	CMaterial* pMaterial = pRenderer->GetMaterial();
	//
	//	pMaterial->SetDiffuseTexInfo("Linear", "ChatEdit",
	//		0, 0, L"LoginEdit.bmp");
	//
	//	SAFE_RELEASE(pMaterial);
	//	SAFE_RELEASE(pRenderer);
	//
	//	CColliderRect* pRC = pUIChat->FindComponentFromType<CColliderRect>(CT_COLLIDER);
	//
	//	pRC->SetRect(0, 0, 200, 50);
	//
	//	SAFE_RELEASE(pRC);
	//
	//	pUIChatText = pUIChat->AddComponent<CFont>("TextUI");
	//	pUIChatText->SetFont("나눔고딕");
	//	pChatString = L"채팅 테스트";
	//	pUIChatText->SetText(pChatString);
	//	pUIChatText->SetArea(0, 0, 200, 30.f);
	//}

#pragma region UI

#pragma endregion

	//{
	//	CLayer*	pUILayer = m_pScene->GetLayer("UI+2");

	//	CGameObject*	pHPUIObj = CGameObject::CreateObject("HPUI", pUILayer);
	//	CUIButton* pUIHearthBar = pHPUIObj->AddComponent<CUIButton>("HPUI");
	//	pUIHearthBar->setUILength(125.f);
	//	pUIHearthBar->setLengthRatio(1.f);

	//	CTransform*	pButtonTr = pHPUIObj->GetTransform();
	//	//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
	//	pButtonTr->SetWorldScale(125.f, 25.f, 1.f);
	//	pButtonTr->SetWorldPos(127.f, 6.f, 0.f);

	//	SAFE_RELEASE(pButtonTr);

	//	CRenderer2D* pRenderer = pHPUIObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
	//	pRenderer->SetShader(UI_HEARTH_BAR_SHADER);
	//	CMaterial* pMaterial = pRenderer->GetMaterial();

	//	pMaterial->SetDiffuseTexInfo("Linear", "HealthPoint",
	//		0, 0, L"UserInterface/UI_HP.png");

	//	SAFE_RELEASE(pMaterial);

	//	SAFE_RELEASE(pRenderer);

	//	CColliderRect* pRC = pHPUIObj->FindComponentFromType<CColliderRect>(CT_COLLIDER);

	//	pRC->SetRect(0, 0, 125, 25);

	//	SAFE_RELEASE(pRC);

	//	CFont* HPFont = pHPUIObj->AddComponent<CFont>("HPFont");
	//	HPFont->SetFont("나눔고딕");
	//	HPFont->SetText(L"");
	//	HPFont->SetArea(25, 0, 200, 25.f);
	//	SAFE_RELEASE(HPFont);

	//	GET_SINGLE(UserInterfaceManager)->setUIHearthBar(pUIHearthBar);

	//	SAFE_RELEASE(pHPUIObj);

	//	SAFE_RELEASE(pUILayer);
	//}

	//{
	//	CLayer*	pUILayer = m_pScene->GetLayer("UI+2");

	//	CGameObject*	pHPUIObj = CGameObject::CreateObject("Enemy1_HPUI", pUILayer);
	//	CUIButton* pEnemyUIHearthBar = pHPUIObj->AddComponent<CUIButton>("HPUI");
	//	pEnemyUIHearthBar->setUILength(200.f);
	//	pEnemyUIHearthBar->setLengthRatio(1.f);

	//	CTransform*	pButtonTr = pHPUIObj->GetTransform();

	//	//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
	//	pButtonTr->SetWorldScale(125.f, 25.f, 1.f);
	//	pButtonTr->SetWorldPos(527.f, 6.f, 0.f);

	//	SAFE_RELEASE(pButtonTr);

	//	CRenderer2D* pRenderer = pHPUIObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
	//	pRenderer->SetShader(UI_HEARTH_BAR_SHADER);
	//	CMaterial* pMaterial = pRenderer->GetMaterial();

	//	pMaterial->SetDiffuseTexInfo("Linear", "HealthPoint",
	//		0, 0, L"UserInterface/UI_HP.png");

	//	SAFE_RELEASE(pMaterial);

	//	SAFE_RELEASE(pRenderer);

	//	CColliderRect* pRC = pHPUIObj->FindComponentFromType<CColliderRect>(CT_COLLIDER);

	//	pRC->SetRect(0, 0, 200, 25);

	//	SAFE_RELEASE(pRC);
	//	CFont* HPFont = pHPUIObj->AddComponent<CFont>("HPFont");
	//	HPFont->SetFont("나눔고딕");
	//	HPFont->SetText(L"");
	//	HPFont->SetArea(50, 0, 200, 25.f);
	//	SAFE_RELEASE(HPFont);

	//	GET_SINGLE(UserInterfaceManager)->setEnemyUIHearthBar(pEnemyUIHearthBar);

	//	SAFE_RELEASE(pHPUIObj);

	//	SAFE_RELEASE(pUILayer);
	//}

#pragma region sound
	//GET_SINGLE(SoundManager)->LoadSound("MainSceneBGM", true, "WoodlandFantasy.mp3");

	//GET_SINGLE(SoundManager)->Play("MainSceneBGM", SC_BGM);
#pragma endregion

#pragma region Item
	//{
	//	CLayer*	pLayer = m_pScene->GetLayer("UI+2");
	//	CGameObject* pItem = CGameObject::CreateObject("SWORD_ITEM", pLayer);

	//	CUIButton*	pItemUI = pItem->AddComponent<CUIButton>("SWORD_ITEM");
	//	SAFE_RELEASE(pItemUI);

	//	CTransform*	pItemTr = pItem->GetTransform();
	//	//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
	//	pItemTr->SetWorldScale(30.f, 30.f, 1.f);
	//	pItemTr->SetWorldPos(DEVICE_RESOLUTION.iWidth / 2.f - 100.f,
	//		DEVICE_RESOLUTION.iHeight / 2.f, 0.f);

	//	SAFE_RELEASE(pItemTr);
	//	CRenderer2D* pRenderer = pItem->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
	//	CMaterial* pMaterial = pRenderer->GetMaterial();

	//	pMaterial->SetDiffuseTexInfo("Linear", "SWORD_ICON",
	//		0, 0, L"SWORD_ICON.jpg");

	//	SAFE_RELEASE(pMaterial);
	//	SAFE_RELEASE(pRenderer);

	//	CColliderRect* pRC = pItem->FindComponentFromType<CColliderRect>(CT_COLLIDER);

	//	pRC->SetRect(0, 0, 0, 0);

	//	SAFE_RELEASE(pRC);

	//	GET_SINGLE(UserInterfaceManager)->getInventory()->addItem(pItem);
	//	SAFE_RELEASE(pItem);
	//}

	//{
	//	CLayer*	pLayer = m_pScene->GetLayer("UI+2");
	//	CGameObject* pItem = CGameObject::CreateObject("SWORD_ITEM2", pLayer);

	//	CUIButton*	pItemUI = pItem->AddComponent<CUIButton>("SWORD_ITEM");
	//	SAFE_RELEASE(pItemUI);

	//	CTransform*	pItemTr = pItem->GetTransform();
	//	pItemTr->SetWorldScale(30.f, 30.f, 1.f);
	//	pItemTr->SetWorldPos(DEVICE_RESOLUTION.iWidth / 2.f - 100.f,
	//		DEVICE_RESOLUTION.iHeight / 2.f, 0.f);

	//	SAFE_RELEASE(pItemTr);
	//	CRenderer2D* pRenderer = pItem->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
	//	CMaterial* pMaterial = pRenderer->GetMaterial();

	//	pMaterial->SetDiffuseTexInfo("Linear", "SWORD_ICON",
	//		0, 0, L"SWORD_ICON.jpg");

	//	SAFE_RELEASE(pMaterial);
	//	SAFE_RELEASE(pRenderer);

	//	CColliderRect* pRC = pItem->FindComponentFromType<CColliderRect>(CT_COLLIDER);

	//	pRC->SetRect(0, 0, 0, 0);

	//	SAFE_RELEASE(pRC);

	//	GET_SINGLE(UserInterfaceManager)->getInventory()->addItem(pItem);
	//	SAFE_RELEASE(pItem);
	//}

	//{
	//	CLayer*	pLayer = m_pScene->GetLayer("UI+2");
	//	CGameObject* pItem = CGameObject::CreateObject("SWORD_ITEM3", pLayer);

	//	CUIButton*	pItemUI = pItem->AddComponent<CUIButton>("SWORD_ITEM");
	//	SAFE_RELEASE(pItemUI);

	//	CTransform*	pItemTr = pItem->GetTransform();
	//	//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
	//	pItemTr->SetWorldScale(30.f, 30.f, 1.f);
	//	pItemTr->SetWorldPos(DEVICE_RESOLUTION.iWidth / 2.f - 100.f,
	//		DEVICE_RESOLUTION.iHeight / 2.f, 0.f);

	//	SAFE_RELEASE(pItemTr);
	//	CRenderer2D* pRenderer = pItem->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
	//	CMaterial* pMaterial = pRenderer->GetMaterial();

	//	pMaterial->SetDiffuseTexInfo("Linear", "SWORD_ICON",
	//		0, 0, L"SWORD_ICON.jpg");

	//	SAFE_RELEASE(pMaterial);
	//	SAFE_RELEASE(pRenderer);

	//	CColliderRect* pRC = pItem->FindComponentFromType<CColliderRect>(CT_COLLIDER);

	//	pRC->SetRect(0, 0, 0, 0);

	//	SAFE_RELEASE(pRC);

	//	GET_SINGLE(UserInterfaceManager)->getInventory()->addItem(pItem);
	//	SAFE_RELEASE(pItem);
	//}
#pragma endregion

//#pragma region Particle
//	CLayer*    pParticleLayer = m_pScene->GetLayer("ParticleLayer");
//
//	for (int i = 0; i < 5; ++i)
//	{
//		CGameObject*    pParticleObj = CGameObject::CreateObject(
//			"ParticleObj", pParticleLayer);
//
//		CTransform*    pParticleTr = pParticleObj->GetTransform();
//
//		pParticleTr->SetWorldPos(300.f, 2.f, 300.f * i);
//
//		SAFE_RELEASE(pParticleTr);
//
//		CParticleSingle*    pParticleSingle = pParticleObj->AddComponent<CParticleSingle>("ParticleSingle");
//
//		pParticleSingle->SetSize(10.f, 10.f);
//
//		SAFE_RELEASE(pParticleSingle);
//
//		CRenderer*    pParticleRenderer = pParticleObj->FindComponentFromType<CRenderer>(CT_RENDERER);
//
//		pParticleRenderer->CreateCBuffer("Animation2D", 10, sizeof(ANIMATION2DBUFFER),
//			SCT_VERTEX | SCT_PIXEL);
//
//		CAnimation2D*    pParticleAnimation = pParticleObj->AddComponent<CAnimation2D>("ParticleAnimation");
//
//		pParticleAnimation->SetRenderer2DEnable(false);
//
//		vector<wstring>    vecExplosion;
//
//		for (int i = 1; i <= 89; ++i)
//		{
//			wchar_t    strPath[MAX_PATH] = {};
//			wsprintf(strPath, L"Explosion/Explosion%d.png", i);
//
//			vecExplosion.push_back(strPath);
//		}
//
//		pParticleAnimation->CreateClip("Explosion", A2D_FRAME, A2DO_LOOP,
//			89, 1, 89, 1, 0, 0.5f, 0, 0.f, "Explosion", &vecExplosion);
//
//		SAFE_RELEASE(pParticleAnimation);
//
//		pParticleRenderer->SetRenderState(ALPHA_BLEND);
//
//		SAFE_RELEASE(pParticleRenderer);
//
//		SAFE_RELEASE(pParticleObj);
//	}
//	SAFE_RELEASE(pParticleLayer);
//#pragma endregion

//#pragma region billboard
//	{
//		default_random_engine dre;
//		uniform_int_distribution<> ui;
//
//		CLayer*    pParticleLayer = m_pScene->GetLayer("ParticleLayer");
//
//		for (int i = 0; i < 10; ++i)
//		{
//			for (int j = 0; j < 10; ++j)
//			{
//				string number_tag = to_string(i);
//				string object_tag = "ParticleObj" + number_tag;
//				CGameObject*    pParticleObj = CGameObject::CreateObject(object_tag, pParticleLayer);
//
//				CTransform*    pParticleTr = pParticleObj->GetTransform();
//				pParticleTr->SetWorldPos(300.f + (0.2 * i), 2.f, 300.f + (0.2*j));
//				SAFE_RELEASE(pParticleTr);
//
//				CParticleSingle*    pParticleSingle = pParticleObj->AddComponent<CParticleSingle>("ParticleSingle");
//				pParticleSingle->SetSize(10.f, 10.f);
//				SAFE_RELEASE(pParticleSingle);
//
//				CRenderer*    pParticleRenderer = pParticleObj->FindComponentFromType<CRenderer>(CT_RENDERER);
//				pParticleRenderer->CreateCBuffer("Animation2D", 10, sizeof(ANIMATION2DBUFFER),
//					SCT_VERTEX | SCT_PIXEL);
//				pParticleRenderer->SetRenderState(ALPHA_BLEND);
//				CAnimation2D*    pParticleAnimation = pParticleObj->AddComponent<CAnimation2D>("ParticleAnimation");
//				pParticleAnimation->SetRenderer2DEnable(false);
//
//				vector<wstring>    vecExplosion;
//				for (int i = 1; i <= 1; ++i)
//				{
//					wchar_t    strPath[MAX_PATH] = {};
//					wsprintf(strPath, L"Billboard/grass.tga", i);
//
//					vecExplosion.push_back(strPath);
//				}
//
//				pParticleAnimation->CreateClip("Explosion", A2D_FRAME, A2DO_LOOP,
//					1, 1, 1, 1, 0, 0.5f, 0, 0.f, "Explosion", &vecExplosion);
//
//				SAFE_RELEASE(pParticleAnimation);
//				SAFE_RELEASE(pParticleRenderer);
//				SAFE_RELEASE(pParticleObj);
//			}
//		}
//		SAFE_RELEASE(pParticleLayer);
//	}
//#pragma endregion

	GET_SINGLE(CNaviManager)->CreateNaviMesh("Main_Scene_1");
	GET_SINGLE(CNaviManager)->SetRenderCheck(true);

	return true;
}


void CMainScene::Input(float fTime)
{
	if (KEYDOWN("QUICKSLOT-Q"))
	{
		{
#pragma region AlphaBlend
			{
				CLayer*	pLayer = m_pScene->GetLayer("UI+1");
				CGameObject* pFadeInOut = CGameObject::CreateObject("FadeInOut", pLayer);
				CTransform* pFadeInOutTr = pFadeInOut->GetTransform();
				CUIButton* pButton = pFadeInOut->AddComponent<CUIButton>("d");
				pButton->enableAlpha(true);
				SAFE_RELEASE(pButton);
				pFadeInOutTr->SetWorldScale(1280.f, 960.f, 1.f);
				pFadeInOutTr->SetWorldPos(0.f, 0.f, 0.f);
				CRenderer2D* pRenderer = pFadeInOut->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
				CMaterial* pMaterial = pRenderer->GetMaterial();

				pMaterial->SetDiffuseTexInfo("Linear", "Fadeinout",
					0, 0, L"Fadeinout.png");
				SAFE_RELEASE(pFadeInOutTr);
				SAFE_RELEASE(pMaterial);
				SAFE_RELEASE(pRenderer);
				SAFE_RELEASE(pFadeInOut);
				SAFE_RELEASE(pLayer);
			}
#pragma endregion
		}
		//string appendTag = to_string(NetworkManager::getInstance()->getMyClientID());
		//string objectTag = "Player" + appendTag;
		//CGameObject* pGameObject = CGameObject::FindObject(objectTag);
		//CPlayer* pPlayerComponent = pGameObject->FindComponentFromTag<CPlayer>("Player");
		//if (pPlayerComponent->clickedID != -1)
		//{
		//	cs_packet_attack_player* pPacket = reinterpret_cast<cs_packet_attack_player*>(NetworkManager::getInstance()->getSendBuffer());
		//	pPacket->size = sizeof(cs_packet_attack_player);
		//	NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_attack_player);
		//	DWORD iobyte;

		//	pPacket->id = NetworkManager::getInstance()->getMyClientID();
		//	pPacket->targetid = pPlayerComponent->clickedID;
		//	pPacket->type = CS_PACKET_ATTACK_SWORD_SKILL1;
		//	int ret = WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);

		//	if (ret)
		//	{

		//	}
		//}
	}

	if (KEYDOWN("Enter"))
	{
		Chatting* pChatting = GET_SINGLE(UserInterfaceManager)->getChatting();

		if (false == pChatting->isChatWrite())
		{
			pChatting->enableChatWrite(true);
		}
		else
		{
			cs_packet_chat* packet = reinterpret_cast<cs_packet_chat*>(NetworkManager::getInstance()->getSendBuffer());
			packet->type = CS_PACKET_CHAT;



			wstring chat_message = L"";
			const auto& chat_edit_cont = pChatting->getChatCont();

			for (auto input : chat_edit_cont)
			{
				wchar_t temp[64];
				wchar_t temp2 = input;
				wstring appendString = L"";
				appendString = appendString + temp2;
				chat_message = chat_message + appendString;
			}

			//while (chat_message.length() < 20)
			//{
			//	chat_message += L" ";
			//}
			packet->id = NetworkManager::getInstance()->getMyClientID();
			packet->size = sizeof(cs_packet_chat);
			wcscpy_s(packet->message, chat_message.c_str());
			packet->message[MAX_STR_SIZE] = 0;
			NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_chat);
			DWORD iobyte;
			int ret = WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);
			pChatting->getChatCont().clear();
			pChatting->enableChatWrite(false);
		}
	}


	if (KEYPUSH("MoveRight"))
	{
		cs_packet_rotate* pPacket = reinterpret_cast<cs_packet_rotate*>(NetworkManager::getInstance()->getSendBuffer());

		pPacket->size = sizeof(cs_packet_rotate);
		NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_rotate);
		DWORD iobyte;

		pPacket->type = CS_PACKET_ROTATE_Y;

		NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_rotate);
		int ret = WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);

	}


	if (KEYPUSH("MoveLeft"))
	{
		cs_packet_rotate* pPacket = reinterpret_cast<cs_packet_rotate*>(NetworkManager::getInstance()->getSendBuffer());

		pPacket->size = sizeof(cs_packet_rotate);
		NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_rotate);
		DWORD iobyte;

		pPacket->type = CS_PACKET_ROTATE_INV_Y;

		NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_rotate);
		int ret = WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);

	}

	if (KEYPUSH("MoveFront"))
	{

		CTransform* pTransform = pPlayer->GetTransform();
		cs_packet_up* pPacket = reinterpret_cast<cs_packet_up*>(NetworkManager::getInstance()->getSendBuffer());

		pPacket->size = sizeof(cs_packet_up);
		NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_up);
		DWORD iobyte;

		Vector3 Axis = pTransform->GetWorldAxis(AXIS_Z);
		pPacket->type = CS_PACKET_MOVE_FRONT;
		pPacket->dir_x = Axis.x;
		pPacket->dir_y = Axis.y;
		pPacket->dir_z = Axis.z;

		NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_up);
		int ret = WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);

		if (ret)
		{
			// 에러처리.
			char a = 0;
		}
		//CAnimation* pAnimation = pPlayer->FindComponentFromType<CAnimation>(CT_ANIMATION);
		//pAnimation->ChangeClip("MOVE");
		//SAFE_RELEASE(pAnimation);
	}

	if (KEYUP("MoveFront"))
	{
		CAnimation* pAnimation = pPlayer->FindComponentFromType<CAnimation>(CT_ANIMATION);
		pAnimation->ReturnDefaultClip();
	}

	if (KEYPUSH("MoveBack"))
	{

		CTransform* pTransform = pPlayer->GetTransform();

		cs_packet_up* pPacket = reinterpret_cast<cs_packet_up*>(NetworkManager::getInstance()->getSendBuffer());

		pPacket->size = sizeof(cs_packet_up);
		NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_up);
		DWORD iobyte;

		Vector3 Axis = pTransform->GetWorldAxis(AXIS_Z);
		pPacket->type = CS_PACKET_MOVE_BACK;
		pPacket->dir_x = Axis.x;
		pPacket->dir_y = Axis.y;
		pPacket->dir_z = Axis.z;


		NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_up);
		int ret = WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);

		if (ret)
		{
			char a = 0;
		}
	}

	if (KEYDOWN("MouseLButton"))
	{
		CGameObject *pMouseObj = GET_SINGLE(CInput)->GetMouseObj();
		CColliderRay *pRay = pMouseObj->FindComponentFromTag<CColliderRay>("MouseRay");
		bool clickedEnemy = false;
		for (const auto& object : CGameObject::getObjectList())
		{
			CColliderSphere *pColl = object->FindComponentFromType<CColliderSphere>(CT_COLLIDER);

			if (pRay->CheckCollList(pColl))
			{
				clickedEnemy = true;
			}
			SAFE_RELEASE(pColl);
		}

		if (clickedEnemy)
			GET_SINGLE(UserInterfaceManager)->getEnemyStatus()->enableRender(true);
		else
			GET_SINGLE(UserInterfaceManager)->getEnemyStatus()->enableRender(false);

		SAFE_RELEASE(pRay);
		SAFE_RELEASE(pMouseObj);
	}

	if (KEYDOWN("MouseRButton"))
	{
		CGameObject *pMouseObj = GET_SINGLE(CInput)->GetMouseObj();
		CColliderRay *pRay = pMouseObj->FindComponentFromTag<CColliderRay>("MouseRay");
		bool clickedEnemy = false;
		for (const auto& object : CGameObject::getObjectList())
		{
			CColliderSphere *pColl = object->FindComponentFromType<CColliderSphere>(CT_COLLIDER);

			if (pRay->CheckCollList(pColl))
			{
				clickedEnemy = true;
			}
			SAFE_RELEASE(pColl);
		}
		if (clickedEnemy == false)
		{
			GET_SINGLE(UserInterfaceManager)->getDropTableUI()->enableRender(false);
			GET_SINGLE(UserInterfaceManager)->getTargetPlayerUI()->enableRender(false);
		}

		SAFE_RELEASE(pRay);
		SAFE_RELEASE(pMouseObj);
	}

	if (KEYDOWN("ESC"))
	{
		GET_SINGLE(UserInterfaceManager)->getDropTableUI()->enableRender(false);
		GET_SINGLE(UserInterfaceManager)->getTargetPlayerUI()->enableRender(false);
		GET_SINGLE(UserInterfaceManager)->getInventory()->enableRender(false);
	}

	if (KEYDOWN("Attack"))
	{
		char str[128];
		int id = NetworkManager::getInstance()->getMyClientID();
		string appendTag = _itoa(id, str, 10);
		string objectTag = "Player" + appendTag;
		CGameObject* pGameObject = CGameObject::FindObject(objectTag);
		CPlayer* pPlayerComponent = pGameObject->FindComponentFromTag<CPlayer>("Player");
		if (pPlayerComponent->clickedID != -1)
		{
			cs_packet_attack_player* pPacket = reinterpret_cast<cs_packet_attack_player*>(NetworkManager::getInstance()->getSendBuffer());
			pPacket->size = sizeof(cs_packet_attack_player);
			NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_attack_player);
			DWORD iobyte;

			pPacket->id = NetworkManager::getInstance()->getMyClientID();
			pPacket->targetid = pPlayerComponent->clickedID;
			pPacket->type = CS_PACKET_ATTACK;
			int ret = WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);

			if (ret)
			{

			}
		}
	}

}


int CMainScene::Update(float fTime)
{

//#pragma region Chatting
//	{
//		Chatting* pChatting = GET_SINGLE(UserInterfaceManager)->getChatting();
//		wstring edit_view = L"";
//		for (auto input : pChatting->getChatCont())
//		{
//			//wchar_t temp[64];
//			wchar_t temp2 = input;
//			wstring appendString = L"";
//			appendString = appendString + temp2;
//			edit_view = edit_view + appendString;
//		}
//		pChatting->setChatString(edit_view);
//		pChatting->getUIChatText()->SetText(edit_view);
//	}
//#pragma endregion

	while (false == NetworkManager::getInstance()->getClientPacketQueue().empty())
	{
		void* packet = NetworkManager::getInstance()->fetchToClientPacket();

		if (nullptr == packet)
			continue;

		unsigned char packet_type = reinterpret_cast<unsigned char*>(packet)[2];

		switch (packet_type)
		{
		case SC_PACKET_CHAT_PLAYER:
		{
			sc_packet_chat* pPacket = reinterpret_cast<sc_packet_chat*>(packet);

			Chatting* pChatting = GET_SINGLE(UserInterfaceManager)->getChatting();

			wchar_t str[128];
			int target = pPacket->id;
			wstring tag = _itow(target, str, 10);
			wstring chat = L"Player" + tag + L" : ";
			chat += pPacket->message;

			pChatting->getUserChatLogCont().push_back(chat);

			wstring allChatLog;

			const vector<wstring>& ChatLogCont = pChatting->getUserChatLogCont();
			int index = 0;
			int offset = 0;
			if (ChatLogCont.size() < 8)
			{
				for (int i = 0; i < ChatLogCont.size(); ++i)
				{
					allChatLog += ChatLogCont[i];
					index++;
				}
			}
			else
			{
				offset = ChatLogCont.size() - 7;
				for (int i = offset; i < offset + 7; ++i)
				{
					allChatLog += ChatLogCont[i];
					index++;
				}
			}
			pChatting->getUIChatLogText()->SetText(allChatLog);
		}
		break;

		case SC_PACKET_PUT_PLAYER:
		{
			sc_packet_put_player* pPacket = reinterpret_cast<sc_packet_put_player*>(packet);
			int id = pPacket->id;
			OBJECT_SET_TYPE ObjectSetType = (OBJECT_SET_TYPE)static_cast<sc_packet_put_player*>(packet)->objectSetType;
			
			if (id == NetworkManager::getInstance()->getMyClientID())
			{
				char str[128];
				string appendTag = _itoa(id, str, 10);
				string objectTag = "Player" + appendTag;

				CLayer*	pLayer = m_pScene->GetLayer("Default");

				CGameObject* pPlayerObj = CGameObject::FindObject(objectTag);
				if (nullptr == pPlayerObj)
				{
					pPlayerObj = CGameObject::CreateClone("PlayerCharacter", pLayer);
					this->pPlayer = pPlayerObj;
					pPlayerObj->SetTag(objectTag);
					CTransform*	pTr = pPlayerObj->GetTransform();
					prevPos.x = pPacket->x;
					prevPos.y = pPacket->y;
					prevPos.z = pPacket->z;
					float yPos = GET_SINGLE(CQuadTreeManager)->GetY(Vector3(pPacket->x, pPacket->y, pPacket->z));
					pTr->SetWorldPos(pPacket->x, yPos, pPacket->z);
					pTr->SetWorldScale(2.f, 2.f, 2.f);
					pTr->SetWorldRotY(XMConvertToRadians(pPacket->y));
					SAFE_RELEASE(pTr);

					CPlayer*	pPlayer = pPlayerObj->AddComponent<CPlayer>("Player");
					GET_SINGLE(UserInterfaceManager)->setPlayer(pPlayer);
					pPlayer->setAnimation(pPlayerObj->FindComponentFromType<CAnimation>(CT_ANIMATION));
					SAFE_RELEASE(pPlayer);

					//CGameObject *pSwordObj = CGameObject::CreateObject(objectTag + "Sword", pLayer);
					//CSword	*pSword = pSwordObj->AddComponent<CSword>("Sword");
					//pSword->setTargetPlayerID(id);
					//pSword->initialize();
					//SAFE_RELEASE(pSword);
					//SAFE_RELEASE(pSwordObj);

					//CColliderSphere* pCollider = pPlayerObj->AddComponent<CColliderSphere>("PlayerCollider");
					//pCollider->SetSphere(Vector3(0.f, 0.f, 0.f), 2.f);
					//pCollider->setColor(Vector4::Green);
					//SAFE_RELEASE(pCollider);
					/*CPlayer* pPlayer = pPlayerObj->FindComponentFromTag<CPlayer>("Player");
					pPlayer->setAnimation(pPlayerObj->FindComponentFromType<CAnimation>(CT_ANIMATION));*/

					CGameObject*	pCameraObj = m_pScene->GetMainCameraObj();
					CTransform* pTransform = m_pScene->GetMainCameraTr();
					CTransform* pLightTransform = m_pScene->GetLightCameraTr();
					//Vector3 LightPos = m_pTransform->GetWorldPos();
					Vector3 LightPos = Vector3{ pPacket->x, pPacket->y, pPacket->z } +Vector3{ -15, 15, -15 };
					pLightTransform->SetWorldPos(LightPos);

					//pTransform->SetWorldPos(30.f, 30.f, -15.f);

					/*
						//CGameObject*	pCameraObj = m_pScene->GetMainCameraObj();

						//CThirdCamera*	pThirdCam = pCameraObj->AddComponent<CThirdCamera>("ThirdCamera");

						//SAFE_RELEASE(pThirdCam);

						//CArm*	pArm = pCameraObj->AddComponent<CArm>("Arm");

						//pArm->SetTarget(pPlayerObj);
						//pArm->SetLookAtDist(Vector3(0.f, 1.f, 0.f));
					*/

					CThirdCamera*	pThirdCam = pCameraObj->AddComponent<CThirdCamera>("ThirdCamera");

					SAFE_RELEASE(pThirdCam);

					CArm*	pArm = pCameraObj->AddComponent<CArm>("Arm");

					pArm->SetTarget(pPlayerObj);
					pArm->SetLookAtDist(Vector3(0.f, 1.f, 0.f));

					SAFE_RELEASE(pArm);

					SAFE_RELEASE(pCameraObj);
					SAFE_RELEASE(pTr);
					SAFE_RELEASE(pPlayerObj);
				}
			}
			else if (id < NPC_START)
			{
				char str[128];
				string appendTag = _itoa(id, str, 10);
				string objectTag = "Player" + appendTag;

				CLayer*	pLayer = m_pScene->GetLayer("Default");
				CGameObject* pPlayerObj = CGameObject::FindObject(objectTag);
				if (nullptr == pPlayerObj)
				{
					pPlayerObj = CGameObject::CreateClone("PlayerCharacter", pLayer);

					//pPlayerObj->SetTag(objectTag);
					pPlayerObj->SetTag(objectTag);

					CPlayer*	pPlayer = pPlayerObj->AddComponent<CPlayer>("Player");
					pPlayer->setAnimation(pPlayerObj->FindComponentFromType<CAnimation>(CT_ANIMATION));
					SAFE_RELEASE(pPlayer);

					CGameObject *pSwordObj = CGameObject::CreateObject(objectTag + "Sword", pLayer);
					CSword	*pSword = pSwordObj->AddComponent<CSword>("Sword");
					pSword->setTargetPlayerID(id);
					pSword->initialize();
					SAFE_RELEASE(pSword);
					SAFE_RELEASE(pSwordObj);

					CTransform*	pTr = pPlayerObj->GetTransform();
					CColliderSphere* pCollider = pPlayerObj->AddComponent<CColliderSphere>("collider");
					pCollider->SetSphere(Vector3(0.f, 0.f, 0.f), 2.f);
					SAFE_RELEASE(pCollider);
					float yPos = GET_SINGLE(CQuadTreeManager)->GetY(Vector3(pPacket->x, pPacket->y, pPacket->z));
					pTr->SetWorldPos(pPacket->x, yPos, pPacket->z);
					pTr->SetWorldScale(2.f, 2.f, 2.f);
					pTr->SetWorldRotY(XMConvertToRadians(pPacket->angle));
					SAFE_RELEASE(pTr);
				}
			}
			else
			{
				char a = 0;
				auto check = static_cast<sc_packet_put_player*>(packet)->objectSetType;
				if (static_cast<sc_packet_put_player*>(packet)->objectSetType == OBJECT_SET_TYPE::GOLEM)
				{
					char str[128];
					string appendTag = _itoa(id, str, 10);
					string objectTag = "Player" + appendTag;

					CLayer*	pLayer = m_pScene->GetLayer("Default");
					CGameObject* pGolemObj = CGameObject::FindObject(objectTag);
					if (nullptr == pGolemObj)
					{
						CGameObject* pGolemObject = CGameObject::CreateClone("GolemObject", pLayer);
						pGolemObject->SetTag(objectTag);
						Golem*	pGolem = pGolemObject->AddComponent<Golem>("Golem");
						SAFE_RELEASE(pGolem);
						CTransform*	pTr = pGolemObject->GetTransform();
						CColliderSphere* pCollider = pGolemObject->AddComponent<CColliderSphere>("collider");
						pCollider->SetSphere(Vector3(0.f, 0.f, 0.f), 2.f);
						SAFE_RELEASE(pCollider);
						float yPos = GET_SINGLE(CQuadTreeManager)->GetY(Vector3(pPacket->x, pPacket->y, pPacket->z));
						pTr->SetWorldRotY(XMConvertToRadians(pPacket->angle));
						pTr->SetWorldPos(pPacket->x, yPos, pPacket->z);
						pTr->SetWorldScale(0.5f, 0.5f, 0.5f);
					}
				}
				else if (static_cast<sc_packet_put_player*>(packet)->objectSetType == OBJECT_SET_TYPE::MINO)
				{
					char str[128];
					string appendTag = _itoa(id, str, 10);
					string objectTag = "Player" + appendTag;

					CLayer*	pLayer = m_pScene->GetLayer("Default");
					CGameObject* pGolemObj = CGameObject::FindObject(objectTag);
					if (nullptr == pGolemObj)
					{
						CGameObject* pMinoObject = CGameObject::CreateClone("MinoObject", pLayer);
						pMinoObject->SetTag(objectTag);
						Mino*	pMino = pMinoObject->AddComponent<Mino>("Mino");
						SAFE_RELEASE(pMino);
						CTransform*	pTr = pMinoObject->GetTransform();
						//CColliderSphere* pCollider = pMinoObject->AddComponent<CColliderSphere>("collider");
						//pCollider->SetSphere(Vector3(0.f, 0.f, 0.f), 2.f);
						//SAFE_RELEASE(pCollider);
						float yPos = GET_SINGLE(CQuadTreeManager)->GetY(Vector3(pPacket->x, pPacket->y, pPacket->z));
						pTr->SetLocalRotY(XMConvertToRadians(180.f));
						pTr->SetWorldRotY(XMConvertToRadians(pPacket->angle));
						pTr->SetWorldPos(pPacket->x, yPos, pPacket->z);
						//pTr->SetWorldScale(0.5f, 0.5f, 0.5f);
						
						SAFE_RELEASE(pTr);
					}
				}
				else if (static_cast<sc_packet_put_player*>(packet)->objectSetType == OBJECT_SET_TYPE::SEUTEOMPI)
				{

					char str[128];
					string appendTag = _itoa(id, str, 10);
					string objectTag = "Player" + appendTag;

					CLayer*	pLayer = m_pScene->GetLayer("Default");
					CGameObject* pGolemObj = CGameObject::FindObject(objectTag);
					if (nullptr == pGolemObj)
					{
						CGameObject* pSeuteompiObj = CGameObject::CreateClone("SeuteompiObject", pLayer);
						pSeuteompiObj->SetTag(objectTag);
						Seuteompi*	pSeuteompi = pSeuteompiObj->AddComponent<Seuteompi>("Seuteompi");
						SAFE_RELEASE(pSeuteompi);
						CTransform*	pTr = pSeuteompiObj->GetTransform();
						//CColliderSphere* pCollider = pSeuteompiObj->AddComponent<CColliderSphere>("collider");
						//pCollider->SetSphere(Vector3(0.f, 0.f, 0.f), 2.f);
						//SAFE_RELEASE(pCollider);
						float yPos = GET_SINGLE(CQuadTreeManager)->GetY(Vector3(pPacket->x, pPacket->y, pPacket->z));
						pTr->SetWorldRotY(XMConvertToRadians(pPacket->angle));
						pTr->SetWorldPos(pPacket->x, yPos, pPacket->z);
					}
				}
				else if (static_cast<sc_packet_put_player*>(packet)->objectSetType == OBJECT_SET_TYPE::NPC1)
				{
					/////// < NPC 생성하게끔해야함. >
					////char str[128];
					////string appendTag = _itoa(id, str, 10);
					////string objectTag = "Player" + appendTag;

					////CLayer * pLayer = m_pScene->GetLayer("Default");
					////CGameObject* pNpcObj = CGameObject::FindObject(objectTag);
					////if (nullptr == pNpcObj)
					////{
					////	pNpcObj = CGameObject::CreateObject(objectTag, pLayer);

					////	CRenderer*	pRenderer = pNpcObj->AddComponent<CRenderer>("PlayerRenderer");

					////	pRenderer->SetMesh("npc", L"99.Dynamic_Mesh\\01.npc\\npc.msh");

					////	pNpcObj->SetTag(objectTag);

					////	CAnimation* pAnimation = pNpcObj->AddComponent<CAnimation>("NpcAnimation");

					////	pAnimation->Load("99.Dynamic_Mesh\\01.npc\\npc.anm");

					////	//CColliderSphere* pCollider = pNpcObj->AddComponent<CColliderSphere>("collider");
					////	//pCollider->SetSphere(Vector3(0.f, 0.f, 0.f), 2.f);
					////	//SAFE_RELEASE(pCollider);
					////	CTransform*	pTr = pNpcObj->GetTransform();

					////	float yPos = GET_SINGLE(CQuadTreeManager)->GetY(Vector3(pPacket->x, pPacket->y, pPacket->z));
					////	pTr->SetWorldPos(pPacket->x, yPos, pPacket->z);
					////	pTr->SetWorldRotY(XMConvertToRadians(pPacket->angle));

					////	string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);
					////	string transformPath = meshBasePath + "99.Dynamic_Mesh\\01.npc\\npc.dat";
					////	SAFE_RELEASE(pTr);
					////}
				}
			}
		}
		break;
		case SC_PACKET_MOVE:
		{
			const auto test = CGameObject::getObjectList();
			sc_packet_pos* pPacket = reinterpret_cast<sc_packet_pos*>(packet);
			int id = pPacket->id;
			int myClientID = NetworkManager::getInstance()->getMyClientID();
			if (id == NetworkManager::getInstance()->getMyClientID())
			{
				char str[128];
				string appendTag = to_string(myClientID);
				string objectTag = "Player" + appendTag;

				CGameObject* pGameObject = CGameObject::FindObject(objectTag);
				CTransform* pTransform = pGameObject->GetTransform();
				
				prevPos.x = pPacket->x;
				prevPos.y = pPacket->y;
				prevPos.z = pPacket->z;
				float yPos = GET_SINGLE(CQuadTreeManager)->GetY(Vector3(pPacket->x, pPacket->y, pPacket->z));
				pTransform->SetWorldPos(pPacket->x, yPos, pPacket->z);
				
				//Vector3 vPos = pTransform->GetWorldPos();
				//Vector3 vDir = pTransform->GetWorldAxis(AXIS_Z) * 0.172f;
				//if (GET_SINGLE(CNaviManager)->CheckPosition(vPos, &vDir))
				//{
				//	vPos = vPos + vDir;
				//	float fPosY = GET_SINGLE(CNaviManager)->GetY(vPos);
				//	vPos.y = fPosY;
				//	pTransform->SetWorldPos(vPos);
				//}

				CTransform* pLightTransform = m_pScene->GetLightCameraTr();
				Vector3 LightPos = Vector3{ pPacket->x, pPacket->y, pPacket->z } +Vector3{ -15, 15, -15 };
				pLightTransform->SetWorldPos(LightPos);
				CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
				pAnimation->ChangeClip("Run1");

				SAFE_RELEASE(pAnimation);
				SAFE_RELEASE(pTransform);
				SAFE_RELEASE(pGameObject);
			}
			else if (id < NPC_START)
			{
				char str[128];
				_cprintf("%d", id);
				string appendTag = to_string(id);
				string objectTag = "Player" + appendTag;

				CGameObject* pGameObject = CGameObject::FindObject(objectTag);
				CTransform* pTransform = pGameObject->GetTransform();
				float yPos = GET_SINGLE(CQuadTreeManager)->GetY(Vector3(pPacket->x, pPacket->y, pPacket->z));
				pTransform->SetWorldPos(pPacket->x, yPos, pPacket->z);
				CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
				pAnimation->ChangeClip("Run1");
				SAFE_RELEASE(pAnimation);
				SAFE_RELEASE(pTransform);
				SAFE_RELEASE(pGameObject);
			}
			else
			{
				char str[128];
				string appendTag = to_string(id);
				string objectTag = "Player" + appendTag;

				CGameObject* pGameObject = CGameObject::FindObject(objectTag);

				if (nullptr != pGameObject)
				{
					CTransform* pTransform = pGameObject->GetTransform();
					float yPos = GET_SINGLE(CQuadTreeManager)->GetY(Vector3(pPacket->x, pPacket->y, pPacket->z));
					pTransform->SetWorldPos(pPacket->x, yPos, pPacket->z);
					CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
					pAnimation->ChangeClip("Run1");
					SAFE_RELEASE(pAnimation);
					SAFE_RELEASE(pTransform);
					SAFE_RELEASE(pGameObject);
				}

			}
		}
		break;
		case SC_PACKET_ATTACK_PLAYER:
		{
			char str[128];
			sc_packet_attack_player* pPacket = reinterpret_cast<sc_packet_attack_player*>(packet);
			int id = pPacket->id;
			
			string appendTag = to_string(id);
			string objectTag = "Player" + appendTag;

			if (id == NetworkManager::getInstance()->getMyClientID())
			{
				CGameObject* pGameObject = CGameObject::FindObject(objectTag);
				if (nullptr != pGameObject)
				{
					CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
					pAnimation->ChangeClip("Attack1");
					SAFE_RELEASE(pAnimation);
					SAFE_RELEASE(pGameObject);
				}

				{
					int target = pPacket->targetid;
					int damage = pPacket->damage;
					string appendTag = _itoa(target, str, 10);
					string objectTag = "Player" + appendTag;
					CGameObject* pTargetObject = CGameObject::FindObject(objectTag);
					if (nullptr != pTargetObject)
					{

						if (reinterpret_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::GOLEM)
						{
							Golem* pGolem = pTargetObject->FindComponentFromTag<Golem>("golem");

							int hp = pGolem->getCurrentHP() - pPacket->damage;
							//CTransform* pTransform = pGolem->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);
							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGolem->getMaxHP();
								pGolem->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(0.f);
							}
							else
							{
								float ratio = (float)hp / (float)pGolem->getMaxHP();
								pGolem->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(ratio);
							}
						}
						else if (reinterpret_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::MINO)
						{
							Mino* pMino = pTargetObject->FindComponentFromTag<Mino>("Mino");

							int hp = pMino->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pMino->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{

								float ratio = (float)hp / (float)pMino->getMaxHP();
								pMino->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(0.f);
							}
							else
							{
								float ratio = (float)hp / (float)pMino->getMaxHP();
								pMino->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(ratio);

							}
						}
						else if (reinterpret_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::SEUTEOMPI)
						{
							Seuteompi* pSeuteompi = pTargetObject->FindComponentFromTag<Seuteompi>("Seuteompi");

							int hp = pSeuteompi->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pSeuteompi->getMaxHP();
								pSeuteompi->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(0.f);
							}
							else
							{
								float ratio = (float)hp / (float)pSeuteompi->getMaxHP();
								pSeuteompi->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(ratio);
							}
						}
					}
				}
			}
			else if (id < NPC_START)
			{
				CGameObject* pGameObject = CGameObject::FindObject(objectTag);
				if (nullptr != pGameObject)
				{
					CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
					pAnimation->ChangeClip("Attack1");
					SAFE_RELEASE(pAnimation);
					SAFE_RELEASE(pGameObject);
				}

				{
					int target = pPacket->targetid;
					int damage = pPacket->damage;
					string appendTag = _itoa(target, str, 10);
					string objectTag = "Player" + appendTag;
					CGameObject* pTargetObject = CGameObject::FindObject(objectTag);
					if (nullptr != pTargetObject)
					{

						if (reinterpret_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::GOLEM)
						{
							Golem* pGolem = pTargetObject->FindComponentFromTag<Golem>("golem");

							int hp = pGolem->getCurrentHP() - pPacket->damage;

							CTransform* pTransform = pGolem->GetTransform();
							Vector3 vPos = pTransform->GetWorldPos();
							Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							vPos += vLook * 1.25f;
							vPos.y += 0.95f;
							GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGolem->getMaxHP();
								pGolem->setCurrentHP(hp);
							}
							else
							{
								float ratio = (float)hp / (float)pGolem->getMaxHP();
								pGolem->setCurrentHP(hp);
							}
						}
						else if (reinterpret_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::MINO)
						{
							Mino* pMino = pTargetObject->FindComponentFromTag<Mino>("Mino");

							int hp = pMino->getCurrentHP() - pPacket->damage;

							CTransform* pTransform = pMino->GetTransform();
							Vector3 vPos = pTransform->GetWorldPos();
							Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							vPos += vLook * 1.25f;
							vPos.y += 0.95f;
							GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pMino->getMaxHP();
								pMino->setCurrentHP(hp);
							}
							else
							{
								float ratio = (float)hp / (float)pMino->getMaxHP();
								pMino->setCurrentHP(hp);

							}
						}
						else if (reinterpret_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::SEUTEOMPI)
						{
							Seuteompi* pSeuteompi = pTargetObject->FindComponentFromTag<Seuteompi>("Seuteompi");

							int hp = pSeuteompi->getCurrentHP() - pPacket->damage;

							CTransform* pTransform = pSeuteompi->GetTransform();
							Vector3 vPos = pTransform->GetWorldPos();
							Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							vPos += vLook * 1.25f;
							vPos.y += 0.95f;
							GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pSeuteompi->getMaxHP();
								pSeuteompi->setCurrentHP(hp);
							}
							else
							{
								float ratio = (float)hp / (float)pSeuteompi->getMaxHP();
								pSeuteompi->setCurrentHP(hp);
							}
						}
					}
				}
			}
			else
			{
				CGameObject* pGameObject = CGameObject::FindObject(objectTag);
				if (nullptr != pGameObject)
				{
					CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
					pAnimation->ChangeClip("Attack1");
					SAFE_RELEASE(pAnimation);
					SAFE_RELEASE(pGameObject);
				}

				{
					int target = pPacket->targetid;
					int damage = pPacket->damage;
					string appendTag = to_string(target);
					string objectTag = "Player" + appendTag;
					CGameObject* pTargetObject = CGameObject::FindObject(objectTag);
					
					if (nullptr != pTargetObject)
					{
						if (NetworkManager::getInstance()->getMyClientID() == pPacket->targetid)
						{
							CPlayer* pPlayer = pTargetObject->FindComponentFromTag<CPlayer>("Player");
							int hp = pPlayer->getCurrentHP() - pPacket->damage;
							if (hp < 0)
							{
								float ratio = (float)hp / (float)pPlayer->getMaxHP();
								pPlayer->setCurrentHP(hp);
								CUIButton* pUIHearthBar = GET_SINGLE(UserInterfaceManager)->getUIHeartBar();
								pUIHearthBar->setLengthRatio(0.f);
								_cprintf("플레이어 사망처리");
							}
							else
							{
								float ratio = (float)hp / (float)pPlayer->getMaxHP();
								pPlayer->setCurrentHP(hp);
								CUIButton* pUIHearthBar = GET_SINGLE(UserInterfaceManager)->getUIHeartBar();
								pUIHearthBar->setLengthRatio(ratio);
							}
						}
					}

				}
			}

		}
		break;
		case SC_PACKET_ATTACK_SWORD_SKILL1:
		{
			/*char str[128];
			sc_packet_attack_player* pPacket = reinterpret_cast<sc_packet_attack_player*>(packet);
			int id = pPacket->id;
			string appendTag = _itoa(id, str, 10);
			string objectTag = "Player" + appendTag;

			if (id == NetworkManager::getInstance()->getMyClientID())
			{
				CGameObject* pGameObject = CGameObject::FindObject(objectTag);
				if (nullptr != pGameObject)
				{
					CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
					pAnimation->ChangeClip("Attack_S");
					SAFE_RELEASE(pAnimation);
					SAFE_RELEASE(pGameObject);
				}

				{

					int target = pPacket->targetid;
					int damage = pPacket->damage;
					string appendTag = to_string(target);
					string objectTag = "Player" + appendTag;
					CGameObject* pTargetObject = CGameObject::FindObject(objectTag);
					if (nullptr != pTargetObject)
					{
						if (reinterpret_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::GOLEM)
						{
							Golem* pGolem = pTargetObject->FindComponentFromTag<Golem>("golem");

							int hp = pGolem->getCurrentHP() - pPacket->damage;
							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGolem->getMaxHP();
								pGolem->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(0.f);
								_cprintf("플레이어 사망처리");
							}
							else
							{
								float ratio = (float)hp / (float)pGolem->getMaxHP();
								pGolem->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(ratio);
							}
						}
						else if (reinterpret_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::MINO)
						{
							Mino* pMino = pTargetObject->FindComponentFromTag<Mino>("Mino");

							int hp = pMino->getCurrentHP() - pPacket->damage;
							if (hp < 0)
							{
								float ratio = (float)hp / (float)pMino->getMaxHP();
								pMino->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(0.f);
								_cprintf("플레이어 사망처리");
							}
							else
							{
								float ratio = (float)hp / (float)pMino->getMaxHP();
								pMino->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(ratio);
							}
						}
						else if (reinterpret_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::SEUTEOMPI)
						{
							Seuteompi* pSeuteompi = pTargetObject->FindComponentFromTag<Seuteompi>("Seuteompi");

							int hp = pSeuteompi->getCurrentHP() - pPacket->damage;
							if (hp < 0)
							{
								float ratio = (float)hp / (float)pSeuteompi->getMaxHP();
								pSeuteompi->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(0.f);
								_cprintf("플레이어 사망처리");
							}
							else
							{
								float ratio = (float)hp / (float)pSeuteompi->getMaxHP();
								pSeuteompi->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(ratio);
							}
						}
					}
				}
			}*/
		}
		break;
		case SC_PACKET_ROTATE_X:
		case SC_PACKET_ROTATE_Y:
		case SC_PACKET_ROTATE_Z:
		{
			sc_packet_rotate* pPacket = reinterpret_cast<sc_packet_rotate*>(packet);
			int id = pPacket->id;
			int myClientID = NetworkManager::getInstance()->getMyClientID();
			if (id == NetworkManager::getInstance()->getMyClientID())
			{
				char str[128];
				_cprintf("%d", myClientID);
				string appendTag = to_string(myClientID);
				string objectTag = "Player" + appendTag;

				CGameObject* pGameObject = CGameObject::FindObject(objectTag);
				CTransform* pTransform = pGameObject->GetTransform();

				if (SC_PACKET_ROTATE_X == packet_type)
				{
					pTransform->SetWorldRotX(XMConvertToRadians(pPacket->angle));
				}
				else if (SC_PACKET_ROTATE_Y == packet_type)
				{
					pTransform->SetWorldRotY(XMConvertToRadians(pPacket->angle));
				}
				else if (SC_PACKET_ROTATE_Z == packet_type)
				{
					pTransform->SetWorldRotZ(XMConvertToRadians(pPacket->angle));
				}

				CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
				pAnimation->ChangeClip("Run1");
				SAFE_RELEASE(pAnimation);
				SAFE_RELEASE(pTransform);
				SAFE_RELEASE(pGameObject);

			}
			else if (id < NPC_START)
			{
				char str[128];
				_cprintf("%d", id);
				string appendTag = to_string(id);
				string objectTag = "Player" + appendTag;
				_cprintf("%s", appendTag.c_str());
				_cprintf("%s", objectTag.c_str());
				CGameObject* pGameObject = CGameObject::FindObject(objectTag);
				CTransform* pTransform = pGameObject->GetTransform();

				if (SC_PACKET_ROTATE_X == packet_type)
				{
					pTransform->SetWorldRotX(XMConvertToRadians(pPacket->angle));
				}
				else if (SC_PACKET_ROTATE_Y == packet_type)
				{
					pTransform->SetWorldRotY(XMConvertToRadians(pPacket->angle));
				}
				else if (SC_PACKET_ROTATE_Z == packet_type)
				{
					pTransform->SetWorldRotZ(XMConvertToRadians(pPacket->angle));
				}
				CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
				pAnimation->ChangeClip("Run1");

				SAFE_RELEASE(pAnimation);
				SAFE_RELEASE(pTransform);
				SAFE_RELEASE(pGameObject);
			}
			else
			{
				char str[128];
				string appendTag = _itoa(id, str, 10);
				string objectTag = "Player" + appendTag;

				CGameObject* pGameObject = CGameObject::FindObject(objectTag);
				if (pGameObject != nullptr)
				{
					CTransform* pTransform = pGameObject->GetTransform();

					if (SC_PACKET_ROTATE_X == packet_type)
					{
						pTransform->SetWorldRotX(XMConvertToRadians(pPacket->angle));
					}
					else if (SC_PACKET_ROTATE_Y == packet_type)
					{
						pTransform->SetWorldRotY(XMConvertToRadians(pPacket->angle));
					}
					else if (SC_PACKET_ROTATE_Z == packet_type)
					{
						pTransform->SetWorldRotZ(XMConvertToRadians(pPacket->angle));
					}
					CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
					pAnimation->ChangeClip("Run1");

					SAFE_RELEASE(pAnimation);
					SAFE_RELEASE(pTransform);
					SAFE_RELEASE(pGameObject);
				}
			}
		};
		break;
		case SC_PACKET_ROTATE_X_CI:
		case SC_PACKET_ROTATE_Y_CI:
		case SC_PACKET_ROTATE_Z_CI:
		{
			sc_packet_rotate* pPacket = reinterpret_cast<sc_packet_rotate*>(packet);
			int id = pPacket->id;
			char str[128];
			string appendTag = _itoa(id, str, 10);
			string objectTag = "Player" + appendTag;

			CGameObject* pGameObject = CGameObject::FindObject(objectTag);
			if (pGameObject != nullptr)
			{
				CTransform* pTransform = pGameObject->GetTransform();

				if (SC_PACKET_ROTATE_X_CI == packet_type)
				{

					pTransform->RotateWorldX(XMConvertToRadians(pPacket->angle));
				}
				else if (SC_PACKET_ROTATE_Y_CI == packet_type)
				{
					pTransform->RotateWorldY(XMConvertToRadians(pPacket->angle));
				}
				else if (SC_PACKET_ROTATE_Z_CI == packet_type)
				{
					pTransform->RotateWorldZ(XMConvertToRadians(pPacket->angle));
				}
				CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
				pAnimation->ChangeClip("Run1");

				SAFE_RELEASE(pAnimation);
				SAFE_RELEASE(pTransform);
				SAFE_RELEASE(pGameObject);
			}
		}
		break;

		case SC_PACKET_DIE_ANIMATION:
		{
			sc_packet_animation_player* pPacket = reinterpret_cast<sc_packet_animation_player*>(packet);
			int id = pPacket->id;
			char str[128];
			string appendTag = _itoa(id, str, 10);
			string objectTag = "Player" + appendTag;
			CGameObject* pGameObject = CGameObject::FindObject(objectTag);
			if (pGameObject != nullptr)
			{
				CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
				pAnimation->ChangeClip("Die1");
				SAFE_RELEASE(pAnimation);
				Mino* pMino = pGameObject->FindComponentFromTag<Mino>("Mino");
				if (nullptr != pMino)
				{
					pMino->setDieState(true);
					SAFE_RELEASE(pMino);
				}
				Golem* pGolem = pGameObject->FindComponentFromTag<Golem>("Golem");
				if (nullptr != pGolem)
				{
					pGolem->setDieState(true);
					SAFE_RELEASE(pMino);
				}
				Seuteompi* pSeuteompi = pGameObject->FindComponentFromTag<Seuteompi>("Seuteompi");
				if (nullptr != pSeuteompi)
				{
					pSeuteompi->setDieState(true);
					SAFE_RELEASE(pMino);
				}
			}
		}
		break;

		case SC_PACKET_REMOVE_PLAYER:
		{
			sc_packet_pos* pPacket = reinterpret_cast<sc_packet_pos*>(packet);
			int id = pPacket->id;
			char str[128];
			string appendTag = _itoa(id, str, 10);
			string objectTag = "Player" + appendTag;

			CGameObject* pRemoveObject = CGameObject::FindObject(objectTag);

			if (nullptr != pRemoveObject)
			{
				pRemoveObject->Die();
			}
		}
		break;

		case SC_PACKET_ROOTING_TABLE:
		{
			sc_packet_require_itemtable* pPacket = reinterpret_cast<sc_packet_require_itemtable*>(packet);
			//int id = pPacket->id;
			DropTableUI* pDropTableUI = GET_SINGLE(UserInterfaceManager)->getDropTableUI();

			for (int i = 0; i < pPacket->count; ++i)
			{
				pDropTableUI->addDropItemSlot(pPacket->itemname1 + (i*100));
			}
			//pDropTableUI->addDropItemSlot(pPacket->itemname1);
			//pDropTableUI->addDropItemSlot(pPacket->itemname2);
			//pDropTableUI->addDropItemSlot(pPacket->itemname3);

			pDropTableUI->enableRender(true);
		}
		break;
		case SC_PACKET_ADDITEM_INVENTORY:
		{
			sc_packet_additem_inventory* pPacket = reinterpret_cast<sc_packet_additem_inventory*>(packet);
			wstring tempitemname = pPacket->itemname;

			CLayer*	pLayer = m_pScene->GetLayer("UI+2");
			CGameObject* pItem = CGameObject::CreateObject("SWORD_ITEM", pLayer);

			CUIButton*	pItemUI = pItem->AddComponent<CUIButton>("SWORD_ITEM");
			SAFE_RELEASE(pItemUI);

			CTransform*	pItemTr = pItem->GetTransform();
			//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
			pItemTr->SetWorldScale(30.f, 30.f, 1.f);

			SAFE_RELEASE(pItemTr);
			CRenderer2D* pRenderer = pItem->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
			CMaterial* pMaterial = pRenderer->GetMaterial();
			tempitemname = L"ItemIcon/" +tempitemname +L".png";
			string itemname = strconv(tempitemname);
			pMaterial->SetDiffuseTexInfo("Linear", itemname, 0, 0, tempitemname.c_str());
			SAFE_RELEASE(pMaterial);
			SAFE_RELEASE(pRenderer);

			CColliderRect* pRC = pItem->FindComponentFromType<CColliderRect>(CT_COLLIDER);

			pRC->SetRect(0, 0, 0, 0);

			SAFE_RELEASE(pRC);

			GET_SINGLE(UserInterfaceManager)->getInventory()->addItem(pItem);
			SAFE_RELEASE(pItem);

			DropTableUI* pDropTableUI = GET_SINGLE(UserInterfaceManager)->getDropTableUI();
			pDropTableUI->detachDropItemSlot(pPacket->index);
		}
		break;
		case SC_PACKET_INVITE_PARTY:
		{
			sc_packet_party* pPacket = reinterpret_cast<sc_packet_party*>(packet);
			PGMessageBox* pMessageBox = GET_SINGLE(UserInterfaceManager)->getPGMessageBox();
			pMessageBox->enableRender(true);
			GET_SINGLE(UserInterfaceManager)->getTargetPlayerUI()->setFromID(pPacket->fromID);
			GET_SINGLE(UserInterfaceManager)->getTargetPlayerUI()->setToID(pPacket->toID);
		}
		break;

		case SC_PACKET_PARTY_ADD:
		{
			// 해당 플레이어의 정보로 왼쪽 파티 프레임에 띄운다.
			sc_packet_party* pPacket = reinterpret_cast<sc_packet_party*>(packet);
			
			string objectID = "Player" + to_string(pPacket->toID);
			//CGameObject* pPlayerObj = CGameObject::FindObject(objectID);

			GET_SINGLE(UserInterfaceManager)->addPartyPlayer(objectID);
			//SAFE_RELEASE(pPlayerObj);
		}
		break;

		default:
			break;
		}
		NetworkManager::getInstance()->popData();
		free(packet);
	}

	return 0;
}

int CMainScene::LateUpdate(float fTime)
{
	GET_SINGLE(UserInterfaceManager)->update(fTime);
	return 0;
}

void CMainScene::Render(float fTime)
{
}

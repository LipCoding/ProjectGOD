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
#pragma region Layer Setting
	{
		CLayer* pLayer = m_pScene->CreateLayer("UI+1", UI_LAYER + 1);
		SAFE_RELEASE(pLayer);
	}
	{
		CLayer* pLayer = m_pScene->CreateLayer("UI+2", UI_LAYER + 2);
		SAFE_RELEASE(pLayer);
	}
#pragma endregion

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

		pLandScape->Load_Terrain("village_500");

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
	// 경로 지정
	wchar_t strPath[MAX_PATH] = {};
	wcscpy_s(strPath, MAX_PATH, GET_SINGLE(CPathManager)->FindPath(DATA_PATH));
	wcscat_s(strPath, MAX_PATH, L"Object\\village_500.bin");

	ifstream file;
	file.open(strPath, ios::in);

	if (!file.is_open())
		return false;

	int iObjSize = 0;
	file >> iObjSize;

	for (int i = 0; i < iObjSize; i++)
	{
		string objName = "ObjName_" + to_string(i);

		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer* pLayer = pScene->GetLayer("Default");
		CGameObject *pObj = CGameObject::CreateObject(objName, pLayer);

		string objTag;
		file >> objTag;

		// Mesh
		string meshPath, meshRestPath;
		meshPath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);
		meshRestPath = objTag;

		string meshDataPath;
		meshDataPath = meshPath + meshRestPath + ".msh";

		CRenderer* pRenderer = pObj->AddComponent<CRenderer>("Renderer");

		wstring wMeshDataPath;
		wMeshDataPath.assign(meshDataPath.begin(), meshDataPath.end());
		pRenderer->SetMeshFromFullPath(objTag, wMeshDataPath.c_str());

		SAFE_RELEASE(pRenderer);

		// Transform
		// Local Transform Data
		string localDataPath;

		localDataPath = meshPath + meshRestPath + ".dat";

		FILE* pFile = nullptr;

		fopen_s(&pFile, localDataPath.c_str(), "rb");

		if (!pFile)
			return false;

		CTransform* pTr = pObj->GetTransform();
		pTr->Load_Local(pFile);
		fclose(pFile);

		// World Transform Data
		Vector3 vScale, vRotation, vPos;
		file >> vScale.x >> vScale.y >> vScale.z;
		file >> vRotation.x >> vRotation.y >> vRotation.z;
		file >> vPos.x >> vPos.y >> vPos.z;

		pTr->SetWorldScale(vScale);
		pTr->SetWorldRot(vRotation);
		pTr->SetWorldPos(vPos);

		SAFE_RELEASE(pTr);
	}
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

			pRenderer->SetMesh("Player", L"99.Dynamic_Mesh\\00.Player\\Player.msh");
			//pRenderer->SetForwardShader();

			string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);

			//string animPath = meshBasePath + "99.Dynamic_Mesh\\00.Player\\Player.anm";

			//CAnimation* pAnimation = pPlayerObj->AddComponent<CAnimation>("PlayerAnimation");
			//pAnimation->LoadFromFullPath(animPath.c_str());

			string transformPath = meshBasePath + "99.Dynamic_Mesh\\00.Player\\Player.dat";

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

			CRenderer*	pRenderer = pGolemObj->AddComponent<CRenderer>("PlayerRenderer");

			pRenderer->SetMesh("golem", L"99.Dynamic_Mesh\\02.Monster\\golem.msh");

			string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);

			string transformPath = meshBasePath + "99.Dynamic_Mesh\\02.Monster\\golem.dat";

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

			pRenderer->SetMesh("mino", L"99.Dynamic_Mesh\\02.Monster\\Mino.msh");


			string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);

			string transformPath = meshBasePath + "99.Dynamic_Mesh\\02.Monster\\Mino.dat";

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

			pRenderer->SetMesh("seuteompi", L"99.Dynamic_Mesh\\02.Monster\\seuteompi.msh");


			string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);

			string transformPath = meshBasePath + "99.Dynamic_Mesh\\02.Monster\\seuteompi.dat";

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

	//	CUIButton*	pButton = pUIChat->AddComponent<CUIButton>("ChatEdit");
	//	pButton->SetCallback(this, &CMainScene::chat_callback);
	//	SAFE_RELEASE(pButton);

	//	CTransform*	pButtonTr = pUIChat->GetTransform();

	//	//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
	//	pButtonTr->SetWorldScale(200.f, 50.f, 1.f);
	//	pButtonTr->SetWorldPos(0.f, DEVICE_RESOLUTION.iHeight - 120.f, 0.f);

	//	SAFE_RELEASE(pButtonTr);
	//	CRenderer2D* pRenderer = pUIChat->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
	//	CMaterial* pMaterial = pRenderer->GetMaterial();

	//	pMaterial->SetDiffuseTexInfo("Linear", "ChatEdit",
	//		0, 0, L"LoginEdit.bmp");

	//	SAFE_RELEASE(pMaterial);
	//	SAFE_RELEASE(pRenderer);

	//	CColliderRect* pRC = pUIChat->FindComponentFromType<CColliderRect>(CT_COLLIDER);

	//	pRC->SetRect(0, 0, 200, 50);

	//	SAFE_RELEASE(pRC);

	//	pUIChatText = pUIChat->AddComponent<CFont>("TextUI");
	//	pUIChatText->SetFont("나눔고딕");
	//	pChatString = L"채팅 테스트";
	//	pUIChatText->SetText(pChatString);
	//	pUIChatText->SetArea(0, 0, 200, 30.f);
	//}

#pragma region UI

#pragma endregion



	return true;
}

int CMainScene::Update(float fTime)
{
	if (false == isInitialize)
	{
		CMainScene::Init();
		isInitialize = true;

		{
			CLayer*	pUILayer = m_pScene->GetLayer("UI");

			CGameObject*	pHPUIObj = CGameObject::CreateObject("HPUI", pUILayer);
			CUIButton* pUIHearthBar = pHPUIObj->AddComponent<CUIButton>("HPUI");
			pUIHearthBar->setUILength(200.f);
			pUIHearthBar->setLengthRatio(1.f);
			GET_SINGLE(UserInterfaceManager)->setUIHearthBar(pUIHearthBar);

			CTransform*	pButtonTr = pHPUIObj->GetTransform();
			//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
			pButtonTr->SetWorldScale(200.f, 25.f, 1.f);
			pButtonTr->SetWorldPos(25.f, 25.f, 0.f);

			SAFE_RELEASE(pButtonTr);

			CRenderer2D* pRenderer = pHPUIObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
			pRenderer->SetShader(UI_HEARTH_BAR_SHADER);
			CMaterial* pMaterial = pRenderer->GetMaterial();

			pMaterial->SetDiffuseTexInfo("Linear", "HealthPoint",
				0, 0, L"HearthPoint.bmp");

			SAFE_RELEASE(pMaterial);

			SAFE_RELEASE(pRenderer);

			CColliderRect* pRC = pHPUIObj->FindComponentFromType<CColliderRect>(CT_COLLIDER);

			pRC->SetRect(0, 0, 200, 25);

			SAFE_RELEASE(pRC);

			SAFE_RELEASE(pHPUIObj);

			SAFE_RELEASE(pUILayer);
		}

		{
			CLayer*	pUILayer = m_pScene->GetLayer("UI");

			CGameObject*	pHPUIObj = CGameObject::CreateObject("Enemy1_HPUI", pUILayer);
			CUIButton* pEnemyUIHearthBar = pHPUIObj->AddComponent<CUIButton>("HPUI");
			pEnemyUIHearthBar->setUILength(200.f);
			pEnemyUIHearthBar->setLengthRatio(1.f);
			GET_SINGLE(UserInterfaceManager)->setEnemyUIHearthBar(pEnemyUIHearthBar);

			CTransform*	pButtonTr = pHPUIObj->GetTransform();

			//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
			pButtonTr->SetWorldScale(200.f, 25.f, 1.f);
			pButtonTr->SetWorldPos(300.f, 25.f, 0.f);

			SAFE_RELEASE(pButtonTr);

			CRenderer2D* pRenderer = pHPUIObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
			pRenderer->SetShader(UI_HEARTH_BAR_SHADER);
			CMaterial* pMaterial = pRenderer->GetMaterial();

			pMaterial->SetDiffuseTexInfo("Linear", "HealthPoint",
				0, 0, L"HearthPoint.bmp");

			SAFE_RELEASE(pMaterial);

			SAFE_RELEASE(pRenderer);

			CColliderRect* pRC = pHPUIObj->FindComponentFromType<CColliderRect>(CT_COLLIDER);

			pRC->SetRect(0, 0, 200, 25);

			SAFE_RELEASE(pRC);

			SAFE_RELEASE(pHPUIObj);

			SAFE_RELEASE(pUILayer);
		}

		GET_SINGLE(UserInterfaceManager)->initialize();

#pragma region sound
		GET_SINGLE(SoundManager)->LoadSound("MainSceneBGM", false, "MainSound.mp3");

		GET_SINGLE(SoundManager)->Play("MainSceneBGM", SC_BGM);
#pragma endregion

#pragma region Item
		{
			CLayer*	pLayer = m_pScene->GetLayer("UI+2");
			CGameObject* pItem = CGameObject::CreateObject("SWORD_ITEM", pLayer);

			CUIButton*	pItemUI = pItem->AddComponent<CUIButton>("SWORD_ITEM");
			SAFE_RELEASE(pItemUI);

			CTransform*	pItemTr = pItem->GetTransform();
			//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
			pItemTr->SetWorldScale(30.f, 30.f, 1.f);
			pItemTr->SetWorldPos(DEVICE_RESOLUTION.iWidth / 2.f - 100.f,
				DEVICE_RESOLUTION.iHeight / 2.f, 0.f);

			SAFE_RELEASE(pItemTr);
			CRenderer2D* pRenderer = pItem->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
			CMaterial* pMaterial = pRenderer->GetMaterial();

			pMaterial->SetDiffuseTexInfo("Linear", "SWORD_ICON",
				0, 0, L"SWORD_ICON.jpg");

			SAFE_RELEASE(pMaterial);
			SAFE_RELEASE(pRenderer);

			CColliderRect* pRC = pItem->FindComponentFromType<CColliderRect>(CT_COLLIDER);

			pRC->SetRect(0, 0, 30, 30);

			SAFE_RELEASE(pRC);

			GET_SINGLE(UserInterfaceManager)->getInventory()->addItem(pItem);
			SAFE_RELEASE(pItem);
		}

		{
			CLayer*	pLayer = m_pScene->GetLayer("UI+2");
			CGameObject* pItem = CGameObject::CreateObject("SWORD_ITEM2", pLayer);

			CUIButton*	pItemUI = pItem->AddComponent<CUIButton>("SWORD_ITEM");
			SAFE_RELEASE(pItemUI);

			CTransform*	pItemTr = pItem->GetTransform();
			//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
			pItemTr->SetWorldScale(30.f, 30.f, 1.f);
			pItemTr->SetWorldPos(DEVICE_RESOLUTION.iWidth / 2.f - 100.f,
				DEVICE_RESOLUTION.iHeight / 2.f, 0.f);

			SAFE_RELEASE(pItemTr);
			CRenderer2D* pRenderer = pItem->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
			CMaterial* pMaterial = pRenderer->GetMaterial();

			pMaterial->SetDiffuseTexInfo("Linear", "SWORD_ICON",
				0, 0, L"SWORD_ICON.jpg");

			SAFE_RELEASE(pMaterial);
			SAFE_RELEASE(pRenderer);

			CColliderRect* pRC = pItem->FindComponentFromType<CColliderRect>(CT_COLLIDER);

			pRC->SetRect(0, 0, 30, 30);

			SAFE_RELEASE(pRC);

			GET_SINGLE(UserInterfaceManager)->getInventory()->addItem(pItem);
			SAFE_RELEASE(pItem);
		}

		{
			CLayer*	pLayer = m_pScene->GetLayer("UI+2");
			CGameObject* pItem = CGameObject::CreateObject("SWORD_ITEM3", pLayer);

			CUIButton*	pItemUI = pItem->AddComponent<CUIButton>("SWORD_ITEM");
			SAFE_RELEASE(pItemUI);

			CTransform*	pItemTr = pItem->GetTransform();
			//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
			pItemTr->SetWorldScale(30.f, 30.f, 1.f);
			pItemTr->SetWorldPos(DEVICE_RESOLUTION.iWidth / 2.f - 100.f,
				DEVICE_RESOLUTION.iHeight / 2.f, 0.f);

			SAFE_RELEASE(pItemTr);
			CRenderer2D* pRenderer = pItem->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
			CMaterial* pMaterial = pRenderer->GetMaterial();

			pMaterial->SetDiffuseTexInfo("Linear", "SWORD_ICON",
				0, 0, L"SWORD_ICON.jpg");

			SAFE_RELEASE(pMaterial);
			SAFE_RELEASE(pRenderer);

			CColliderRect* pRC = pItem->FindComponentFromType<CColliderRect>(CT_COLLIDER);

			pRC->SetRect(0, 0, 30, 30);

			SAFE_RELEASE(pRC);

			GET_SINGLE(UserInterfaceManager)->getInventory()->addItem(pItem);
			SAFE_RELEASE(pItem);
		}
#pragma endregion
	}

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

		char packet_type = reinterpret_cast<char*>(packet)[2];

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
				offset = ChatLogCont.size() - 8;
				for (int i = offset; i < offset + 8; ++i)
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
					pTr->SetWorldRot(0.f, 0.0f, 0.f);
					SAFE_RELEASE(pTr);

					CPlayer*	pPlayer = pPlayerObj->AddComponent<CPlayer>("Player");
					pPlayer->setAnimation(pPlayerObj->FindComponentFromType<CAnimation>(CT_ANIMATION));
					SAFE_RELEASE(pPlayer);
					CColliderSphere* pCollider = pPlayerObj->AddComponent<CColliderSphere>("PlayerCollider");
					pCollider->SetSphere(Vector3(0.f, 0.f, 0.f), 2.f);
					pCollider->setColor(Vector4::Green);
					SAFE_RELEASE(pCollider);
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
					CTransform*	pTr = pPlayerObj->GetTransform();
					CColliderSphere* pCollider = pPlayerObj->AddComponent<CColliderSphere>("collider");
					pCollider->SetSphere(Vector3(0.f, 0.f, 0.f), 2.f);
					SAFE_RELEASE(pCollider);
					float yPos = GET_SINGLE(CQuadTreeManager)->GetY(Vector3(pPacket->x, pPacket->y, pPacket->z));
					pTr->SetWorldPos(pPacket->x, yPos, pPacket->z);
					pTr->SetWorldScale(2.f, 2.f, 2.f);
					pTr->SetWorldRot(0.f, 0.0f, 0.f);
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
						CColliderSphere* pCollider = pMinoObject->AddComponent<CColliderSphere>("collider");
						pCollider->SetSphere(Vector3(0.f, 0.f, 0.f), 2.f);
						SAFE_RELEASE(pCollider);
						float yPos = GET_SINGLE(CQuadTreeManager)->GetY(Vector3(pPacket->x, pPacket->y, pPacket->z));
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
						CColliderSphere* pCollider = pSeuteompiObj->AddComponent<CColliderSphere>("collider");
						pCollider->SetSphere(Vector3(0.f, 0.f, 0.f), 2.f);
						SAFE_RELEASE(pCollider);
						float yPos = GET_SINGLE(CQuadTreeManager)->GetY(Vector3(pPacket->x, pPacket->y, pPacket->z));
						pTr->SetWorldPos(pPacket->x, yPos, pPacket->z);
					}
				}
				else if (static_cast<sc_packet_put_player*>(packet)->objectSetType == OBJECT_SET_TYPE::NPC1)
				{
					/// < NPC 생성하게끔해야함. >
					char str[128];
					string appendTag = _itoa(id, str, 10);
					string objectTag = "Player" + appendTag;

					CLayer * pLayer = m_pScene->GetLayer("Default");
					CGameObject* pNpcObj = CGameObject::FindObject(objectTag);
					if (nullptr == pNpcObj)
					{
						pNpcObj = CGameObject::CreateObject(objectTag, pLayer);

						CRenderer*	pRenderer = pNpcObj->AddComponent<CRenderer>("PlayerRenderer");

						pRenderer->SetMesh("npc", L"99.Dynamic_Mesh\\01.npc\\npc.msh");

						pNpcObj->SetTag(objectTag);

						CAnimation* pAnimation = pNpcObj->AddComponent<CAnimation>("NpcAnimation");

						pAnimation->Load("99.Dynamic_Mesh\\01.npc\\npc.anm");

						CColliderSphere* pCollider = pNpcObj->AddComponent<CColliderSphere>("collider");
						pCollider->SetSphere(Vector3(0.f, 0.f, 0.f), 2.f);
						SAFE_RELEASE(pCollider);
						CTransform*	pTr = pNpcObj->GetTransform();

						float yPos = GET_SINGLE(CQuadTreeManager)->GetY(Vector3(pPacket->x, pPacket->y, pPacket->z));
						pTr->SetWorldPos(pPacket->x, yPos, pPacket->z);
						pTr->SetWorldScale(1.f, 1.f, 1.f);
						pTr->SetLocalRot(-PG_PI / 2.f, PG_PI, 0.f);
						pTr->SetWorldRot(0.f, 0.f, 0.f);

						string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);
						string transformPath = meshBasePath + "99.Dynamic_Mesh\\01.npc\\npc.dat";
						SAFE_RELEASE(pTr);
					}
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
				string appendTag = _itoa(myClientID, str, 10);
				string objectTag = "Player" + appendTag;

				CGameObject* pGameObject = CGameObject::FindObject(objectTag);
				CTransform* pTransform = pGameObject->GetTransform();

				prevPos.x = pPacket->x;
				prevPos.y = pPacket->y;
				prevPos.z = pPacket->z;
				float yPos = GET_SINGLE(CQuadTreeManager)->GetY(Vector3(pPacket->x, pPacket->y, pPacket->z));
				pTransform->SetWorldPos(pPacket->x, yPos, pPacket->z);

				CTransform* pLightTransform = m_pScene->GetLightCameraTr();
				Vector3 LightPos = Vector3{ pPacket->x, pPacket->y, pPacket->z } +Vector3{ -15, 15, -15 };
				pLightTransform->SetWorldPos(LightPos);
				CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
				pAnimation->ChangeClip("Run");

				SAFE_RELEASE(pAnimation);
				SAFE_RELEASE(pTransform);
				SAFE_RELEASE(pGameObject);
			}
			else if (id < NPC_START)
			{
				char str[128];
				string appendTag = _itoa(id, str, 10);
				string objectTag = "Player" + appendTag;

				CGameObject* pGameObject = CGameObject::FindObject(objectTag);
				CTransform* pTransform = pGameObject->GetTransform();
				float yPos = GET_SINGLE(CQuadTreeManager)->GetY(Vector3(pPacket->x, pPacket->y, pPacket->z));
				pTransform->SetWorldPos(pPacket->x, yPos, pPacket->z);
				CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
				pAnimation->ChangeClip("Run");
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

				if (nullptr != pGameObject)
				{
					CTransform* pTransform = pGameObject->GetTransform();
					float yPos = GET_SINGLE(CQuadTreeManager)->GetY(Vector3(pPacket->x, pPacket->y, pPacket->z));
					pTransform->SetWorldPos(pPacket->x, yPos, pPacket->z);
					CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
					pAnimation->ChangeClip("Walk");
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
			string appendTag = _itoa(id, str, 10);
			string objectTag = "Player" + appendTag;

			if (id == NetworkManager::getInstance()->getMyClientID())
			{
				CGameObject* pGameObject = CGameObject::FindObject(objectTag);
				if (nullptr != pGameObject)
				{
					CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
					pAnimation->ChangeClip("Attack");
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
			}
			else if (id < NPC_START)
			{
			}
			else
			{
				CGameObject* pGameObject = CGameObject::FindObject(objectTag);
				if (nullptr != pGameObject)
				{
					CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
					pAnimation->ChangeClip("Attack");
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
		break;
		case SC_PACKET_ATTACK_SWORD_SKILL1:
		{
			char str[128];
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
					string appendTag = _itoa(target, str, 10);
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
			}
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
				string appendTag = _itoa(myClientID, str, 10);
				string objectTag = "Player" + appendTag;

				CGameObject* pGameObject = CGameObject::FindObject(objectTag);
				CTransform* pTransform = pGameObject->GetTransform();

				if (SC_PACKET_ROTATE_X == packet_type)
				{
					pTransform->SetWorldRotX(pPacket->angle);
				}
				else if (SC_PACKET_ROTATE_Y == packet_type)
				{
					pTransform->SetWorldRotY(pPacket->angle);
				}
				else if (SC_PACKET_ROTATE_Z == packet_type)
				{
					pTransform->SetWorldRotZ(pPacket->angle);
				}

				CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
				pAnimation->ChangeClip("MOVE");
				SAFE_RELEASE(pAnimation);
				SAFE_RELEASE(pTransform);
				SAFE_RELEASE(pGameObject);

			}
			else if (id < NPC_START)
			{
				char str[128];
				string appendTag = _itoa(id, str, 10);
				string objectTag = "Player" + appendTag;

				CGameObject* pGameObject = CGameObject::FindObject(objectTag);
				CTransform* pTransform = pGameObject->GetTransform();

				if (SC_PACKET_ROTATE_X == packet_type)
				{
					pTransform->SetWorldRotX(pPacket->angle);
				}
				else if (SC_PACKET_ROTATE_Y == packet_type)
				{
					pTransform->SetWorldRotY(pPacket->angle);
				}
				else if (SC_PACKET_ROTATE_Z == packet_type)
				{
					pTransform->SetWorldRotZ(pPacket->angle);
				}
				CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
				pAnimation->ChangeClip("MOVE");

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
						pTransform->SetWorldRotX(pPacket->angle);
					}
					else if (SC_PACKET_ROTATE_Y == packet_type)
					{
						pTransform->SetWorldRotY(pPacket->angle);
					}
					else if (SC_PACKET_ROTATE_Z == packet_type)
					{
						pTransform->SetWorldRotZ(pPacket->angle);
					}
					CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
					pAnimation->ChangeClip("Walk");

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
				pAnimation->ChangeClip("Walk");

				SAFE_RELEASE(pAnimation);
				SAFE_RELEASE(pTransform);
				SAFE_RELEASE(pGameObject);
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

		default:
			break;
		}
		NetworkManager::getInstance()->popData();
		free(packet);
	}

	return 0;
}

void CMainScene::Input(float fTime)
{
	if (KEYDOWN("QUICKSLOT-Q"))
	{
		CGameObject* pGameObject = CGameObject::FindObject("Player0");
		CPlayer* pPlayerComponent = pGameObject->FindComponentFromTag<CPlayer>("Player");
		if (pPlayerComponent->clickedID != -1)
		{
			cs_packet_attack_player* pPacket = reinterpret_cast<cs_packet_attack_player*>(NetworkManager::getInstance()->getSendBuffer());
			pPacket->size = sizeof(cs_packet_attack_player);
			NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_attack_player);
			DWORD iobyte;

			pPacket->id = NetworkManager::getInstance()->getMyClientID();
			pPacket->targetid = pPlayerComponent->clickedID;
			pPacket->type = CS_PACKET_ATTACK_SWORD_SKILL1;
			int ret = WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);

			if (ret)
			{

			}
		}
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

			while (chat_message.length() < 50)
			{
				chat_message += L" ";
			}
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
		CGameObject* pLandScapeObj = CGameObject::FindObject("LandScape_Stage1");
		if (pLandScapeObj != NULL)
		{
			CLandScape* pLandScape = pLandScapeObj->FindComponentFromTag<CLandScape>("LandScape");
			CPicking* pPicking = pLandScapeObj->FindComponentFromTag<CPicking>("Picking");

			list<QUADTREENODE*>* pNodes = pLandScape->GetAllNodes();

			bool bFirstCheck = false;

			if (!pNodes->empty())
			{
				for (const auto iter : *pNodes)
				{
					Vector3 pos;
					if (pPicking->Picking_ToBuffer(&pos,
						GET_SINGLE(CInput)->GetRayPos(),
						GET_SINGLE(CInput)->GetRayDir(),
						iter->vecVtx, iter->vecIndex))
					{
						char str[128];
						string appendTag = _itoa(NetworkManager::getInstance()->getMyClientID(), str, 10);
						string objectTag = "Player" + appendTag;
						CGameObject* pGameObject = CGameObject::FindObject(objectTag);
						CPlayer* pPlayer = pGameObject->FindComponentFromTag<CPlayer>("Player");
						pPlayer->clickedID = -1;

						CGameObject* pUIObject = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar()->GetGameObject();
						pUIObject->Enable(false);
						SAFE_RELEASE(pUIObject);
					}
				}
			}

			SAFE_RELEASE(pPicking);
			SAFE_RELEASE(pLandScape);
			SAFE_RELEASE(pLandScapeObj);
		}
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

int CMainScene::LateUpdate(float fTime)
{
	GET_SINGLE(UserInterfaceManager)->update(fTime);
	return 0;
}

void CMainScene::Render(float fTime)
{
}

#include "DungeonScene.h"
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
#include "../ObjectScript/GreenLizard.h"
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
#include "../ObjectScript/DemonLord.h"
#include "../ObjectScript/GreenLizard.h"
#include "../ObjectScript/BlueLizard.h"
#include "../ObjectScript/Armored_GreenLizard.h"
#include "../ObjectScript/Armored_BlueLizard.h"

DungeonScene::DungeonScene()
{
}

DungeonScene::~DungeonScene()
{

}

void DungeonScene::chat_callback(float fTime)
{
	//this->chat_write = true;
}

bool DungeonScene::Init()
{
	///* Effect */
	//GET_SINGLE(CEffectManager)->AddEffect("Attack", "Effect\\Attack.bin");
	//GET_SINGLE(CEffectManager)->AddEffect("Attack2", "Effect\\Attack2.bin");
	//GET_SINGLE(CEffectManager)->AddEffect("Attack3", "Effect\\Attack3.bin");
	////GET_SINGLE(CEffectManager)->AddEffect("Spell", "Effect\\Spell.bin");
	//GET_SINGLE(CEffectManager)->AddEffect("Spell1", "Effect\\Spell1.bin");
	//GET_SINGLE(CEffectManager)->AddEffect("Spell2", "Effect\\Spell2.bin");
	//GET_SINGLE(CEffectManager)->AddEffect("Spell3", "Effect\\Spell3.bin");
	//GET_SINGLE(CEffectManager)->AddEffect("Spell4", "Effect\\Spell4.bin");
	//GET_SINGLE(CEffectManager)->AddEffect("Level_Up", "Effect\\Level_Up.bin");
#pragma region Portal
	GET_SINGLE(CEffectManager)->OperateEffect("Portal", nullptr, Vector3(78.f, 0.f, 95.f));
#pragma endregion
	const auto& objlist = CGameObject::getObjectList();
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


	//#pragma endregion
	GET_SINGLE(UserInterfaceManager)->initialize();

	//#pragma region KeySetting
	//	GET_SINGLE(CInput)->CreateKey("Attack", 'X');
	//	GET_SINGLE(CInput)->CreateKey("Skill1", 'Q');
	//	GET_SINGLE(CInput)->CreateKey("Skill2", 'E');
	//	GET_SINGLE(CInput)->CreateKey("Skill3", 'R');
	//	GET_SINGLE(CInput)->CreateKey("Skill4", 'T');
	//	GET_SINGLE(CInput)->CreateKey("INVENTORY", 'I');
	//#pragma endregion

#pragma region Terrain
	{
		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer* pLayer = pScene->GetLayer("Default");

		// Load Terrain
		CGameObject* pLandScapeObj = CGameObject::CreateObject("LandScape_Stage1", pLayer);
		CLandScape* pLandScape = pLandScapeObj->AddComponent<CLandScape>("LandScape");

		pLandScape->Load_Terrain("dungeon_scene");

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
	wcscat_s(strPath, MAX_PATH, L"Object\\Main_Scene_1.bin");

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
			pRenderer->SetMesh("Player", L"99.Dynamic_Mesh\\00.Player\\Tanker\\Tanker.msh");

			string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);
			string transformPath = meshBasePath + "99.Dynamic_Mesh\\00.Player\\Tanker\\Tanker.dat";
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

			pRenderer->SetMesh("Golem", L"99.Dynamic_Mesh\\02.Monster\\Golem\\Golem.msh");

			string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);

			string transformPath = meshBasePath + "99.Dynamic_Mesh\\02.Monster\\Golem\\Golem.dat";

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

			string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);

			string transformPath = meshBasePath + "99.Dynamic_Mesh\\02.Monster\\Mino\\Mino.dat";

			FILE* pFile_Player = nullptr;

			fopen_s(&pFile_Player, transformPath.c_str(), "rb");

			if (!pFile_Player)
				return false;

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
			CLayer*	pLayer = m_pScene->GetLayer("Default");
			CGameObject*	pGreenLizardObj = CGameObject::CreatePrototypeDontDestroy("GreenLizard", m_pScene);
			CTransform*	pTr = pGreenLizardObj->GetTransform();

			CRenderer*	pRenderer = pGreenLizardObj->AddComponent<CRenderer>("PlayerRenderer");

			pRenderer->SetMesh("greenlizard", L"99.Dynamic_Mesh\\02.Monster\\GreenLizard\\GreenLizard.msh");

			string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);

			string transformPath = meshBasePath + "99.Dynamic_Mesh\\02.Monster\\GreenLizard\\GreenLizard.dat";

			FILE* pFile_Player = nullptr;

			fopen_s(&pFile_Player, transformPath.c_str(), "rb");

			if (!pFile_Player)
				return false;

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
			CLayer*	pLayer = m_pScene->GetLayer("Default");
			CGameObject*	pBlueLizardObj = CGameObject::CreatePrototypeDontDestroy("BlueLizard", m_pScene);
			CTransform*	pTr = pBlueLizardObj->GetTransform();

			CRenderer*	pRenderer = pBlueLizardObj->AddComponent<CRenderer>("PlayerRenderer");

			pRenderer->SetMesh("bluelizard", L"99.Dynamic_Mesh\\02.Monster\\BlueLizard\\BlueLizard.msh");

			string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);

			string transformPath = meshBasePath + "99.Dynamic_Mesh\\02.Monster\\BlueLizard\\BlueLizard.dat";

			FILE* pFile_Player = nullptr;

			fopen_s(&pFile_Player, transformPath.c_str(), "rb");

			if (!pFile_Player)
				return false;

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
			CLayer*	pLayer = m_pScene->GetLayer("Default");
			CGameObject*	pArmoredGreenLizard = CGameObject::CreatePrototypeDontDestroy("Armored_GreenLizard", m_pScene);
			CTransform*	pTr = pArmoredGreenLizard->GetTransform();

			CRenderer*	pRenderer = pArmoredGreenLizard->AddComponent<CRenderer>("PlayerRenderer");

			pRenderer->SetMesh("Armored_GreenLizard", L"99.Dynamic_Mesh\\02.Monster\\Armored_GreenLizard\\Armored_GreenLizard.msh");

			string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);

			string transformPath = meshBasePath + "99.Dynamic_Mesh\\02.Monster\\Armored_GreenLizard\\Armored_GreenLizard.dat";

			FILE* pFile_Player = nullptr;

			fopen_s(&pFile_Player, transformPath.c_str(), "rb");

			if (!pFile_Player)
				return false;

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
			CLayer*	pLayer = m_pScene->GetLayer("Default");
			CGameObject*	pArmored_BlueLizardObj = CGameObject::CreatePrototypeDontDestroy("Armored_BlueLizard", m_pScene);
			CTransform*	pTr = pArmored_BlueLizardObj->GetTransform();

			CRenderer*	pRenderer = pArmored_BlueLizardObj->AddComponent<CRenderer>("PlayerRenderer");

			pRenderer->SetMesh("Armored_BlueLizard", L"99.Dynamic_Mesh\\02.Monster\\Armored_BlueLizard\\Armored_BlueLizard.msh");

			string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);

			string transformPath = meshBasePath + "99.Dynamic_Mesh\\02.Monster\\Armored_BlueLizard\\Armored_BlueLizard.dat";

			FILE* pFile_Player = nullptr;

			fopen_s(&pFile_Player, transformPath.c_str(), "rb");

			if (!pFile_Player)
				return false;

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
			CLayer*	pLayer = m_pScene->GetLayer("Default");
			CGameObject*	pSeuteompiObj = CGameObject::CreatePrototypeDontDestroy("DemonLordObjeect", m_pScene);
			CTransform*	pTr = pSeuteompiObj->GetTransform();

			CRenderer*	pRenderer = pSeuteompiObj->AddComponent<CRenderer>("DemonRnederer");

			pRenderer->SetMesh("DemonLord", L"99.Dynamic_Mesh\\02.Monster\\DemonLord\\DemonLord.msh");

			string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);

			string transformPath = meshBasePath + "99.Dynamic_Mesh\\02.Monster\\DemonLord\\DemonLord.dat";

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
#pragma endregion


		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
	}
#pragma region billboard
	{
		default_random_engine dre;
		uniform_int_distribution<> ui;

		CLayer*    pParticleLayer = m_pScene->GetLayer("ParticleLayer");

		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				string number_tag = to_string(i);
				string object_tag = "ParticleObj" + number_tag;
				CGameObject*    pParticleObj = CGameObject::CreateObject(object_tag, pParticleLayer);

				CTransform*    pParticleTr = pParticleObj->GetTransform();
				pParticleTr->SetWorldPos(300.f + (0.2 * i), 2.f, 300.f + (0.2*j));
				SAFE_RELEASE(pParticleTr);

				CParticleSingle*    pParticleSingle = pParticleObj->AddComponent<CParticleSingle>("ParticleSingle");
				pParticleSingle->SetSize(10.f, 10.f);
				SAFE_RELEASE(pParticleSingle);

				CRenderer*    pParticleRenderer = pParticleObj->FindComponentFromType<CRenderer>(CT_RENDERER);
				pParticleRenderer->CreateCBuffer("Animation2D", 10, sizeof(ANIMATION2DBUFFER),
					SCT_VERTEX | SCT_PIXEL);
				pParticleRenderer->SetRenderState(ALPHA_BLEND);
				CAnimation2D*    pParticleAnimation = pParticleObj->AddComponent<CAnimation2D>("ParticleAnimation");
				pParticleAnimation->SetRenderer2DEnable(false);

				vector<wstring>    vecExplosion;
				for (int i = 1; i <= 1; ++i)
				{
					wchar_t    strPath[MAX_PATH] = {};
					wsprintf(strPath, L"Billboard/test.dds", i);

					vecExplosion.push_back(strPath);
				}

				pParticleAnimation->CreateClip("Explosion", A2D_FRAME, A2DO_LOOP,
					1, 1, 1, 1, 0, 0.5f, 0, 0.f, "Explosion", &vecExplosion);

				SAFE_RELEASE(pParticleAnimation);
				SAFE_RELEASE(pParticleRenderer);
				SAFE_RELEASE(pParticleObj);
			}
		}
		SAFE_RELEASE(pParticleLayer);
	}
#pragma endregion

	// mainserver로의 접속요청을 한다.
	{
		cs_packet_connect* pPacket = reinterpret_cast<cs_packet_connect*>(NetworkManager::getInstance()->getSendBuffer());
		pPacket->size = sizeof(cs_packet_connect);
		pPacket->type = CS_PACKET_MAINSERVER_CONNECT;

		NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_connect);
		DWORD iobyte;
		int ret = WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);
	}
	NetworkManager::getInstance()->inputTime = high_resolution_clock::now();
	//
	//#pragma region sound
	//	GET_SINGLE(SoundManager)->LoadSound("MainSceneBGM", true, "WoodlandFantasy.mp3");
	//
	//	GET_SINGLE(SoundManager)->Play("MainSceneBGM", SC_BGM);
	//#pragma endregion

	GET_SINGLE(CNaviManager)->CreateNaviMesh("Main_Scene_1");
	GET_SINGLE(CNaviManager)->SetRenderCheck(false);
	isInitComplete = true;
	return true;
}


void DungeonScene::Input(float fTime)
{
	if (isInitComplete == true)
	{
		int checkID = NetworkManager::getInstance()->getMyClientID();
		string checkAppendTag = to_string(checkID);
		string checkObjectTag = "Player" + checkAppendTag;
		CGameObject* pMyGameObject = CGameObject::FindObject(checkObjectTag);
		if (pMyGameObject != nullptr)
		{
			if (KEYDOWN("Skill1"))
			{
				char str[128];
				int id = NetworkManager::getInstance()->getMyClientID();
				string appendTag = _itoa(id, str, 10);
				string objectTag = "Player" + appendTag;
				CGameObject* pGameObject = CGameObject::FindObject(objectTag);
				CPlayer* pPlayerComponent = pGameObject->FindComponentFromTypeName<CPlayer>("Actor");
				if (pPlayerComponent->clickedID != -1)
				{
					cs_packet_attack_player* pPacket = reinterpret_cast<cs_packet_attack_player*>(NetworkManager::getInstance()->getSendBuffer());
					pPacket->size = sizeof(cs_packet_attack_player);
					NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_attack_player);
					DWORD iobyte;

					pPacket->id = NetworkManager::getInstance()->getMyClientID();
					pPacket->targetid = pPlayerComponent->clickedID;
					pPacket->type = CS_PACKET_ATTACK_SKILL1;
					int ret = WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);

					if (ret)
					{

					}
				}
			}

			if (KEYDOWN("Skill2"))
			{
				char str[128];
				int id = NetworkManager::getInstance()->getMyClientID();
				string appendTag = _itoa(id, str, 10);
				string objectTag = "Player" + appendTag;
				CGameObject* pGameObject = CGameObject::FindObject(objectTag);
				CPlayer* pPlayerComponent = pGameObject->FindComponentFromTypeName<CPlayer>("Actor");
				if (pPlayerComponent->clickedID != -1)
				{
					cs_packet_attack_player* pPacket = reinterpret_cast<cs_packet_attack_player*>(NetworkManager::getInstance()->getSendBuffer());
					pPacket->size = sizeof(cs_packet_attack_player);
					NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_attack_player);
					DWORD iobyte;

					pPacket->id = NetworkManager::getInstance()->getMyClientID();
					pPacket->targetid = pPlayerComponent->clickedID;
					pPacket->type = CS_PACKET_ATTACK_SKILL2;
					int ret = WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);

					if (ret)
					{

					}
				}
			}
			if (KEYDOWN("Skill3"))
			{
				char str[128];
				int id = NetworkManager::getInstance()->getMyClientID();
				string appendTag = _itoa(id, str, 10);
				string objectTag = "Player" + appendTag;
				CGameObject* pGameObject = CGameObject::FindObject(objectTag);
				CPlayer* pPlayerComponent = pGameObject->FindComponentFromTypeName<CPlayer>("Actor");
				if (pPlayerComponent->clickedID != -1)
				{
					cs_packet_attack_player* pPacket = reinterpret_cast<cs_packet_attack_player*>(NetworkManager::getInstance()->getSendBuffer());
					pPacket->size = sizeof(cs_packet_attack_player);
					NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_attack_player);
					DWORD iobyte;

					pPacket->id = NetworkManager::getInstance()->getMyClientID();
					pPacket->targetid = pPlayerComponent->clickedID;
					pPacket->type = CS_PACKET_ATTACK_SKILL3;
					int ret = WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);

					if (ret)
					{

					}
				}
			}
			if (KEYDOWN("Skill4"))
			{
				char str[128];
				int id = NetworkManager::getInstance()->getMyClientID();
				string appendTag = _itoa(id, str, 10);
				string objectTag = "Player" + appendTag;
				CGameObject* pGameObject = CGameObject::FindObject(objectTag);
				CPlayer* pPlayerComponent = pGameObject->FindComponentFromTypeName<CPlayer>("Actor");

				cs_packet_attack_player* pPacket = reinterpret_cast<cs_packet_attack_player*>(NetworkManager::getInstance()->getSendBuffer());
				pPacket->size = sizeof(cs_packet_attack_player);
				NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_attack_player);
				DWORD iobyte;

				pPacket->id = NetworkManager::getInstance()->getMyClientID();
				pPacket->targetid = pPlayerComponent->clickedID;
				pPacket->type = CS_PACKET_ATTACK_SKILL4;
				int ret = WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);

				if (ret)
				{

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

				CTransform* pTransform = NetworkManager::getInstance()->pPlayer->GetTransform();
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
				CAnimation* pAnimation = NetworkManager::getInstance()->pPlayer->FindComponentFromType<CAnimation>(CT_ANIMATION);
				pAnimation->ReturnDefaultClip();
			}

			if (KEYPUSH("MoveBack"))
			{

				CTransform* pTransform = NetworkManager::getInstance()->pPlayer->GetTransform();

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
				{
					GET_SINGLE(UserInterfaceManager)->getEnemyStatus()->enableRender(true);
				}
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
					GET_SINGLE(UserInterfaceManager)->getDropTableUI()->clear();
				}

				SAFE_RELEASE(pRay);
				SAFE_RELEASE(pMouseObj);
			}

			if (KEYDOWN("ESC"))
			{
				GET_SINGLE(UserInterfaceManager)->getDropTableUI()->enableRender(false);
				GET_SINGLE(UserInterfaceManager)->getTargetPlayerUI()->enableRender(false);
				GET_SINGLE(UserInterfaceManager)->getInventory()->enableRender(false);
				GET_SINGLE(UserInterfaceManager)->getPGMessageBox()->enableRender(false);
				GET_SINGLE(UserInterfaceManager)->getDropTableUI()->clear();
			}

			if (KEYDOWN("Attack"))
			{
				char str[128];
				int id = NetworkManager::getInstance()->getMyClientID();
				string appendTag = _itoa(id, str, 10);
				string objectTag = "Player" + appendTag;
				CGameObject* pGameObject = CGameObject::FindObject(objectTag);
				CPlayer* pPlayerComponent = pGameObject->FindComponentFromTypeName<CPlayer>("Actor");
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


			/* For Rendering Navi, QuadTree, etc */
			if (KEYDOWN("F1"))
			{
				CGameObject* pLandScapeObj = CGameObject::FindObject("LandScape_Stage1");

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
	}
}


int DungeonScene::Update(float fTime)
{

#pragma region Chatting
	{
		Chatting* pChatting = GET_SINGLE(UserInterfaceManager)->getChatting();
		wstring edit_view = L"";
		for (auto input : pChatting->getChatCont())
		{
			//wchar_t temp[64];
			wchar_t temp2 = input;
			wstring appendString = L"";
			appendString = appendString + temp2;
			edit_view = edit_view + appendString;
		}
		pChatting->setChatString(edit_view);
		pChatting->getUIChatText()->SetText(edit_view);
	}
#pragma endregion

	return 0;
}

int DungeonScene::LateUpdate(float fTime)
{
	GET_SINGLE(UserInterfaceManager)->update(fTime);
	return 0;
}

void DungeonScene::Render(float fTime)
{
}

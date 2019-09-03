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
#include "../MFObjectManager.h"
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

	//m_pScene->LoadSky(L"Skybox_2");
	//m_pScene->LoadGlobLight("Night_Test");
	//m_pScene->LoadPointLight("Night_Test");
#pragma region Layer Setting
	m_pScene->CreateLayer("UI+1", UI_LAYER + 1);
	m_pScene->CreateLayer("UI+2", UI_LAYER + 2);
	m_pScene->CreateLayer("UI+3", UI_LAYER + 3);
	m_pScene->CreateLayer("ParticleLayer", 2000);
	m_pScene->CreateLayer("UI_QUEST+1", UI_LAYER + 11);
	m_pScene->CreateLayer("UI_QUEST+2", UI_LAYER + 12);
	m_pScene->CreateLayer("UI_QUEST+3", UI_LAYER + 13);
#pragma endregion

	GET_SINGLE(UserInterfaceManager)->initialize();
#pragma region Terrain
	{
		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer* pLayer = pScene->GetLayer("Default");

		// Load Terrain
		CGameObject* pLandScapeObj = CGameObject::CreateObject("LandScape_Dungeon", pLayer);
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
	MFObjectManager::getInstance()->initialize(L"dungeon_scene");

	NetworkManager::getInstance()->connectMainServer();
	NetworkManager::getInstance()->inputTime = high_resolution_clock::now();

#pragma region sound
	GET_SINGLE(SoundManager)->LoadSound("MainSceneBGM", true, "WoodlandFantasy.mp3");
	GET_SINGLE(SoundManager)->LoadSound("SwordAttack", false, "SwordAttack.wav");
	GET_SINGLE(SoundManager)->LoadSound("SwordAttack1", false, "SwordAttack1.wav");
	GET_SINGLE(SoundManager)->LoadSound("SwordAttack2", false, "SwordAttack2.mp3");
	GET_SINGLE(SoundManager)->LoadSound("SwordAttack3", false, "SwordAttack3.wav");
	GET_SINGLE(SoundManager)->LoadSound("monster_attack", false, "monster_attack.ogg");
	GET_SINGLE(SoundManager)->LoadSound("monster_damaged", false, "monster_damaged.flac");
	GET_SINGLE(SoundManager)->LoadSound("monster_death", false, "monster_death.flac");
	GET_SINGLE(SoundManager)->LoadSound("firecircle_first", false, "firecircle_first.aif");
	GET_SINGLE(SoundManager)->LoadSound("firecircle_second", false, "firecircle_second.aif");
	GET_SINGLE(SoundManager)->Play("MainSceneBGM", SC_BGM);
#pragma endregion

	GET_SINGLE(CEffectManager)->AddEffect("Fire_Tall_Dark", "Effect\\Common\\Fire_tall_dark.bin");
	GET_SINGLE(CEffectManager)->OperateEffect("Fire_Tall_Dark", nullptr, Vector3(33.8f, 14.9f, 19.5f));
	GET_SINGLE(CEffectManager)->OperateEffect("Fire_Tall_Dark", nullptr, Vector3(14.75f, 14.9f, 18.49f));
	GET_SINGLE(CEffectManager)->OperateEffect("Fire_Tall_Dark", nullptr, Vector3(34.22f, 14.9f, 74.74f));
	GET_SINGLE(CEffectManager)->OperateEffect("Fire_Tall_Dark", nullptr, Vector3(15.82f, 14.9f, 76.95f));
	GET_SINGLE(CEffectManager)->OperateEffect("Fire_Tall_Dark", nullptr, Vector3(46.81f, 14.9f, 136.96f));
	GET_SINGLE(CEffectManager)->OperateEffect("Fire_Tall_Dark", nullptr, Vector3(47.75f, 14.9f, 110.16f));
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

	m_pScene->LoadSky(L"Skybox_2");
	m_pScene->LoadGlobLight("dungeon_scene", Vector4(0.2f, 0.2f, 0.2f, 1.f));
	m_pScene->LoadPointLight("dungeon_scene", Vector4(0.2f, 0.2f, 0.2f, 1.f));

	CLayer* pLayer2 = m_pScene->CreateLayer("RayCollide");
	CGameObject::LoadEnvObjects(L"dungeon_scene_collision", pLayer2, false);
	GET_SINGLE(CNaviManager)->CreateNaviMeshFromFile("dungeon_scene");
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

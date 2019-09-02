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
#include "../QuestManager.h"

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

	//m_pScene->LoadSky(L"Skybox_2");
	//m_pScene->LoadGlobLight("Night_Test");
	//m_pScene->LoadPointLight("Night_Test");

#pragma region Effect Setting
	GET_SINGLE(CEffectManager)->AddEffect("Attack", "Effect\\Attack.bin");
	GET_SINGLE(CEffectManager)->AddEffect("Attack2", "Effect\\Attack2.bin");
	GET_SINGLE(CEffectManager)->AddEffect("Attack3", "Effect\\Attack3.bin");
	GET_SINGLE(CEffectManager)->AddEffect("Spell1", "Effect\\Player\\Tanker\\Spell_Attack_RollingBlade.bin");
	GET_SINGLE(CEffectManager)->AddEffect("Spell2", "Effect\\Player\\Tanker\\Spell_Attack_ManySwings.bin");
	GET_SINGLE(CEffectManager)->AddEffect("Spell3", "Effect\\Player\\Tanker\\Spell_Attack_Bomb.bin");
	GET_SINGLE(CEffectManager)->AddEffect("Spell4", "Effect\\Player\\Tanker\\Spell_Buff_FireCircle.bin");
	GET_SINGLE(CEffectManager)->AddEffect("Level_Up", "Effect\\Level_Up.bin");
	GET_SINGLE(CEffectManager)->AddEffect("Portal", "Effect\\Portal.bin");
	GET_SINGLE(CEffectManager)->OperateEffect("Portal", nullptr, Vector3(78.f, 0.f, 95.f));
	GET_SINGLE(CEffectManager)->AddEffect("Holy_Light",
		"Effect\\Common\\Holy_Light.bin");
#pragma endregion

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

#pragma region KeySetting
	GET_SINGLE(CInput)->CreateKey("Attack", 'X');
	GET_SINGLE(CInput)->CreateKey("Skill1", 'Q');
	GET_SINGLE(CInput)->CreateKey("Skill2", 'E');
	GET_SINGLE(CInput)->CreateKey("Skill3", 'R');
	GET_SINGLE(CInput)->CreateKey("Skill4", 'T');
	GET_SINGLE(CInput)->CreateKey("INVENTORY", 'I');
	GET_SINGLE(CInput)->CreateKey("Quest", 'L');
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
	MFObjectManager::getInstance()->initialize();
	

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

	GET_SINGLE(CNaviManager)->CreateNaviMesh("Main_Scene_1");
	GET_SINGLE(CNaviManager)->SetRenderCheck(false);
	isInitComplete = true;

	return true;
}


void CMainScene::Input(float fTime)
{
	move_time += fTime;

	if (move_time >= 0.0333f)
	{
		move_enable = true;
	}

	if(isInitComplete == true)
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
			if (KEYDOWN("Quest"))
			{
				_cprintf("test\n");
				bool isEnable = QuestManager::getInstance()->isShow();
				QuestManager::getInstance()->enableShow(!isEnable);
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

			if (move_enable)
			{
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
				else if (KEYUP("MoveRight"))
				{
					cs_packet_move_stop* pPacket = reinterpret_cast<cs_packet_move_stop*>(NetworkManager::getInstance()->getSendBuffer());

					pPacket->size = sizeof(cs_packet_move_stop);
					NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_move_stop);
					DWORD iobyte;

					pPacket->type = CS_PACKET_MOVE_STOP;

					NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_move_stop);
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
				else if (KEYUP("MoveLeft"))
				{
					cs_packet_move_stop* pPacket = reinterpret_cast<cs_packet_move_stop*>(NetworkManager::getInstance()->getSendBuffer());

					pPacket->size = sizeof(cs_packet_move_stop);
					NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_move_stop);
					DWORD iobyte;

					pPacket->type = CS_PACKET_MOVE_STOP;

					NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_move_stop);
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
				else if (KEYUP("MoveFront"))
				{
					cs_packet_move_stop* pPacket = reinterpret_cast<cs_packet_move_stop*>(NetworkManager::getInstance()->getSendBuffer());

					pPacket->size = sizeof(cs_packet_move_stop);
					NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_move_stop);
					DWORD iobyte;

					pPacket->type = CS_PACKET_MOVE_STOP;

					NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_move_stop);
					int ret = WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);
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
				else if (KEYUP("MoveBack"))
				{
					cs_packet_move_stop* pPacket = reinterpret_cast<cs_packet_move_stop*>(NetworkManager::getInstance()->getSendBuffer());

					pPacket->size = sizeof(cs_packet_move_stop);
					NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_move_stop);
					DWORD iobyte;

					pPacket->type = CS_PACKET_MOVE_STOP;

					NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_move_stop);
					int ret = WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);
				}

				move_enable = false;
			}

			if (KEYDOWN("MouseLButton"))
			{
				CGameObject *pMouseObj = GET_SINGLE(CInput)->GetMouseObj();
				CColliderRay *pRay = pMouseObj->FindComponentFromTag<CColliderRay>("MouseRay");
				bool clickedEnemy = false;
				for (const auto& object : CGameObject::getObjectList())
				{
					CColliderSphere *pColl = object->FindComponentFromType<CColliderSphere>(CT_COLLIDER);

					if (pColl == nullptr)
						continue;

					CGameObject* pObject = pColl->GetGameObject();
					string object_tag = "Player" + to_string(NetworkManager::getInstance()->getMyClientID());
					if (pObject->GetTag() == object_tag)
					{
						clickedEnemy = false;
						break;
					}
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


int CMainScene::Update(float fTime)
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

int CMainScene::LateUpdate(float fTime)
{
	GET_SINGLE(UserInterfaceManager)->update(fTime);
	return 0;
}

void CMainScene::Render(float fTime)
{
}

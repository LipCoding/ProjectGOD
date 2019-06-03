#include "NetworkManager.h"
#include "Core.h"
#include "Core/PathManager.h"
#include "Scene/Layer.h"
#include "Core/Input.h"
#include "Device.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/Renderer.h"
#include "Component/Transform.h"
#include "ObjectScript/Player.h"
#include "ObjectScript/Golem.h"
#include "ObjectScript/Effect.h"
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
#include "ObjectScript/CharacterPanel.h"
#include "ObjectScript/Gun.h"
#include "Component/Decal.h"
#include "Core/NavigationManager.h"
#include "Core/NavigationMesh.h"
#include "Resources/Mesh.h"
#include "NetworkManager.h"
#include "Core.h"
#include "Device.h"
#include "Component/Renderer2D.h"
#include "Component/ColliderRect.h"
#include "Core/PathManager.h"
#include "Core/QuadTreeManager.h"
#include "Core/SoundManager.h"
#include "Client.h"
#include "UserInterfaceManager.h"
#include "Chatting.h"
#include "Item.h"
#include "Inventory.h"
#include "ObjectScript/Mino.h"
#include "ObjectScript/GreenLizard.h"
#include "Component/Picking.h"
#include "Component/ParticleMultiple.h"
#include "Core/QuadTreeManager.h"
#include "Core/NaviManager.h"
#include "Component/Cell.h"
#include "Component/ColliderRay.h"
#include "DropTableUI.h"
#include <random>
#include "Component/UIButton.h"
#include "PGMessageBox.h"
#include "TargetPlayerUI.h"
#include "Core/EffectManager.h"
#include "ObjectScript/Sword.h"
#include "Component/ColliderAABB.h"
#include "ObjectScript/DemonLord.h"
#include "ObjectScript/GreenLizard.h"
#include "ObjectScript/BlueLizard.h"
#include "ObjectScript/Armored_GreenLizard.h"
#include "ObjectScript/Armored_BlueLizard.h"
#include "PartyStatus.h"
#include "SceneScript/SecondScene.h"
#include "SceneScript/DungeonScene.h"
queue<void*> NetworkManager::clientPacketQueue;
CGameObject* NetworkManager::pPlayer;
NetworkManager::NetworkManager() :
	sock{ 0 },
	inPacketSize{ 0 },
	savedPacketSize{ 0 },
	myClientID{ 0 },
	isConnected{ false },
	pRecvThread{ nullptr }
{
}

void * NetworkManager::fetchToClientPacket()
{
	void* packet = NULL;

	if (false == clientPacketQueue.empty())
		packet = clientPacketQueue.front();


	return packet;
}

NetworkManager::~NetworkManager()
{
}

bool NetworkManager::initialize()
{
	// 파일로부터 ip주소를 읽어들인다.
	//

	string path = GET_SINGLE(CPathManager)->FindPathToMultiByte(DATA_PATH);
	path += "serverip.txt";
	ifstream in(path.c_str());
	in >> serverip_address;



	inPacketSize = 0;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

	if (sock == INVALID_SOCKET)
	{
		//에러처리.
	}

	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(serverip_address.c_str());
	serverAddr.sin_port = htons(SERVER_PORT);

	int retval = WSAConnect(sock, (sockaddr *)&serverAddr, sizeof(serverAddr), NULL, NULL, NULL, NULL);

	WSAAsyncSelect(sock, GET_SINGLE(CCore)->GetWindowHandle(), WM_SOCKET, FD_CLOSE | FD_READ);

	send_wsabuf.buf = sendBuffer;
	send_wsabuf.len = MAX_BUFF_SIZE;
	recv_wsabuf.buf = recvBuffer;
	recv_wsabuf.len = MAX_BUFF_SIZE;

	if (retval == SOCKET_ERROR)
	{
		MessageBox(NULL, L"서버와의 연결에 실패하였습니다.", L"서버에러", MB_OK);
	}

	//pRecvThread = new thread{ recvThread };

	return true;
}

void NetworkManager::shutDownServer()
{
	//pRecvThread->join();
	//delete pRecvThread;
	closesocket(sock);
	WSACleanup();
}

void NetworkManager::processPacket(char * ptr)
{
	//void* packet = (void*)malloc(sizeof(char) * ((unsigned short*)ptr)[0]);
	//memcpy(packet, ptr, ((unsigned short*)ptr)[0]);

	//clientPacketQueue.emplace(move(packet));
	void* packet = (void*)malloc(sizeof(char) * ((unsigned short*)ptr)[0]);
	memcpy(packet, ptr, ((unsigned short*)ptr)[0]);

	//while (false == NetworkManager::getInstance()->getClientPacketQueue().empty())
	{
		//void* packet = NetworkManager::getInstance()->fetchToClientPacket();

		if (nullptr == packet)
			return;

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

				CLayer*	pLayer = GET_SINGLE(CSceneManager)->GetCurrentScene()->GetLayer("Default");

				CGameObject* pPlayerObj = CGameObject::FindObject(objectTag);
				if (nullptr == pPlayerObj)
				{
					pPlayerObj = CGameObject::CreateClone("PlayerCharacter", pLayer);
					this->pPlayer = pPlayerObj;

					{
						CColliderSphere* pCollider = pPlayerObj->AddComponent<CColliderSphere>("collider");
						pCollider->SetSphere(Vector3(0.f, 1.f, 0.f), 1.f);
						SAFE_RELEASE(pCollider);
					}
					pPlayerObj->SetTag(objectTag);
					CTransform*	pTr = pPlayerObj->GetTransform();
					float yPos = GET_SINGLE(CQuadTreeManager)->GetY(Vector3(pPacket->x, pPacket->y, pPacket->z));
					pTr->SetWorldPos(pPacket->x, yPos, pPacket->z);
					//pTr->SetWorldScale(2.f, 2.f, 2.f);
					pTr->SetWorldRotY(XMConvertToRadians(pPacket->y));
					SAFE_RELEASE(pTr);

					CPlayer*	pPlayer = pPlayerObj->AddComponent<CPlayer>("Player");
					pPlayer->setCurrentHP(pPacket->current_hp);
					pPlayer->setCurrentMP(pPacket->current_mp);
					pPlayer->setLevel(pPacket->level);
					pPlayer->setMaxHP(200 + pPacket->level * 50);
					pPlayer->setMaxMP(30 + pPacket->level * 10);
					pPlayer->setEXP(pPacket->exp);
					pPlayer->setMaxEXP(pPacket->level * 100);
					pPlayer->setAttackDamag(10 + pPacket->level * 5);

					Status* pStatus = GET_SINGLE(UserInterfaceManager)->getStatus();

					float ratio = (float)(pPlayer->getEXP()) / (float)(pPlayer->getMaxEXP());
					//pEnemyUIHearthBar->

					pStatus->getUIPureBar()->setLengthRatio(ratio);

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

					CGameObject*	pCameraObj = GET_SINGLE(CSceneManager)->GetCurrentScene()->GetMainCameraObj();
					CTransform* pTransform = GET_SINGLE(CSceneManager)->GetCurrentScene()->GetMainCameraTr();
					//CTransform* pLightTransform = GET_SINGLE(CSceneManager)->GetCurrentScene()->GetLightCameraTr();
					////Vector3 LightPos = m_pTransform->GetWorldPos();
					//Vector3 LightPos = Vector3{ pPacket->x, pPacket->y, pPacket->z } +Vector3{ -15, 15, -15 };
					//pLightTransform->SetWorldPos(LightPos);

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

				CLayer*	pLayer = GET_SINGLE(CSceneManager)->GetCurrentScene()->GetLayer("Default");
				CGameObject* pPlayerObj = CGameObject::FindObject(objectTag);
				if (nullptr == pPlayerObj)
				{
					pPlayerObj = CGameObject::CreateClone("PlayerCharacter", pLayer);

					//pPlayerObj->SetTag(objectTag);
					pPlayerObj->SetTag(objectTag);
					{
						CColliderSphere* pCollider = pPlayerObj->AddComponent<CColliderSphere>("collider");
						pCollider->SetSphere(Vector3(0.f, 1.f, 0.f), 1.f);
						SAFE_RELEASE(pCollider);
					}
					CPlayer*	pPlayer = pPlayerObj->AddComponent<CPlayer>("Player");
					pPlayer->setCurrentHP(pPacket->current_hp);
					pPlayer->setCurrentMP(pPacket->current_mp);
					pPlayer->setLevel(pPacket->level);
					pPlayer->setMaxHP(200 + pPacket->level * 50);
					string debugstring = "level : " + to_string(pPacket->level) + " MaxHP : " + to_string(pPlayer->getMaxHP()) + "\n";
					OutputDebugStringA(debugstring.c_str());
					pPlayer->setMaxMP(30 + pPacket->level * 10);
					pPlayer->setMaxEXP(pPacket->level * 100);
					pPlayer->setAttackDamag(10 + pPacket->level * 5);
					pPlayer->setAnimation(pPlayerObj->FindComponentFromType<CAnimation>(CT_ANIMATION));
					SAFE_RELEASE(pPlayer);

					CTransform*	pTr = pPlayerObj->GetTransform();
					CColliderSphere* pCollider = pPlayerObj->AddComponent<CColliderSphere>("collider");
					pCollider->SetSphere(Vector3(0.f, 0.f, 0.f), 2.f);
					SAFE_RELEASE(pCollider);
					float yPos = GET_SINGLE(CQuadTreeManager)->GetY(Vector3(pPacket->x, pPacket->y, pPacket->z));
					pTr->SetWorldPos(pPacket->x, yPos, pPacket->z);
					//pTr->SetWorldScale(2.f, 2.f, 2.f);
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

					CLayer*	pLayer = GET_SINGLE(CSceneManager)->GetCurrentScene()->GetLayer("Default");
					CGameObject* pGolemObj = CGameObject::FindObject(objectTag);
					if (nullptr == pGolemObj)
					{
						CGameObject* pGolemObject = CGameObject::CreateClone("GolemObject", pLayer);
						pGolemObject->SetTag(objectTag);
						{
							CColliderSphere* pCollider = pGolemObject->AddComponent<CColliderSphere>("collider");
							pCollider->SetSphere(Vector3(0.f, 1.f, 0.f), 1.f);
							SAFE_RELEASE(pCollider);
						}
						Golem*	pGolem = pGolemObject->AddComponent<Golem>("Golem");
						pGolem->setCurrentHP(pPacket->current_hp);
						pGolem->setCurrentMP(pPacket->current_mp);
						pGolem->setLevel(pPacket->level);
						pGolem->setMaxHP(30 + pPacket->level * 10);
						pGolem->setMaxMP(10);
						pGolem->setAttackDamag(3 + pPacket->level * 1);
						pGolem->setExperience(pPacket->exp);
						SAFE_RELEASE(pGolem);
						CTransform*	pTr = pGolemObject->GetTransform();
						CColliderSphere* pCollider = pGolemObject->AddComponent<CColliderSphere>("collider");
						pCollider->SetSphere(Vector3(0.f, 0.f, 0.f), 2.f);
						SAFE_RELEASE(pCollider);
						float yPos = GET_SINGLE(CQuadTreeManager)->GetY(Vector3(pPacket->x, pPacket->y, pPacket->z));
						pTr->SetWorldRotY(XMConvertToRadians(pPacket->angle));
						pTr->SetWorldPos(pPacket->x, yPos, pPacket->z);
						pTr->SetWorldScale(2.f, 2.f, 2.f);
						SAFE_RELEASE(pGolemObject);
					}
				}
				else if (static_cast<sc_packet_put_player*>(packet)->objectSetType == OBJECT_SET_TYPE::MINO)
				{
					char str[128];
					string appendTag = _itoa(id, str, 10);
					string objectTag = "Player" + appendTag;

					CLayer*	pLayer = GET_SINGLE(CSceneManager)->GetCurrentScene()->GetLayer("Default");
					CGameObject* pGolemObj = CGameObject::FindObject(objectTag);
					if (nullptr == pGolemObj)
					{
						CGameObject* pMinoObject = CGameObject::CreateClone("MinoObject", pLayer);
						pMinoObject->SetTag(objectTag);
						{
							CColliderSphere* pCollider = pMinoObject->AddComponent<CColliderSphere>("collider");
							pCollider->SetSphere(Vector3(0.f, 1.f, 0.f), 1.f);
							SAFE_RELEASE(pCollider);
						}
						{
							CColliderSphere* pCollider = pMinoObject->AddComponent<CColliderSphere>("collider2");
							pCollider->SetSphere(Vector3(0.f, 2.f, 0.f), 1.f);
							SAFE_RELEASE(pCollider);
						}
						Mino*	pMino = pMinoObject->AddComponent<Mino>("Mino");
						pMino->setCurrentHP(pPacket->current_hp);
						pMino->setCurrentMP(pPacket->current_mp);
						pMino->setLevel(pPacket->level);
						pMino->setMaxHP(50 + pPacket->level * 5);
						pMino->setMaxMP(10);
						pMino->setAttackDamag(5 + pPacket->level * 1);
						pMino->setExperience(pPacket->exp);
						SAFE_RELEASE(pMino);
						CTransform*	pTr = pMinoObject->GetTransform();
						//CColliderSphere* pCollider = pMinoObject->AddComponent<CColliderSphere>("collider");
						//pCollider->SetSphere(Vector3(0.f, 0.f, 0.f), 2.f);
						//SAFE_RELEASE(pCollider);
						float yPos = GET_SINGLE(CQuadTreeManager)->GetY(Vector3(pPacket->x, pPacket->y, pPacket->z));
						pTr->SetLocalRotY(XMConvertToRadians(180.f));
						pTr->SetWorldRotY(XMConvertToRadians(pPacket->angle));
						pTr->SetWorldPos(pPacket->x, yPos, pPacket->z);
						pTr->SetWorldScale(2.f, 2.f, 2.f);

						SAFE_RELEASE(pTr);
						SAFE_RELEASE(pMinoObject);
					}
				}
				else if (static_cast<sc_packet_put_player*>(packet)->objectSetType == OBJECT_SET_TYPE::GREEN_LIZARD)
				{
					char str[128];
					string appendTag = _itoa(id, str, 10);
					string objectTag = "Player" + appendTag;

					CLayer*	pLayer = GET_SINGLE(CSceneManager)->GetCurrentScene()->GetLayer("Default");
					CGameObject* pGolemObj = CGameObject::FindObject(objectTag);
					if (nullptr == pGolemObj)
					{
						CGameObject* pGreenLizardObj = CGameObject::CreateClone("GreenLizard", pLayer);
						pGreenLizardObj->SetTag(objectTag);
						{
							CColliderSphere* pCollider = pGreenLizardObj->AddComponent<CColliderSphere>("collider");
							pCollider->SetSphere(Vector3(0.f, 1.f, 0.f), 1.f);
							SAFE_RELEASE(pCollider);
						}
						GreenLizard*	pGreenLizard = pGreenLizardObj->AddComponent<GreenLizard>("GreenLizard");
						pGreenLizard->setCurrentHP(pPacket->current_hp);
						pGreenLizard->setCurrentMP(pPacket->current_mp);
						pGreenLizard->setLevel(pPacket->level);
						pGreenLizard->setMaxHP(30 + pPacket->level * 5);
						pGreenLizard->setMaxMP(10);
						pGreenLizard->setAttackDamag(3 + pPacket->level * 1);
						pGreenLizard->setExperience(pPacket->exp);
						SAFE_RELEASE(pGreenLizard);
						CTransform*	pTr = pGreenLizardObj->GetTransform();
						//CColliderSphere* pCollider = pSeuteompiObj->AddComponent<CColliderSphere>("collider");
						//pCollider->SetSphere(Vector3(0.f, 0.f, 0.f), 2.f);
						//SAFE_RELEASE(pCollider);
						float yPos = GET_SINGLE(CQuadTreeManager)->GetY(Vector3(pPacket->x, pPacket->y, pPacket->z));
						pTr->SetWorldRotY(XMConvertToRadians(pPacket->angle));
						pTr->SetWorldPos(pPacket->x, yPos, pPacket->z);
						SAFE_RELEASE(pGreenLizardObj);
					}
				}
				else if (static_cast<sc_packet_put_player*>(packet)->objectSetType == OBJECT_SET_TYPE::BLUE_LIZARD)
				{
					char str[128];
					string appendTag = _itoa(id, str, 10);
					string objectTag = "Player" + appendTag;

					CLayer*	pLayer = GET_SINGLE(CSceneManager)->GetCurrentScene()->GetLayer("Default");
					CGameObject* pGolemObj = CGameObject::FindObject(objectTag);
					if (nullptr == pGolemObj)
					{
						CGameObject* pBlueLizardObj = CGameObject::CreateClone("BlueLizard", pLayer);
						pBlueLizardObj->SetTag(objectTag);
						{
							CColliderSphere* pCollider = pBlueLizardObj->AddComponent<CColliderSphere>("collider");
							pCollider->SetSphere(Vector3(0.f, 1.f, 0.f), 1.f);
							SAFE_RELEASE(pCollider);
						}
						BlueLizard*	pBlueLizard = pBlueLizardObj->AddComponent<BlueLizard>("BlueLizard");
						pBlueLizard->setCurrentHP(pPacket->current_hp);
						pBlueLizard->setCurrentMP(pPacket->current_mp);
						pBlueLizard->setLevel(pPacket->level);
						pBlueLizard->setMaxHP(30 + pPacket->level * 5);
						pBlueLizard->setMaxMP(10);
						pBlueLizard->setAttackDamag(3 + pPacket->level * 1);
						pBlueLizard->setExperience(pPacket->exp);
						SAFE_RELEASE(pBlueLizard);
						CTransform*	pTr = pBlueLizardObj->GetTransform();
						//CColliderSphere* pCollider = pSeuteompiObj->AddComponent<CColliderSphere>("collider");
						//pCollider->SetSphere(Vector3(0.f, 0.f, 0.f), 2.f);
						//SAFE_RELEASE(pCollider);
						float yPos = GET_SINGLE(CQuadTreeManager)->GetY(Vector3(pPacket->x, pPacket->y, pPacket->z));
						pTr->SetWorldRotY(XMConvertToRadians(pPacket->angle));
						pTr->SetWorldPos(pPacket->x, yPos, pPacket->z);
						SAFE_RELEASE(pBlueLizardObj);
					}
				}
				else if (static_cast<sc_packet_put_player*>(packet)->objectSetType == OBJECT_SET_TYPE::ARMORED_GREEN_LIZARD)
				{
					char str[128];
					string appendTag = _itoa(id, str, 10);
					string objectTag = "Player" + appendTag;

					CLayer*	pLayer = GET_SINGLE(CSceneManager)->GetCurrentScene()->GetLayer("Default");
					CGameObject* pGolemObj = CGameObject::FindObject(objectTag);
					if (nullptr == pGolemObj)
					{
						CGameObject* pArmored_GreenLizardObj = CGameObject::CreateClone("Armored_GreenLizard", pLayer);
						pArmored_GreenLizardObj->SetTag(objectTag);
						{
							CColliderSphere* pCollider = pArmored_GreenLizardObj->AddComponent<CColliderSphere>("collider");
							pCollider->SetSphere(Vector3(0.f, 1.f, 0.f), 1.f);
							SAFE_RELEASE(pCollider);
						}
						BlueLizard*	pArmored_GreenLizard = pArmored_GreenLizardObj->AddComponent<BlueLizard>("Armored_GreenLizard");
						pArmored_GreenLizard->setCurrentHP(pPacket->current_hp);
						pArmored_GreenLizard->setCurrentMP(pPacket->current_mp);
						pArmored_GreenLizard->setLevel(pPacket->level);
						pArmored_GreenLizard->setMaxHP(50 + pPacket->level * 10);
						pArmored_GreenLizard->setMaxMP(10);
						pArmored_GreenLizard->setAttackDamag(10 + pPacket->level * 2);
						pArmored_GreenLizard->setExperience(pPacket->exp);
						SAFE_RELEASE(pArmored_GreenLizard);
						CTransform*	pTr = pArmored_GreenLizardObj->GetTransform();
						//CColliderSphere* pCollider = pSeuteompiObj->AddComponent<CColliderSphere>("collider");
						//pCollider->SetSphere(Vector3(0.f, 0.f, 0.f), 2.f);
						//SAFE_RELEASE(pCollider);
						float yPos = GET_SINGLE(CQuadTreeManager)->GetY(Vector3(pPacket->x, pPacket->y, pPacket->z));
						pTr->SetWorldRotY(XMConvertToRadians(pPacket->angle));
						pTr->SetWorldPos(pPacket->x, yPos, pPacket->z);

						SAFE_RELEASE(pArmored_GreenLizardObj);
					}
				}
				else if (static_cast<sc_packet_put_player*>(packet)->objectSetType == OBJECT_SET_TYPE::ARMORED_BLUE_LIZARD)
				{
					char str[128];
					string appendTag = _itoa(id, str, 10);
					string objectTag = "Player" + appendTag;

					CLayer*	pLayer = GET_SINGLE(CSceneManager)->GetCurrentScene()->GetLayer("Default");
					CGameObject* pGolemObj = CGameObject::FindObject(objectTag);
					if (nullptr == pGolemObj)
					{
						CGameObject* pArmored_BlueLizardObj = CGameObject::CreateClone("Armored_BlueLizard", pLayer);
						pArmored_BlueLizardObj->SetTag(objectTag);
						{
							CColliderSphere* pCollider = pArmored_BlueLizardObj->AddComponent<CColliderSphere>("collider");
							pCollider->SetSphere(Vector3(0.f, 1.f, 0.f), 1.f);
							SAFE_RELEASE(pCollider);
						}
						BlueLizard*	pArmored_BlueLizard = pArmored_BlueLizardObj->AddComponent<BlueLizard>("Armored_GreenLizard");
						pArmored_BlueLizard->setCurrentHP(pPacket->current_hp);
						pArmored_BlueLizard->setCurrentMP(pPacket->current_mp);
						pArmored_BlueLizard->setLevel(pPacket->level);
						pArmored_BlueLizard->setMaxHP(50 + pPacket->level * 10);
						pArmored_BlueLizard->setMaxMP(10);
						pArmored_BlueLizard->setAttackDamag(10 + pPacket->level * 2);
						pArmored_BlueLizard->setExperience(pPacket->exp);
						SAFE_RELEASE(pArmored_BlueLizard);
						CTransform*	pTr = pArmored_BlueLizardObj->GetTransform();
						//CColliderSphere* pCollider = pSeuteompiObj->AddComponent<CColliderSphere>("collider");
						//pCollider->SetSphere(Vector3(0.f, 0.f, 0.f), 2.f);
						//SAFE_RELEASE(pCollider);
						float yPos = GET_SINGLE(CQuadTreeManager)->GetY(Vector3(pPacket->x, pPacket->y, pPacket->z));
						pTr->SetWorldRotY(XMConvertToRadians(pPacket->angle));
						pTr->SetWorldPos(pPacket->x, yPos, pPacket->z);
						SAFE_RELEASE(pArmored_BlueLizardObj);
					}
				}
				else if (static_cast<sc_packet_put_player*>(packet)->objectSetType == OBJECT_SET_TYPE::DEMONLORD)
				{

					char str[128];
					string appendTag = _itoa(id, str, 10);
					string objectTag = "Player" + appendTag;

					CLayer*	pLayer = GET_SINGLE(CSceneManager)->GetCurrentScene()->GetLayer("Default");
					CGameObject* pGolemObj = CGameObject::FindObject(objectTag);
					if (nullptr == pGolemObj)
					{
						CGameObject* pDemonLordObj = CGameObject::CreateClone("DemonLordObjeect", pLayer);
						pDemonLordObj->SetTag(objectTag);
						{
							CColliderSphere* pCollider = pDemonLordObj->AddComponent<CColliderSphere>("collider");
							pCollider->SetSphere(Vector3(0.f, 1.f, 0.f), 1.f);
							SAFE_RELEASE(pCollider);
						}
						DemonLord*	pDemonLord = pDemonLordObj->AddComponent<DemonLord>("DemonLord");
						pDemonLord->setCurrentHP(pPacket->current_hp);
						pDemonLord->setCurrentMP(pPacket->current_mp);
						pDemonLord->setLevel(pPacket->level);
						pDemonLord->setMaxHP(1200);
						pDemonLord->setMaxMP(300);
						pDemonLord->setAttackDamag(25);
						pDemonLord->setExperience(pPacket->exp);
						SAFE_RELEASE(pDemonLord);
						CTransform*	pTr = pDemonLordObj->GetTransform();

						float yPos = GET_SINGLE(CQuadTreeManager)->GetY(Vector3(pPacket->x, pPacket->y, pPacket->z));
						pTr->SetWorldRotY(XMConvertToRadians(pPacket->angle));
						pTr->SetWorldPos(pPacket->x, yPos, pPacket->z);
						SAFE_RELEASE(pDemonLordObj);
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
		case SC_PACKET_IDLE_ANIMATION:
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
				pAnimation->ChangeClip("Idle2");
				SAFE_RELEASE(pAnimation);
				/*Mino* pMino = pGameObject->FindComponentFromTag<Mino>("Mino");
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
				DemonLord* pDemonLord = pGameObject->FindComponentFromTag<DemonLord>("DemonLord");
				if (nullptr != pDemonLord)
				{
					pDemonLord->setDieState(true);
					SAFE_RELEASE(pMino);
				}*/
			}
		}
		break;
		case SC_PACKET_MOVE:
		{
			sc_packet_pos* pPacket = reinterpret_cast<sc_packet_pos*>(packet);
			int id = pPacket->id;
			int myClientID = NetworkManager::getInstance()->getMyClientID();
			if (id == NetworkManager::getInstance()->getMyClientID())
			{
				char str[128];
				string appendTag = to_string(myClientID);
				string objectTag = "Player" + appendTag;

				CGameObject* pGameObject = CGameObject::FindObject(objectTag);
				if (pGameObject == nullptr)
					break;
				CTransform* pTransform = pGameObject->GetTransform();

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

				/*CTransform* pLightTransform = GET_SINGLE(CSceneManager)->GetCurrentScene()->GetLightCameraTr();
				Vector3 LightPos = Vector3{ pPacket->x, pPacket->y, pPacket->z } +Vector3{ -15, 15, -15 };
				pLightTransform->SetWorldPos(LightPos);*/
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
				if (pGameObject == nullptr)
					break;
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

		case SC_PACKET_SCENECHANGE_SCENE2:
		{
			int id = NetworkManager::getInstance()->myClientID;
			string appendTag = to_string(id);
			string objectTag = "Player" + appendTag;
			CGameObject* pGameObject = CGameObject::FindObject(objectTag);
			CTransform* pTr = pGameObject->GetTransform();
			pTr->SetWorldPos(200, 0, 200);
			CScene*	pScene = GET_SINGLE(CSceneManager)->CreateNextScene("SecondScene");

			pScene->CreateSceneScript<SecondScene>("SecondScene", false);

			//SAFE_RELEASE(pMainScene);

			SAFE_RELEASE(pScene);
		}
		break;

		case SC_PACKET_SCENECHANGE_DEVIL_DUNGEON:
		{
			int id = NetworkManager::getInstance()->myClientID;
			string appendTag = to_string(id);
			string objectTag = "Player" + appendTag;
			CGameObject* pGameObject = CGameObject::FindObject(objectTag);
			CTransform* pTr = pGameObject->GetTransform();
			pTr->SetWorldPos(200, 0, 200);
			CScene*	pScene = GET_SINGLE(CSceneManager)->CreateNextScene("DungeonScene");

			pScene->CreateSceneScript<DungeonScene>("DungeonScene", false);

			//SAFE_RELEASE(pMainScene);

			SAFE_RELEASE(pScene);
		}
		break;

		case SC_PACKET_MOVE_STOP:
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
				pAnimation->ChangeClip("Idle1");
				SAFE_RELEASE(pAnimation);
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

			string myappendTag = to_string(NetworkManager::getInstance()->getMyClientID());
			string myobjectTag = "Player" + myappendTag;
			CGameObject* pMyPlayerObj = CGameObject::FindObject(myobjectTag);
			if (pMyPlayerObj == nullptr)
				break;

			CPlayer* pMyPlayer = pMyPlayerObj->FindComponentFromTag<CPlayer>("Player");
			int myTargetID = pMyPlayer->clickedID;
			SAFE_RELEASE(pMyPlayer);
			SAFE_RELEASE(pMyPlayerObj);

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
							Golem* pGolem = pTargetObject->FindComponentFromTag<Golem>("Golem");

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
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == (int)OBJECT_SET_TYPE::GREEN_LIZARD)
						{
							GreenLizard* pGreenLizard = pTargetObject->FindComponentFromTag<GreenLizard>("GreenLizard");

							int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(0.f);
							}
							else
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(ratio);
							}
						}
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == (int)OBJECT_SET_TYPE::BLUE_LIZARD)
						{
							BlueLizard* pGreenLizard = pTargetObject->FindComponentFromTag<BlueLizard>("BlueLizard");

							int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(0.f);
							}
							else
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(ratio);
							}
						}
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::ARMORED_GREEN_LIZARD)
						{
							Armored_GreenLizard* pGreenLizard = pTargetObject->FindComponentFromTag<Armored_GreenLizard>("Armored_GreenLizard");

							int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(0.f);
							}
							else
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(ratio);
							}
						}
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::ARMORED_BLUE_LIZARD)
						{
							Armored_BlueLizard* pGreenLizard = pTargetObject->FindComponentFromTag<Armored_BlueLizard>("Armored_BlueLizard");

							int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(0.f);
							}
							else
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(ratio);
							}
						}

						else if (reinterpret_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::DEMONLORD)
						{
							DemonLord* pDemonLord = pTargetObject->FindComponentFromTag<DemonLord>("DemonLord");

							int hp = pDemonLord->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pDemonLord->getMaxHP();
								pDemonLord->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(0.f);
							}
							else
							{
								float ratio = (float)hp / (float)pDemonLord->getMaxHP();
								pDemonLord->setCurrentHP(hp);
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
							if (pGolem != nullptr)
							{
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
									if (myTargetID == pPacket->targetid)
									{
										CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
										pEnemyUIHearthBar->setLengthRatio(0);
									}
								}
								else
								{
									float ratio = (float)hp / (float)pGolem->getMaxHP();
									pGolem->setCurrentHP(hp);
									if (myTargetID == pPacket->targetid)
									{
										CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
										pEnemyUIHearthBar->setLengthRatio(ratio);
									}
								}
							}
						}
						else if (reinterpret_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::MINO)
						{
							Mino* pMino = pTargetObject->FindComponentFromTag<Mino>("Mino");
							if (pMino != nullptr)
							{
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
									if (myTargetID == pPacket->targetid)
									{
										CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
										pEnemyUIHearthBar->setLengthRatio(0);
									}
								}
								else
								{
									float ratio = (float)hp / (float)pMino->getMaxHP();
									pMino->setCurrentHP(hp);
									if (myTargetID == pPacket->targetid)
									{
										CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
										pEnemyUIHearthBar->setLengthRatio(ratio);
									}
								}
							}
						}
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::GREEN_LIZARD)
						{
							GreenLizard* pGreenLizard = pTargetObject->FindComponentFromTag<GreenLizard>("GreenLizard");
							if (pGreenLizard != nullptr)
							{
								int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

								//CTransform* pTransform = pSeuteompi->GetTransform();
								//Vector3 vPos = pTransform->GetWorldPos();
								//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
								//vPos += vLook * 1.25f;
								//vPos.y += 0.95f;
								//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
								//SAFE_RELEASE(pTransform);

								if (hp < 0)
								{
									float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
									pGreenLizard->setCurrentHP(hp);
									if (myTargetID == pPacket->targetid)
									{
										CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
										pEnemyUIHearthBar->setLengthRatio(0);
									}
								}
								else
								{
									float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
									pGreenLizard->setCurrentHP(hp);
									if (myTargetID == pPacket->targetid)
									{
										CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
										pEnemyUIHearthBar->setLengthRatio(ratio);
									}
								}
							}
						}
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::BLUE_LIZARD)
						{
							BlueLizard* pGreenLizard = pTargetObject->FindComponentFromTag<BlueLizard>("BlueLizard");
							if (pGreenLizard != nullptr)
							{
								int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

								//CTransform* pTransform = pSeuteompi->GetTransform();
								//Vector3 vPos = pTransform->GetWorldPos();
								//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
								//vPos += vLook * 1.25f;
								//vPos.y += 0.95f;
								//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
								//SAFE_RELEASE(pTransform);

								if (hp < 0)
								{
									float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
									pGreenLizard->setCurrentHP(hp);
									if (myTargetID == pPacket->targetid)
									{
										CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
										pEnemyUIHearthBar->setLengthRatio(0);
									}
								}
								else
								{
									float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
									pGreenLizard->setCurrentHP(hp);
									if (myTargetID == pPacket->targetid)
									{
										CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
										pEnemyUIHearthBar->setLengthRatio(ratio);
									}
								}
							}
						}
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::ARMORED_GREEN_LIZARD)
						{
							Armored_GreenLizard* pGreenLizard = pTargetObject->FindComponentFromTag<Armored_GreenLizard>("Armored_GreenLizard");
							if (pGreenLizard != nullptr)
							{
								int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

								//CTransform* pTransform = pSeuteompi->GetTransform();
								//Vector3 vPos = pTransform->GetWorldPos();
								//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
								//vPos += vLook * 1.25f;
								//vPos.y += 0.95f;
								//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
								//SAFE_RELEASE(pTransform);

								if (hp < 0)
								{
									float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
									pGreenLizard->setCurrentHP(hp);
									if (myTargetID == pPacket->targetid)
									{
										CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
										pEnemyUIHearthBar->setLengthRatio(0);
									}
								}
								else
								{
									float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
									pGreenLizard->setCurrentHP(hp);
									if (myTargetID == pPacket->targetid)
									{
										CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
										pEnemyUIHearthBar->setLengthRatio(ratio);
									}
								}
							}
						}
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::ARMORED_BLUE_LIZARD)
						{
							Armored_BlueLizard* pGreenLizard = pTargetObject->FindComponentFromTag<Armored_BlueLizard>("Armored_GreenLizard");
							if (pGreenLizard != nullptr)
							{
								int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

								//CTransform* pTransform = pSeuteompi->GetTransform();
								//Vector3 vPos = pTransform->GetWorldPos();
								//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
								//vPos += vLook * 1.25f;
								//vPos.y += 0.95f;
								//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
								//SAFE_RELEASE(pTransform);

								if (hp < 0)
								{
									float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
									pGreenLizard->setCurrentHP(hp);
									if (myTargetID == pPacket->targetid)
									{
										CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
										pEnemyUIHearthBar->setLengthRatio(0);
									}
								}
								else
								{
									float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
									pGreenLizard->setCurrentHP(hp);
									if (myTargetID == pPacket->targetid)
									{
										CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
										pEnemyUIHearthBar->setLengthRatio(ratio);
									}
								}
							}
						}

						else if (reinterpret_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::DEMONLORD)
						{
							DemonLord* pDemonLord = pTargetObject->FindComponentFromTag<DemonLord>("DemonLord");

							if (pDemonLord != nullptr)
							{
								int hp = pDemonLord->getCurrentHP() - pPacket->damage;

								//CTransform* pTransform = pDemonLord->GetTransform();
								//Vector3 vPos = pTransform->GetWorldPos();
								//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
								//vPos += vLook * 1.25f;
								//vPos.y += 0.95f;
								//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
								//SAFE_RELEASE(pTransform);

								if (hp < 0)
								{
									float ratio = (float)hp / (float)pDemonLord->getMaxHP();
									pDemonLord->setCurrentHP(hp);
									if (myTargetID == pPacket->targetid)
									{
										CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
										pEnemyUIHearthBar->setLengthRatio(0);
									}
								}
								else
								{
									float ratio = (float)hp / (float)pDemonLord->getMaxHP();
									pDemonLord->setCurrentHP(hp);
									if (myTargetID == pPacket->targetid)
									{
										CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
										pEnemyUIHearthBar->setLengthRatio(ratio);
									}
								}
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
					if (reinterpret_cast<sc_packet_attack_player*>(packet)->objectSetType != OBJECT_SET_TYPE::DEMONLORD)
					{
						//DemonLord* pDemonLord = pGameObject->FindComponentFromTag<DemonLord>("DemonLord");
						//pDemonLord->changeAnimation();
						CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
						pAnimation->ChangeClip("Attack1");
						SAFE_RELEASE(pAnimation);
					}
					else
					{
						CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
						wstring temp_animation_name = reinterpret_cast<sc_packet_attack_player*>(packet)->animation_name;
						string animation_name = strconv(temp_animation_name);
						pAnimation->ChangeClip(animation_name);
						SAFE_RELEASE(pAnimation);
					}
				}
				SAFE_RELEASE(pGameObject);

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

							//string test = to_string(hp);
							//test += "\n";
							//OutputDebugStringA(test.c_str());
							if (hp < 0)
							{
								float ratio = (float)hp / (float)pPlayer->getMaxHP();
								pPlayer->setCurrentHP(hp);

								string test = to_string(hp);
								test += "\n";
								OutputDebugStringA(test.c_str());

								/*if (myTargetID == pPacket->targetid)
								{
									CUIButton* pUIHearthBar = GET_SINGLE(UserInterfaceManager)->getUIHeartBar();
									pUIHearthBar->setLengthRatio(0.f);
								}*/
							}
							else
							{
								float ratio = (float)hp / (float)pPlayer->getMaxHP();
								pPlayer->setCurrentHP(hp);

								string test = to_string(hp);
								test += "\n";
								OutputDebugStringA(test.c_str());

								//if (myTargetID == pPacket->targetid)
								//{
								//	CUIButton* pUIHearthBar = GET_SINGLE(UserInterfaceManager)->getUIHeartBar();
								//	pUIHearthBar->setLengthRatio(ratio);
								//}
							}
						}
						else if(pPacket->targetid < NPC_START)
						{
							CPlayer* pPlayer = pTargetObject->FindComponentFromTag<CPlayer>("Player");
							int hp = pPlayer->getCurrentHP() - pPacket->damage;
							if (hp < 0)
							{
								float ratio = (float)hp / (float)pPlayer->getMaxHP();
								pPlayer->setCurrentHP(hp);
								if (myTargetID != pPacket->targetid)
								{
									/*PartyStatus* pStatus = GET_SINGLE(UserInterfaceManager)->findPartyState(objectTag);
									if (pStatus != nullptr)
									{
										CUIButton* pUIHearthBar = pStatus->getUIHearthBar();
										pUIHearthBar->setLengthRatio(0.f);
									}*/
								}
							}
							else
							{

								float ratio = (float)hp / (float)pPlayer->getMaxHP();
								pPlayer->setCurrentHP(hp);
								if (myTargetID != pPacket->targetid)
								{
									/*PartyStatus* pStatus = GET_SINGLE(UserInterfaceManager)->findPartyState(objectTag);
									if (pStatus != nullptr)
									{
										CUIButton* pUIHearthBar = pStatus->getUIHearthBar();
										pUIHearthBar->setLengthRatio(ratio);
									}*/
								}
							}
						}
					}

				}
			}

		}
		break;

		case SC_PACKET_ATTACK_SKILL1:
		{
			char str[128];
			sc_packet_attack_player* pPacket = reinterpret_cast<sc_packet_attack_player*>(packet);
			int id = pPacket->id;

			string appendTag = to_string(id);
			string objectTag = "Player" + appendTag;

			string myappendTag = to_string(NetworkManager::getInstance()->getMyClientID());
			string myobjectTag = "Player" + myappendTag;
			CGameObject* pMyPlayerObj = CGameObject::FindObject(myobjectTag);
			CPlayer* pMyPlayer = pMyPlayerObj->FindComponentFromTag<CPlayer>("Player");
			if (pMyPlayer == nullptr)
				break;
			int myTargetID = pMyPlayer->clickedID;
			SAFE_RELEASE(pMyPlayer);
			SAFE_RELEASE(pMyPlayerObj);

			if (id == NetworkManager::getInstance()->getMyClientID())
			{
				_cprintf("MonsterType : %d", pPacket->objectSetType);
				CGameObject* pGameObject = CGameObject::FindObject(objectTag);
				if (nullptr != pGameObject)
				{
					CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
					pAnimation->ChangeClip("Spell1");
					CTransform* pTr = pGameObject->GetTransform();
					Vector3 pos = pTr->GetWorldPos();
					GET_SINGLE(CEffectManager)->OperateEffect("Spell1", nullptr, pos);
					SAFE_RELEASE(pTr);
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
							Golem* pGolem = pTargetObject->FindComponentFromTag<Golem>("Golem");

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
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == (int)OBJECT_SET_TYPE::GREEN_LIZARD)
						{
							GreenLizard* pGreenLizard = pTargetObject->FindComponentFromTag<GreenLizard>("GreenLizard");

							int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(0.f);
							}
							else
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(ratio);
							}
						}
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == (int)OBJECT_SET_TYPE::BLUE_LIZARD)
						{
							BlueLizard* pGreenLizard = pTargetObject->FindComponentFromTag<BlueLizard>("BlueLizard");

							int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(0.f);
							}
							else
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(ratio);
							}
						}
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::ARMORED_GREEN_LIZARD)
						{
							Armored_GreenLizard* pGreenLizard = pTargetObject->FindComponentFromTag<Armored_GreenLizard>("Armored_GreenLizard");

							int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(0.f);
							}
							else
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(ratio);
							}
						}
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::ARMORED_BLUE_LIZARD)
						{
							Armored_BlueLizard* pGreenLizard = pTargetObject->FindComponentFromTag<Armored_BlueLizard>("Armored_BlueLizard");

							int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(0.f);
							}
							else
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(ratio);
							}
						}

						else if (reinterpret_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::DEMONLORD)
						{
							DemonLord* pDemonLord = pTargetObject->FindComponentFromTag<DemonLord>("DemonLord");

							int hp = pDemonLord->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pDemonLord->getMaxHP();
								pDemonLord->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(0.f);
							}
							else
							{
								float ratio = (float)hp / (float)pDemonLord->getMaxHP();
								pDemonLord->setCurrentHP(hp);
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
					pAnimation->ChangeClip("Spell1");
					CTransform* pTr = pGameObject->GetTransform();
					Vector3 pos = pTr->GetWorldPos();
					GET_SINGLE(CEffectManager)->OperateEffect("Spell1", nullptr, pos);
					SAFE_RELEASE(pTr);
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
							Golem* pGolem = pTargetObject->FindComponentFromTag<Golem>("Golem");

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
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(0);
								}
							}
							else
							{
								float ratio = (float)hp / (float)pGolem->getMaxHP();
								pGolem->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(ratio);
								}
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
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(0);
								}
							}
							else
							{
								float ratio = (float)hp / (float)pMino->getMaxHP();
								pMino->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(ratio);
								}
							}
						}
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::GREEN_LIZARD)
						{
							GreenLizard* pGreenLizard = pTargetObject->FindComponentFromTag<GreenLizard>("GreenLizard");

							int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(0);
								}
							}
							else
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(ratio);
								}
							}
						}
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::BLUE_LIZARD)
						{
							BlueLizard* pGreenLizard = pTargetObject->FindComponentFromTag<BlueLizard>("BlueLizard");

							int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(0);
								}
							}
							else
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(ratio);
								}
							}
						}
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::ARMORED_GREEN_LIZARD)
						{
							Armored_GreenLizard* pGreenLizard = pTargetObject->FindComponentFromTag<Armored_GreenLizard>("Armored_GreenLizard");

							int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(0);
								}
							}
							else
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(ratio);
								}
							}
						}
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::ARMORED_BLUE_LIZARD)
						{
							Armored_BlueLizard* pGreenLizard = pTargetObject->FindComponentFromTag<Armored_BlueLizard>("Armored_GreenLizard");

							int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(0);
								}
							}
							else
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(ratio);
								}
							}
						}

						else if (reinterpret_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::DEMONLORD)
						{
							DemonLord* pDemonLord = pTargetObject->FindComponentFromTag<DemonLord>("DemonLord");

							int hp = pDemonLord->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pDemonLord->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pDemonLord->getMaxHP();
								pDemonLord->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(0);
								}
							}
							else
							{
								float ratio = (float)hp / (float)pDemonLord->getMaxHP();
								pDemonLord->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(ratio);
								}
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
					if (reinterpret_cast<sc_packet_attack_player*>(packet)->objectSetType != OBJECT_SET_TYPE::DEMONLORD)
					{
						//DemonLord* pDemonLord = pGameObject->FindComponentFromTag<DemonLord>("DemonLord");
						//pDemonLord->changeAnimation();
						CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
						pAnimation->ChangeClip("Attack1");
						SAFE_RELEASE(pAnimation);
					}
					else
					{
						CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
						wstring temp_animation_name = reinterpret_cast<sc_packet_attack_player*>(packet)->animation_name;
						string animation_name = strconv(temp_animation_name);
						pAnimation->ChangeClip(animation_name);
						SAFE_RELEASE(pAnimation);
					}
				}
				SAFE_RELEASE(pGameObject);

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
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pUIHearthBar = GET_SINGLE(UserInterfaceManager)->getUIHeartBar();
									pUIHearthBar->setLengthRatio(0.f);
								}
							}
							else
							{
								float ratio = (float)hp / (float)pPlayer->getMaxHP();
								pPlayer->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pUIHearthBar = GET_SINGLE(UserInterfaceManager)->getUIHeartBar();
									pUIHearthBar->setLengthRatio(ratio);
								}
							}
						}
					}

				}
			}

		}
		break;

		case SC_PACKET_ATTACK_SKILL2:
		{
			char str[128];
			sc_packet_attack_player* pPacket = reinterpret_cast<sc_packet_attack_player*>(packet);
			int id = pPacket->id;

			string appendTag = to_string(id);
			string objectTag = "Player" + appendTag;

			string myappendTag = to_string(NetworkManager::getInstance()->getMyClientID());
			string myobjectTag = "Player" + myappendTag;
			CGameObject* pMyPlayerObj = CGameObject::FindObject(myobjectTag);
			CPlayer* pMyPlayer = pMyPlayerObj->FindComponentFromTag<CPlayer>("Player");
			int myTargetID = pMyPlayer->clickedID;
			SAFE_RELEASE(pMyPlayer);
			SAFE_RELEASE(pMyPlayerObj);

			if (id == NetworkManager::getInstance()->getMyClientID())
			{
				_cprintf("MonsterType : %d", pPacket->objectSetType);
				CGameObject* pGameObject = CGameObject::FindObject(objectTag);
				if (nullptr != pGameObject)
				{
					CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
					pAnimation->ChangeClip("Spell2");
					CTransform* pTr = pGameObject->GetTransform();
					Vector3 pos = pTr->GetWorldPos();
					//GET_SINGLE(CEffectManager)->OperateEffect("Spell2", nullptr, pos);
					SAFE_RELEASE(pTr);
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
							Golem* pGolem = pTargetObject->FindComponentFromTag<Golem>("Golem");

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
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == (int)OBJECT_SET_TYPE::GREEN_LIZARD)
						{
							GreenLizard* pGreenLizard = pTargetObject->FindComponentFromTag<GreenLizard>("GreenLizard");

							int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(0.f);
							}
							else
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(ratio);
							}
						}
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == (int)OBJECT_SET_TYPE::BLUE_LIZARD)
						{
							BlueLizard* pGreenLizard = pTargetObject->FindComponentFromTag<BlueLizard>("BlueLizard");

							int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(0.f);
							}
							else
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(ratio);
							}
						}
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::ARMORED_GREEN_LIZARD)
						{
							Armored_GreenLizard* pGreenLizard = pTargetObject->FindComponentFromTag<Armored_GreenLizard>("Armored_GreenLizard");

							int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(0.f);
							}
							else
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(ratio);
							}
						}
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::ARMORED_BLUE_LIZARD)
						{
							Armored_BlueLizard* pGreenLizard = pTargetObject->FindComponentFromTag<Armored_BlueLizard>("Armored_BlueLizard");

							int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(0.f);
							}
							else
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(ratio);
							}
						}

						else if (reinterpret_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::DEMONLORD)
						{
							DemonLord* pDemonLord = pTargetObject->FindComponentFromTag<DemonLord>("DemonLord");

							int hp = pDemonLord->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pDemonLord->getMaxHP();
								pDemonLord->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(0.f);
							}
							else
							{
								float ratio = (float)hp / (float)pDemonLord->getMaxHP();
								pDemonLord->setCurrentHP(hp);
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
					pAnimation->ChangeClip("Spell2");
					CTransform* pTr = pGameObject->GetTransform();
					Vector3 pos = pTr->GetWorldPos();
					//GET_SINGLE(CEffectManager)->OperateEffect("Spell2", nullptr, pos);
					SAFE_RELEASE(pTr);
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
							Golem* pGolem = pTargetObject->FindComponentFromTag<Golem>("Golem");

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
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(0);
								}
							}
							else
							{
								float ratio = (float)hp / (float)pGolem->getMaxHP();
								pGolem->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(ratio);
								}
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
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(0);
								}
							}
							else
							{
								float ratio = (float)hp / (float)pMino->getMaxHP();
								pMino->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(ratio);
								}
							}
						}
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::GREEN_LIZARD)
						{
							GreenLizard* pGreenLizard = pTargetObject->FindComponentFromTag<GreenLizard>("GreenLizard");

							int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(0);
								}
							}
							else
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(ratio);
								}
							}
						}
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::BLUE_LIZARD)
						{
							BlueLizard* pGreenLizard = pTargetObject->FindComponentFromTag<BlueLizard>("BlueLizard");

							int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(0);
								}
							}
							else
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(ratio);
								}
							}
						}
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::ARMORED_GREEN_LIZARD)
						{
							Armored_GreenLizard* pGreenLizard = pTargetObject->FindComponentFromTag<Armored_GreenLizard>("Armored_GreenLizard");

							int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(0);
								}
							}
							else
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(ratio);
								}
							}
						}
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::ARMORED_BLUE_LIZARD)
						{
							Armored_BlueLizard* pGreenLizard = pTargetObject->FindComponentFromTag<Armored_BlueLizard>("Armored_GreenLizard");

							int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(0);
								}
							}
							else
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(ratio);
								}
							}
						}

						else if (reinterpret_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::DEMONLORD)
						{
							DemonLord* pDemonLord = pTargetObject->FindComponentFromTag<DemonLord>("DemonLord");

							int hp = pDemonLord->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pDemonLord->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pDemonLord->getMaxHP();
								pDemonLord->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(0);
								}
							}
							else
							{
								float ratio = (float)hp / (float)pDemonLord->getMaxHP();
								pDemonLord->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(ratio);
								}
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
					if (reinterpret_cast<sc_packet_attack_player*>(packet)->objectSetType != OBJECT_SET_TYPE::DEMONLORD)
					{
						//DemonLord* pDemonLord = pGameObject->FindComponentFromTag<DemonLord>("DemonLord");
						//pDemonLord->changeAnimation();
						CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
						pAnimation->ChangeClip("Attack1");
						SAFE_RELEASE(pAnimation);
					}
					else
					{
						CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
						wstring temp_animation_name = reinterpret_cast<sc_packet_attack_player*>(packet)->animation_name;
						string animation_name = strconv(temp_animation_name);
						pAnimation->ChangeClip(animation_name);
						SAFE_RELEASE(pAnimation);
					}
				}
				SAFE_RELEASE(pGameObject);

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
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pUIHearthBar = GET_SINGLE(UserInterfaceManager)->getUIHeartBar();
									pUIHearthBar->setLengthRatio(0.f);
								}
							}
							else
							{
								float ratio = (float)hp / (float)pPlayer->getMaxHP();
								pPlayer->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pUIHearthBar = GET_SINGLE(UserInterfaceManager)->getUIHeartBar();
									pUIHearthBar->setLengthRatio(ratio);
								}
							}
						}
					}

				}
			}

		}
		break;

		case SC_PACKET_ATTACK_SKILL3:
		{
			char str[128];
			sc_packet_attack_player* pPacket = reinterpret_cast<sc_packet_attack_player*>(packet);
			int id = pPacket->id;

			string appendTag = to_string(id);
			string objectTag = "Player" + appendTag;

			string myappendTag = to_string(NetworkManager::getInstance()->getMyClientID());
			string myobjectTag = "Player" + myappendTag;
			CGameObject* pMyPlayerObj = CGameObject::FindObject(myobjectTag);
			CPlayer* pMyPlayer = pMyPlayerObj->FindComponentFromTag<CPlayer>("Player");
			int myTargetID = pMyPlayer->clickedID;
			SAFE_RELEASE(pMyPlayer);
			SAFE_RELEASE(pMyPlayerObj);

			if (id == NetworkManager::getInstance()->getMyClientID())
			{
				_cprintf("MonsterType : %d", pPacket->objectSetType);
				CGameObject* pGameObject = CGameObject::FindObject(objectTag);
				if (nullptr != pGameObject)
				{
					CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
					pAnimation->ChangeClip("Spell3");
					CTransform* pTr = pGameObject->GetTransform();
					Vector3 pos = pTr->GetWorldPos();
					GET_SINGLE(CEffectManager)->OperateEffect("Spell3", nullptr, pos);
					SAFE_RELEASE(pTr);
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
							Golem* pGolem = pTargetObject->FindComponentFromTag<Golem>("Golem");

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
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == (int)OBJECT_SET_TYPE::GREEN_LIZARD)
						{
							GreenLizard* pGreenLizard = pTargetObject->FindComponentFromTag<GreenLizard>("GreenLizard");

							int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(0.f);
							}
							else
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(ratio);
							}
						}
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == (int)OBJECT_SET_TYPE::BLUE_LIZARD)
						{
							BlueLizard* pGreenLizard = pTargetObject->FindComponentFromTag<BlueLizard>("BlueLizard");

							int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(0.f);
							}
							else
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(ratio);
							}
						}
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::ARMORED_GREEN_LIZARD)
						{
							Armored_GreenLizard* pGreenLizard = pTargetObject->FindComponentFromTag<Armored_GreenLizard>("Armored_GreenLizard");

							int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(0.f);
							}
							else
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(ratio);
							}
						}
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::ARMORED_BLUE_LIZARD)
						{
							Armored_BlueLizard* pGreenLizard = pTargetObject->FindComponentFromTag<Armored_BlueLizard>("Armored_BlueLizard");

							int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(0.f);
							}
							else
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(ratio);
							}
						}

						else if (reinterpret_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::DEMONLORD)
						{
							DemonLord* pDemonLord = pTargetObject->FindComponentFromTag<DemonLord>("DemonLord");

							int hp = pDemonLord->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pDemonLord->getMaxHP();
								pDemonLord->setCurrentHP(hp);
								CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
								pEnemyUIHearthBar->setLengthRatio(0.f);
							}
							else
							{
								float ratio = (float)hp / (float)pDemonLord->getMaxHP();
								pDemonLord->setCurrentHP(hp);
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
					pAnimation->ChangeClip("Spell3");
					CTransform* pTr = pGameObject->GetTransform();
					Vector3 pos = pTr->GetWorldPos();
					GET_SINGLE(CEffectManager)->OperateEffect("Spell3", nullptr, pos);
					SAFE_RELEASE(pTr);
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
							Golem* pGolem = pTargetObject->FindComponentFromTag<Golem>("Golem");

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
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(0);
								}
							}
							else
							{
								float ratio = (float)hp / (float)pGolem->getMaxHP();
								pGolem->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(ratio);
								}
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
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(0);
								}
							}
							else
							{
								float ratio = (float)hp / (float)pMino->getMaxHP();
								pMino->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(ratio);
								}
							}
						}
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::GREEN_LIZARD)
						{
							GreenLizard* pGreenLizard = pTargetObject->FindComponentFromTag<GreenLizard>("GreenLizard");

							int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(0);
								}
							}
							else
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(ratio);
								}
							}
						}
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::BLUE_LIZARD)
						{
							BlueLizard* pGreenLizard = pTargetObject->FindComponentFromTag<BlueLizard>("BlueLizard");

							int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(0);
								}
							}
							else
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(ratio);
								}
							}
						}
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::ARMORED_GREEN_LIZARD)
						{
							Armored_GreenLizard* pGreenLizard = pTargetObject->FindComponentFromTag<Armored_GreenLizard>("Armored_GreenLizard");

							int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(0);
								}
							}
							else
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(ratio);
								}
							}
						}
						else if (static_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::ARMORED_BLUE_LIZARD)
						{
							Armored_BlueLizard* pGreenLizard = pTargetObject->FindComponentFromTag<Armored_BlueLizard>("Armored_GreenLizard");

							int hp = pGreenLizard->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pSeuteompi->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(0);
								}
							}
							else
							{
								float ratio = (float)hp / (float)pGreenLizard->getMaxHP();
								pGreenLizard->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(ratio);
								}
							}
						}

						else if (reinterpret_cast<sc_packet_attack_player*>(packet)->objectSetType == OBJECT_SET_TYPE::DEMONLORD)
						{
							DemonLord* pDemonLord = pTargetObject->FindComponentFromTag<DemonLord>("DemonLord");

							int hp = pDemonLord->getCurrentHP() - pPacket->damage;

							//CTransform* pTransform = pDemonLord->GetTransform();
							//Vector3 vPos = pTransform->GetWorldPos();
							//Vector3 vLook = pTransform->GetWorldAxis(AXIS_Z).Normalize();
							//vPos += vLook * 1.25f;
							//vPos.y += 0.95f;
							//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
							//SAFE_RELEASE(pTransform);

							if (hp < 0)
							{
								float ratio = (float)hp / (float)pDemonLord->getMaxHP();
								pDemonLord->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(0);
								}
							}
							else
							{
								float ratio = (float)hp / (float)pDemonLord->getMaxHP();
								pDemonLord->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
									pEnemyUIHearthBar->setLengthRatio(ratio);
								}
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
					if (reinterpret_cast<sc_packet_attack_player*>(packet)->objectSetType != OBJECT_SET_TYPE::DEMONLORD)
					{
						//DemonLord* pDemonLord = pGameObject->FindComponentFromTag<DemonLord>("DemonLord");
						//pDemonLord->changeAnimation();
						CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
						pAnimation->ChangeClip("Attack1");
						SAFE_RELEASE(pAnimation);
					}
					else
					{
						CAnimation* pAnimation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
						wstring temp_animation_name = reinterpret_cast<sc_packet_attack_player*>(packet)->animation_name;
						string animation_name = strconv(temp_animation_name);
						pAnimation->ChangeClip(animation_name);
						SAFE_RELEASE(pAnimation);
					}
				}
				SAFE_RELEASE(pGameObject);

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
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pUIHearthBar = GET_SINGLE(UserInterfaceManager)->getUIHeartBar();
									pUIHearthBar->setLengthRatio(0.f);
								}
							}
							else
							{
								float ratio = (float)hp / (float)pPlayer->getMaxHP();
								pPlayer->setCurrentHP(hp);
								if (myTargetID == pPacket->targetid)
								{
									CUIButton* pUIHearthBar = GET_SINGLE(UserInterfaceManager)->getUIHeartBar();
									pUIHearthBar->setLengthRatio(ratio);
								}
							}
						}
					}

				}
			}

		}
		break;

		case SC_PACKET_ATTACK_SKILL1_EFFECT:
		{
			sc_packet_attack_skill_player * pPacket = reinterpret_cast<sc_packet_attack_skill_player*>(packet);
			int id = pPacket->playerID;

			int myClientID = NetworkManager::getInstance()->getMyClientID();
			if (id == NetworkManager::getInstance()->getMyClientID())
			{
				string appendTag = to_string(myClientID);
				string objectTag = "Player" + appendTag;

				CGameObject* pPlayerObject = CGameObject::FindObject(objectTag);
				CTransform* pPlayerTr = pPlayerObject->GetTransform();
				double x = pPlayerTr->GetWorldPos().x;
				double y = pPlayerTr->GetWorldPos().y;
				double z = pPlayerTr->GetWorldPos().z;

				Vector3 vLook = pPlayerTr->GetWorldAxis(AXIS_Z).Normalize();
				x += (vLook * 1.75f).x;
				y += (vLook * 1.75f).y;
				z += (vLook * 1.75f).z;
				y += 0.95f;
				GET_SINGLE(CEffectManager)->OperateEffect("Attack3", nullptr, Vector3((float)x, (float)y, (float)z));

				SAFE_RELEASE(pPlayerTr);
				SAFE_RELEASE(pPlayerObject);
			}
			else if (id < NPC_START)
			{
				string appendTag = to_string(id);
				string objectTag = "Player" + appendTag;

				CGameObject* pPlayerObject = CGameObject::FindObject(objectTag);
				CTransform* pPlayerTr = pPlayerObject->GetTransform();
				Vector3 vPos = pPlayerTr->GetWorldPos();
				Vector3 vLook = pPlayerTr->GetWorldAxis(AXIS_Z).Normalize();
				vPos += vLook * 1.75f;
				vPos.y += 0.75f;
				GET_SINGLE(CEffectManager)->OperateEffect("Attack3", nullptr, vPos);

				SAFE_RELEASE(pPlayerTr);
				SAFE_RELEASE(pPlayerObject);
			}
			//	Vector3 vPos = m_pTransform->GetWorldPos();
			//	Vector3 vLook = m_pTransform->GetWorldAxis(AXIS_Z).Normalize();
			//	vPos += vLook * 1.75f;
			//	vPos.y += 0.75f;
			//	GET_SINGLE(CEffectManager)->OperateEffect("Attack3", nullptr, vPos);
			//	_cprintf("effect!\n");


		}
		break;

		case SC_PACKET_ATTACK_SKILL2_EFFECT:
		{
			sc_packet_attack_skill_player * pPacket = reinterpret_cast<sc_packet_attack_skill_player*>(packet);
			int id = pPacket->playerID;

			int myClientID = NetworkManager::getInstance()->getMyClientID();
			if (id == NetworkManager::getInstance()->getMyClientID())
			{
				string appendTag = to_string(myClientID);
				string objectTag = "Player" + appendTag;

				CGameObject* pPlayerObject = CGameObject::FindObject(objectTag);
				CTransform* pPlayerTr = pPlayerObject->GetTransform();
				double x = pPlayerTr->GetWorldPos().x;
				double y = pPlayerTr->GetWorldPos().y;
				double z = pPlayerTr->GetWorldPos().z;

				//Vector3 vLook = pPlayerTr->GetWorldAxis(AXIS_Z).Normalize();
				//x += (vLook * 1.75f).x;
				//y += (vLook * 1.75f).y;
				//z += (vLook * 1.75f).z;
				//y += 0.95f;
				GET_SINGLE(CEffectManager)->OperateEffect("Spell2", nullptr, Vector3((float)x, (float)y, (float)z));

				SAFE_RELEASE(pPlayerTr);
				SAFE_RELEASE(pPlayerObject);
			}
			else if (id < NPC_START)
			{
				string appendTag = to_string(id);
				string objectTag = "Player" + appendTag;

				CGameObject* pPlayerObject = CGameObject::FindObject(objectTag);
				CTransform* pPlayerTr = pPlayerObject->GetTransform();
				Vector3 vPos = pPlayerTr->GetWorldPos();
				//Vector3 vLook = pPlayerTr->GetWorldAxis(AXIS_Z).Normalize();
				//vPos += vLook * 1.75f;
				//vPos.y += 0.75f;
				GET_SINGLE(CEffectManager)->OperateEffect("Spell2", nullptr, vPos);

				SAFE_RELEASE(pPlayerTr);
				SAFE_RELEASE(pPlayerObject);
			}
			//	Vector3 vPos = m_pTransform->GetWorldPos();
			//	Vector3 vLook = m_pTransform->GetWorldAxis(AXIS_Z).Normalize();
			//	vPos += vLook * 1.75f;
			//	vPos.y += 0.75f;
			//	GET_SINGLE(CEffectManager)->OperateEffect("Attack3", nullptr, vPos);
			//	_cprintf("effect!\n");


		}
		break;

		case SC_PACKET_ATTACK_SKILL4:
		{
			sc_packet_attack_skill_player * pPacket = reinterpret_cast<sc_packet_attack_skill_player*>(packet);
			int id = pPacket->playerID;

			int myClientID = NetworkManager::getInstance()->getMyClientID();
			if (id == NetworkManager::getInstance()->getMyClientID())
			{
				string appendTag = to_string(myClientID);
				string objectTag = "Player" + appendTag;

				CGameObject* pPlayerObject = CGameObject::FindObject(objectTag);
				CTransform* pPlayerTr = pPlayerObject->GetTransform();
				Vector3 pos = pPlayerTr->GetWorldPos();
				GET_SINGLE(CEffectManager)->OperateEffect("Spell4", nullptr, pos);
				CAnimation* pAnimation = pPlayerObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
				pAnimation->ChangeClip("Spell4");
				SAFE_RELEASE(pAnimation);
				SAFE_RELEASE(pPlayerTr);
				SAFE_RELEASE(pPlayerObject);
			}
			else if (id < NPC_START)
			{
				string appendTag = to_string(id);
				string objectTag = "Player" + appendTag;

				CGameObject* pPlayerObject = CGameObject::FindObject(objectTag);
				CTransform* pPlayerTr = pPlayerObject->GetTransform();
				Vector3 pos = pPlayerTr->GetWorldPos();
				GET_SINGLE(CEffectManager)->OperateEffect("Spell4", nullptr, pos);
				CAnimation* pAnimation = pPlayerObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
				pAnimation->ChangeClip("Spell4");
				SAFE_RELEASE(pAnimation);
				SAFE_RELEASE(pPlayerTr);
				SAFE_RELEASE(pPlayerObject);
			}
			//	Vector3 vPos = m_pTransform->GetWorldPos();
			//	Vector3 vLook = m_pTransform->GetWorldAxis(AXIS_Z).Normalize();
			//	vPos += vLook * 1.75f;
			//	vPos.y += 0.75f;
			//	GET_SINGLE(CEffectManager)->OperateEffect("Attack3", nullptr, vPos);
			//	_cprintf("effect!\n");


		}
		break;
		//case SC_PACKET_ATTACK_SKILL1:
		//{
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
			//}
			//break;
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
				string appendTag = to_string(myClientID);
				string objectTag = "Player" + appendTag;

				CGameObject* pGameObject = CGameObject::FindObject(objectTag);
				if (pGameObject == nullptr)
					break;
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

		case SC_PACKET_HP_REGENERATION:
		{
			sc_packet_hp_regeneration* pPacket = reinterpret_cast<sc_packet_hp_regeneration*>(packet);
			int id = pPacket->targetID;
			int myClientID = NetworkManager::getInstance()->getMyClientID();
			if (id == myClientID)
			{
				string appendTag = to_string(myClientID);
				string objectTag = "Player" + appendTag;
				CGameObject* pGameObject = CGameObject::FindObject(objectTag);
				if (nullptr != pGameObject)
				{
					CPlayer* pPlayer = pGameObject->FindComponentFromTag<CPlayer>("Player");
					pPlayer->hpRegeneration(pPacket->hpregen);

					float ratio = (float)pPlayer->getCurrentHP() / (float)pPlayer->getMaxHP();
					CUIButton* pUIHearthBar = GET_SINGLE(UserInterfaceManager)->getUIHeartBar();
					pUIHearthBar->setLengthRatio(ratio);

					SAFE_RELEASE(pPlayer);
				}
			}
			else if (id < NPC_START)
			{
				string appendTag = to_string(id);
				string objectTag = "Player" + appendTag;
				CGameObject* pGameObject = CGameObject::FindObject(objectTag);
				if (nullptr != pGameObject)
				{
					CPlayer* pPlayer = pGameObject->FindComponentFromTag<CPlayer>("Player");
					pPlayer->hpRegeneration(pPacket->hpregen);
					SAFE_RELEASE(pPlayer);
				}
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
					SAFE_RELEASE(pGolem);
				}
				GreenLizard* pGreenLizard = pGameObject->FindComponentFromTag<GreenLizard>("GreenLizard");
				if (nullptr != pGreenLizard)
				{
					pGreenLizard->setDieState(true);
					SAFE_RELEASE(pGreenLizard);
				}
				BlueLizard* pBlueLizard = pGameObject->FindComponentFromTag<BlueLizard>("BlueLizard");
				if (nullptr != pBlueLizard)
				{
					pBlueLizard->setDieState(true);
					SAFE_RELEASE(pBlueLizard);
				}
				Armored_GreenLizard* pArmoredGreenLizard = pGameObject->FindComponentFromTag<Armored_GreenLizard>("Armored_GreenLizard");
				if (nullptr != pArmoredGreenLizard)
				{
					pArmoredGreenLizard->setDieState(true);
					SAFE_RELEASE(pArmoredGreenLizard);
				}
				Armored_BlueLizard* pArmoredBlueLizard = pGameObject->FindComponentFromTag<Armored_BlueLizard>("Armored_BlueLizard");
				if (nullptr != pArmoredBlueLizard)
				{
					pArmoredBlueLizard->setDieState(true);
					SAFE_RELEASE(pArmoredBlueLizard);
				}
				DemonLord* pDemonLord = pGameObject->FindComponentFromTag<DemonLord>("DemonLord");
				if (nullptr != pDemonLord)
				{
					pDemonLord->setDieState(true);
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
			string myappendTag = to_string(NetworkManager::getInstance()->getMyClientID());
			string myobjectTag = "Player" + myappendTag;
			CGameObject* pMyPlayerObject = CGameObject::FindObject(myobjectTag);
			if (pMyPlayerObject != nullptr)
			{
				CPlayer* pPlayer = pMyPlayerObject->FindComponentFromTag<CPlayer>("Player");
				if (id == pPlayer->clickedID)
					GET_SINGLE(UserInterfaceManager)->getDropTableUI()->clear();
				SAFE_RELEASE(pPlayer);
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
				pDropTableUI->addDropItemSlot(pPacket->itemname1 + (i * 100));
			}

			pDropTableUI->enableRender(true);
		}
		break;
		case SC_PACKET_ADDITEM_INVENTORY:
		{
			sc_packet_additem_inventory* pPacket = reinterpret_cast<sc_packet_additem_inventory*>(packet);
			wstring tempitemname = pPacket->itemname;

			CLayer*	pLayer = GET_SINGLE(CSceneManager)->GetCurrentScene()->GetLayer("UI+2");
			CGameObject* pItem = CGameObject::CreateObject("SWORD_ITEM", pLayer);

			CUIButton*	pItemUI = pItem->AddComponent<CUIButton>("SWORD_ITEM");
			SAFE_RELEASE(pItemUI);

			CTransform*	pItemTr = pItem->GetTransform();
			//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
			pItemTr->SetWorldScale(30.f, 30.f, 1.f);

			SAFE_RELEASE(pItemTr);
			CRenderer2D* pRenderer = pItem->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
			CMaterial* pMaterial = pRenderer->GetMaterial();
			tempitemname = L"ItemIcon/" + tempitemname + L".png";
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

		case SC_PACKET_CONNECT:
		{
			sc_packet_connect* pPacket = reinterpret_cast<sc_packet_connect*>(packet);
			int id = pPacket->id;

			if (false == NetworkManager::getInstance()->getIsConnected())
			{
				NetworkManager::getInstance()->enableConnected(true);
				NetworkManager::getInstance()->setMyClientID(id);
			}
		}
		break;
		case SC_PACKET_JOIN:
		{
			sc_packet_login* pPacket = reinterpret_cast<sc_packet_login*>(packet);
			if (pPacket->state == PACKETSTATE::PACKETSTATE_JOIN_SUCCESS)
			{
				MessageBox(NULL, L"가입에 성공하습니다.\n로그인 하십시오.", L"가입 성공", MB_OK);
			}
			else if (pPacket->state == PACKETSTATE::PACKETSTATE_JOIN_FAIL)
			{
				MessageBox(NULL, L"이미 해당 아이디가 존재합니다.", L"가입 실패", MB_OK);
			}

		}
		break;
		case SC_PACKET_LOGIN:
		{
			sc_packet_login* login_packet = reinterpret_cast<sc_packet_login*>(packet);

			// 로그인 성공시 씬 전환.
			if (PACKETSTATE::PACKETSTATE_LOGIN_SUCCESS == login_packet->state)
			{
				//GET_SINGLE(CScheduler)->DeleteSchedule("GameStart");

				CScene*	pScene = GET_SINGLE(CSceneManager)->CreateNextScene("MainScene");

				pScene->CreateSceneScript<CMainScene>("MainScene", false);

				//SAFE_RELEASE(pMainScene);

				SAFE_RELEASE(pScene);
			}
			else if (PACKETSTATE::PACKETSTATE_LOGIN_ID_FAIL == login_packet->state)
			{
				MessageBox(NULL, L"아이디가 존재하지 않습니다.", L"로그인 실패", MB_OK);
			}
			else if (PACKETSTATE::PACKETSTATE_LOGIN_PW_FAIL == login_packet->state)
			{
				MessageBox(NULL, L"비밀번호가 틀립니다.", L"로그인 실패", MB_OK);
			}
		};

		case SC_PACKET_EQUIPITEM_PLAYER:
		{
			sc_packet_equip_player* pPacket = reinterpret_cast<sc_packet_equip_player*>(packet);
			int id = pPacket->playerID;
			int myClientID = NetworkManager::getInstance()->getMyClientID();
			if (id == myClientID)
			{
				string appendTag = to_string(myClientID);
				string objectTag = "Player" + appendTag;
				CGameObject* pGameObject = CGameObject::FindObject(objectTag);
				if (nullptr != pGameObject)
				{
					CLayer*	pLayer = GET_SINGLE(CSceneManager)->GetCurrentScene()->GetLayer("Default");
					//int id = NetworkManager::getInstance()->getMyClientID();
					//string objectTag = "Player" + to_string(id);
					CGameObject *pSwordObj = CGameObject::CreateObject(objectTag + "Sword", pLayer);
					CSword	*pSword = pSwordObj->AddComponent<CSword>("Sword");
					pSword->setTargetPlayerID(id);
					pSword->initialize();
					SAFE_RELEASE(pSword);
					SAFE_RELEASE(pSwordObj);
					SAFE_RELEASE(pLayer);
				}
			}
			else if (id < NPC_START)
			{
				string appendTag = to_string(id);
				string objectTag = "Player" + appendTag;
				CGameObject* pGameObject = CGameObject::FindObject(objectTag);
				if (nullptr != pGameObject)
				{
					CLayer*	pLayer = GET_SINGLE(CSceneManager)->GetCurrentScene()->GetLayer("Default");
					//int id = NetworkManager::getInstance()->getMyClientID();
					//string objectTag = "Player" + to_string(id);
					CGameObject *pSwordObj = CGameObject::CreateObject(objectTag + "Sword", pLayer);
					CSword	*pSword = pSwordObj->AddComponent<CSword>("Sword");
					pSword->setTargetPlayerID(id);
					pSword->initialize();
					SAFE_RELEASE(pSword);
					SAFE_RELEASE(pSwordObj);
					SAFE_RELEASE(pLayer);
				}
			}

		}
		break;
		case SC_PACKET_ADD_EXP:
		{
			sc_packet_add_exp* pPacket = reinterpret_cast<sc_packet_add_exp*>(packet);
			int id = pPacket->targetID;
			int myClientID = NetworkManager::getInstance()->getMyClientID();
			if (id == myClientID)
			{
				string appendTag = to_string(myClientID);
				string objectTag = "Player" + appendTag;

				CGameObject* pPlayerObject = CGameObject::FindObject(objectTag);
				if (pPlayerObject != nullptr)
				{
					CPlayer* pPlayer = pPlayerObject->FindComponentFromTag<CPlayer>("Player");
					pPlayer->addExp(pPacket->exp);

					Status* pStatus = GET_SINGLE(UserInterfaceManager)->getStatus();

					float ratio = (float)(pPlayer->getEXP()) / (float)(pPlayer->getMaxEXP());
					//pEnemyUIHearthBar->

					pStatus->getUIPureBar()->setLengthRatio(ratio);
					SAFE_RELEASE(pPlayer);

				}


			}
			else if (id < MAX_USER)
			{
				string appendTag = to_string(id);
				string objectTag = "Player" + appendTag;

				CGameObject* pPlayerObject = CGameObject::FindObject(objectTag);
				if (pPlayerObject != nullptr)
				{
					CPlayer* pPlayer = pPlayerObject->FindComponentFromTag<CPlayer>("Player");
					pPlayer->addExp(pPacket->exp);
					SAFE_RELEASE(pPlayer);
				}
			}
		}
		break;
		
		case SC_PACKET_LEVELUP:
		{
			sc_packet_levelup_player* pPacket = reinterpret_cast<sc_packet_levelup_player*>(packet);
			int id = pPacket->targetID;
			int myClientID = NetworkManager::getInstance()->getMyClientID();
			if (id == myClientID)
			{
				string appendTag = to_string(myClientID);
				string objectTag = "Player" + appendTag;

				CGameObject* pPlayerObject = CGameObject::FindObject(objectTag);
				if (pPlayerObject != nullptr)
				{
					CPlayer* pPlayer = pPlayerObject->FindComponentFromTag<CPlayer>("Player");
					/*
							levelupPacket.exp = 0;
		levelupPacket.level = this->level;
		levelupPacket.current_hp = this->currentHP;
		levelupPacket.current_mp = this->currentMP;
		levelupPacket.max_hp = this->maxHP;
		levelupPacket.max_mp = this->maxMP;
		levelupPacket.attack_damage = this->attackDamage;
					*/
					pPlayer->setEXP(0);
					pPlayer->setLevel(pPacket->level);
					pPlayer->setCurrentHP(pPacket->current_hp);
					pPlayer->setCurrentMP(pPacket->current_mp);
					pPlayer->setMaxHP(pPacket->max_hp);
					pPlayer->setMaxMP(pPacket->max_mp);
					pPlayer->setAttackDamag(pPacket->attack_damage);
					pPlayer->setMaxEXP(pPacket->level * 100);
					Status* pStatus = GET_SINGLE(UserInterfaceManager)->getStatus();

					float ratio = (float)(pPlayer->getEXP()) / (float)(pPlayer->getMaxEXP());
					//pEnemyUIHearthBar->
					CTransform* pTr = pPlayerObject->GetTransform();
					Vector3 pos = pTr->GetWorldPos();
					GET_SINGLE(CEffectManager)->OperateEffect("Level_Up", nullptr, pos);
					SAFE_RELEASE(pTr);
					pStatus->getUIPureBar()->setLengthRatio(ratio);
					SAFE_RELEASE(pPlayer);

				}

			}
			else if (id < MAX_USER)
			{
				string appendTag = to_string(id);
				string objectTag = "Player" + appendTag;

				CGameObject* pPlayerObject = CGameObject::FindObject(objectTag);
				if (pPlayerObject != nullptr)
				{
					CPlayer* pPlayer = pPlayerObject->FindComponentFromTag<CPlayer>("Player");
					pPlayer->setEXP(0);
					pPlayer->setLevel(pPacket->level);
					pPlayer->setCurrentHP(pPacket->current_hp);
					pPlayer->setCurrentMP(pPacket->current_mp);
					pPlayer->setMaxHP(pPacket->max_hp);
					pPlayer->setMaxMP(pPacket->max_mp);
					pPlayer->setAttackDamag(pPacket->attack_damage);
					pPlayer->setMaxEXP(pPacket->level * 100);
					//pPlayer->addExp(pPacket->exp);
					CTransform* pTr = pPlayerObject->GetTransform();
					Vector3 pos = pTr->GetWorldPos();
					GET_SINGLE(CEffectManager)->OperateEffect("Level_Up", nullptr, pos);

					int hp = pPlayer->getCurrentHP();
					float ratio = (float)hp / (float)pPlayer->getMaxHP();
					string testdebug = "level : " + to_string(pPlayer->getLevel()) +  " / hp : " + to_string(hp) + " / maxhp : " + to_string(pPlayer->getMaxHP()) + "\n";
					OutputDebugStringA(testdebug.c_str());
					pPlayer->setCurrentHP(hp);

					{
						PartyStatus* pStatus = GET_SINGLE(UserInterfaceManager)->findPartyState(objectTag);
						if (pStatus != nullptr)
						{
							CUIButton* pUIHearthBar = pStatus->getUIHearthBar();
							pUIHearthBar->setLengthRatio(ratio);
						}
					}

					SAFE_RELEASE(pTr);
					SAFE_RELEASE(pPlayer);
				}
			}
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
		case SC_PACKET_PARTY_SYNCRONIZE:
		{
			sc_packet_put_player* pPacket = reinterpret_cast<sc_packet_put_player*>(packet);
			int id = pPacket->id;
			OBJECT_SET_TYPE ObjectSetType = (OBJECT_SET_TYPE)static_cast<sc_packet_put_player*>(packet)->objectSetType;

			if (id == NetworkManager::getInstance()->getMyClientID())
			{
				char str[128];
				string appendTag = _itoa(id, str, 10);
				string objectTag = "Player" + appendTag;

				CGameObject* pPlayerObj = CGameObject::FindObject(objectTag);
				if (nullptr != pPlayerObj)
				{
					CTransform*	pTr = pPlayerObj->GetTransform();

					float yPos = GET_SINGLE(CQuadTreeManager)->GetY(Vector3(pPacket->x, pPacket->y, pPacket->z));
					pTr->SetWorldPos(pPacket->x, yPos, pPacket->z);
					SAFE_RELEASE(pTr);

					CPlayer*	pPlayer = pPlayerObj->FindComponentFromTag<CPlayer>("Player");
					pPlayer->setCurrentHP(pPacket->current_hp);

					Status* pStatus = GET_SINGLE(UserInterfaceManager)->getStatus();

					float ratio = (float)(pPlayer->getEXP()) / (float)(pPlayer->getMaxEXP());
					pStatus->getUIPureBar()->setLengthRatio(ratio);
					//SAFE_RELEASE(pPlayer);
				}
			}
			else if (id < NPC_START)
			{
				char str[128];
				string appendTag = _itoa(pPacket->id, str, 10);
				string objectTag = "Player" + appendTag;

				CLayer*	pLayer = GET_SINGLE(CSceneManager)->GetCurrentScene()->GetLayer("Default");
				CGameObject* pPlayerObj = CGameObject::FindObject(objectTag);
				if (nullptr != pPlayerObj)
				{
					CPlayer*	pPlayer = pPlayerObj->FindComponentFromTag<CPlayer>("Player");
					pPlayer->setCurrentHP(pPacket->current_hp);
					//

					CTransform*	pTr = pPlayerObj->GetTransform();
					float yPos = GET_SINGLE(CQuadTreeManager)->GetY(Vector3(pPacket->x, pPacket->y, pPacket->z));
					pTr->SetWorldPos(pPacket->x, yPos, pPacket->z);
					SAFE_RELEASE(pTr);

					int hp = pPlayer->getCurrentHP();
					float ratio = (float)hp / (float)pPlayer->getMaxHP();
					pPlayer->setCurrentHP(hp);
					string debugstring = "hp : " + to_string(hp) + " MaxHP : " + to_string(pPlayer->getMaxHP()) + "\n";
					OutputDebugStringA(debugstring.c_str());
					{
						PartyStatus* pStatus = GET_SINGLE(UserInterfaceManager)->findPartyState(objectTag);
						if (pStatus != nullptr)
						{
							CUIButton* pUIHearthBar = pStatus->getUIHearthBar();
							pUIHearthBar->setLengthRatio(ratio);
						}
					}
					//SAFE_RELEASE(pPlayer);
				}
			}


		}
		break;

		default:
			break;
		}
		//NetworkManager::getInstance()->popData();
		free(packet);
	}


}

void NetworkManager::readPacket(SOCKET sock)
{
	DWORD iobyte, ioflag = 0;

	//int ret = recv(sock, recvBuffer, MAX_BUFF_SIZE, 0);
	int ret = WSARecv(sock, &recv_wsabuf, 1, &iobyte, &ioflag, NULL, NULL);

	if (ret)
	{
		int err_code = WSAGetLastError();
		// 에러처리.
	}

	BYTE* ptr = reinterpret_cast<BYTE*>(recvBuffer);
	//(int)((unsigned short*)ptr)[0]
	while (0 != iobyte)
	{
		if (0 == inPacketSize)
			inPacketSize = ((unsigned short*)ptr)[0];
		if (iobyte + savedPacketSize >= inPacketSize)
		{
			memcpy(packetBuffer + savedPacketSize, ptr, inPacketSize - savedPacketSize);

			processPacket(packetBuffer);

			ptr += inPacketSize - savedPacketSize;
			iobyte -= inPacketSize - savedPacketSize;

			inPacketSize = 0;
			savedPacketSize = 0;
		}
		else
		{
			memcpy(packetBuffer + savedPacketSize, ptr, iobyte);
			savedPacketSize += iobyte;
			iobyte = 0;
		}
	}
}

const SOCKET & NetworkManager::getSocket() { return sock; }
char * NetworkManager::getSendBuffer() { return sendBuffer; }
const char * NetworkManager::getRecvBuffer() { return recvBuffer; }
const char * NetworkManager::getPacketBuffer() { return packetBuffer; }
const DWORD NetworkManager::getInPacketSize() { return inPacketSize; }
const int NetworkManager::getSavedPacketSize() { return savedPacketSize; }
const size_t NetworkManager::getMyClientID() { return myClientID; }
const bool NetworkManager::getIsConnected() { return isConnected; }

WSABUF & NetworkManager::getSendWsaBuf()
{
	return this->send_wsabuf;
}

WSABUF & NetworkManager::getRecvWsaBuf()
{
	return this->recv_wsabuf;
}

void NetworkManager::enableConnected(bool connected)
{
	this->isConnected = connected;
}
void NetworkManager::setMyClientID(size_t id)
{
	this->myClientID = id;
}

//void NetworkManager::recvThread()
//{
//	SOCKET sock = NetworkManager::getInstance()->getSocket();
//
//	while (1)
//	{
//		NetworkManager::getInstance()->readPacket(sock);
//	}
//}
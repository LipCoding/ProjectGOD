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
#include "MFObjectManager.h"
#include "ObjectScript/Villager1.h"
#include "QuestManager.h"
#include "QuestItem.h"
#include "SceneScript/MainScene.h"
queue<void*> NetworkManager::clientPacketQueue;
CGameObject* NetworkManager::pPlayer;
NetworkManager::NetworkManager() {}

NetworkManager::~NetworkManager()
{
}

bool NetworkManager::initialize()
{
	string path = GET_SINGLE(CPathManager)->FindPathToMultiByte(DATA_PATH);
	path += "serverip.txt";
	ifstream in(path.c_str());
	in >> serverip_address;

	inPacketSize = 0;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

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
		MessageBox(NULL, L"�������� ���ῡ �����Ͽ����ϴ�.", L"��������", MB_OK);

	return true;
}

void NetworkManager::shutDownServer()
{
	closesocket(sock);
	WSACleanup();
}

void NetworkManager::processPacket(void * packet)
{
	unsigned char packet_type = reinterpret_cast<unsigned char*>(packet)[2];

	switch (packet_type)
	{
#pragma region Object
	case SC_PACKET_PUT_PLAYER:
	{
		sc_packet_put_player* pPacket = reinterpret_cast<sc_packet_put_player*>(packet);
		OBJECT_SET_TYPE ObjectSetType = (OBJECT_SET_TYPE)static_cast<sc_packet_put_player*>(packet)->objectSetType;

		string appendTag = to_string(pPacket->id);
		string objectTag = "Player" + appendTag;

		if (static_cast<sc_packet_put_player*>(packet)->objectSetType == OBJECT_SET_TYPE::PLAYER)
		{
			const auto& player_component = MFObjectManager::getInstance()->CreateActor<CPlayer>("Default", objectTag, "PlayerCharacter", true);
			player_component->settingStatus(pPacket->current_hp, pPacket->current_mp, pPacket->level, pPacket->exp);
			player_component->move(pPacket->x, pPacket->y, pPacket->z);

			if (pPacket->id == NetworkManager::getInstance()->getMyClientID())
			{
				CTransform* pTr = player_component->GetTransform();
				GET_SINGLE(SoundManager)->SetPlayerTr(pTr);
				GET_SINGLE(UserInterfaceManager)->setCameraTarget(player_component->GetGameObject());
				NetworkManager::getInstance()->setPlayer(player_component->GetGameObject());
				GET_SINGLE(UserInterfaceManager)->setPlayer(player_component);
				
			}
			Actor* pActor = player_component->FindComponentFromTypeName<Actor>("Actor");
			if (pPacket->id < NPC_START)
				static_cast<CPlayer*>(pActor)->changeAnimation("Idle1");
			else
				pActor->changeAnimation("Idle3");
		}
		else if (static_cast<sc_packet_put_player*>(packet)->objectSetType == OBJECT_SET_TYPE::NPC1)
		{
			const auto& player_component = MFObjectManager::getInstance()->CreateActor<Villager1>("Default", objectTag, "Villager1", true);
			//player_component->settingStatus(pPacket->current_hp, pPacket->current_mp, pPacket->level, pPacket->exp);
			player_component->move(pPacket->x, pPacket->y, pPacket->z);


		}
		else if (static_cast<sc_packet_put_player*>(packet)->objectSetType == OBJECT_SET_TYPE::GOLEM)
		{
			const auto& player_component = MFObjectManager::getInstance()->CreateActor<Golem>("Default", objectTag, "GolemObject", true);
			player_component->settingStatus(pPacket->current_hp, pPacket->current_mp, pPacket->level, pPacket->exp);
			player_component->move(pPacket->x, pPacket->y, pPacket->z);

			Actor* pActor = player_component->FindComponentFromTypeName<Actor>("Actor");
			if (pPacket->id < NPC_START)
				static_cast<CPlayer*>(pActor)->changeAnimation("Idle1");
			else
				pActor->changeAnimation("Idle3");
		}
		else if (static_cast<sc_packet_put_player*>(packet)->objectSetType == OBJECT_SET_TYPE::MINO)
		{
			const auto& player_component = MFObjectManager::getInstance()->CreateActor<Mino>("Default", objectTag, "MinoObject", true);
			player_component->settingStatus(pPacket->current_hp, pPacket->current_mp, pPacket->level, pPacket->exp);
			player_component->move(pPacket->x, pPacket->y, pPacket->z);

			Actor* pActor = player_component->FindComponentFromTypeName<Actor>("Actor");
			if (pPacket->id < NPC_START)
				static_cast<CPlayer*>(pActor)->changeAnimation("Idle1");
			else
				pActor->changeAnimation("Idle3");
		}
		else if (static_cast<sc_packet_put_player*>(packet)->objectSetType == OBJECT_SET_TYPE::GREEN_LIZARD)
		{
			const auto& player_component = MFObjectManager::getInstance()->CreateActor<GreenLizard>("Default", objectTag, "GreenLizard", true);
			player_component->settingStatus(pPacket->current_hp, pPacket->current_mp, pPacket->level, pPacket->exp);
			player_component->move(pPacket->x, pPacket->y, pPacket->z);

			Actor* pActor = player_component->FindComponentFromTypeName<Actor>("Actor");
			if (pPacket->id < NPC_START)
				static_cast<CPlayer*>(pActor)->changeAnimation("Idle1");
			else
				pActor->changeAnimation("Idle3");
		}
		else if (static_cast<sc_packet_put_player*>(packet)->objectSetType == OBJECT_SET_TYPE::BLUE_LIZARD)
		{
			const auto& player_component = MFObjectManager::getInstance()->CreateActor<BlueLizard>("Default", objectTag, "BlueLizard", true);
			player_component->settingStatus(pPacket->current_hp, pPacket->current_mp, pPacket->level, pPacket->exp);
			player_component->move(pPacket->x, pPacket->y, pPacket->z);

			Actor* pActor = player_component->FindComponentFromTypeName<Actor>("Actor");
			if (pPacket->id < NPC_START)
				static_cast<CPlayer*>(pActor)->changeAnimation("Idle1");
			else
				pActor->changeAnimation("Idle3");
		}
		else if (static_cast<sc_packet_put_player*>(packet)->objectSetType == OBJECT_SET_TYPE::ARMORED_GREEN_LIZARD)
		{
			const auto& player_component = MFObjectManager::getInstance()->CreateActor<Armored_GreenLizard>("Default", objectTag, "Armored_GreenLizard", true);
			player_component->settingStatus(pPacket->current_hp, pPacket->current_mp, pPacket->level, pPacket->exp);
			player_component->move(pPacket->x, pPacket->y, pPacket->z);

			Actor* pActor = player_component->FindComponentFromTypeName<Actor>("Actor");
			if (pPacket->id < NPC_START)
				static_cast<CPlayer*>(pActor)->changeAnimation("Idle1");
			else
				pActor->changeAnimation("Idle3");
		}
		else if (static_cast<sc_packet_put_player*>(packet)->objectSetType == OBJECT_SET_TYPE::ARMORED_BLUE_LIZARD)
		{
			const auto& player_component = MFObjectManager::getInstance()->CreateActor<Armored_BlueLizard>("Default", objectTag, "Armored_BlueLizard", true);
			player_component->settingStatus(pPacket->current_hp, pPacket->current_mp, pPacket->level, pPacket->exp);
			player_component->move(pPacket->x, pPacket->y, pPacket->z);

			Actor* pActor = player_component->FindComponentFromTypeName<Actor>("Actor");
			if (pPacket->id < NPC_START)
				static_cast<CPlayer*>(pActor)->changeAnimation("Idle1");
			else
				pActor->changeAnimation("Idle3");
		}
		else if (static_cast<sc_packet_put_player*>(packet)->objectSetType == OBJECT_SET_TYPE::DEMONLORD)
		{
			const auto& player_component = MFObjectManager::getInstance()->CreateActor<DemonLord>("Default", objectTag, "DemonLordObjeect", true);
			player_component->settingStatus(pPacket->current_hp, pPacket->current_mp, pPacket->level, pPacket->exp);
			player_component->move(pPacket->x, pPacket->y, pPacket->z);

			Actor* pActor = player_component->FindComponentFromTypeName<Actor>("Actor");
			if (pPacket->id < NPC_START)
				static_cast<CPlayer*>(pActor)->changeAnimation("Idle1");
			else
				pActor->changeAnimation("Idle3");
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
			CPlayer* pPlayer = pMyPlayerObject->FindComponentFromTypeName<CPlayer>("Actor");
			if (id == pPlayer->clickedID)
				GET_SINGLE(UserInterfaceManager)->getDropTableUI()->clear();
			SAFE_RELEASE(pPlayer);
		}
	}
	break;
#pragma endregion

#pragma region Actor Transform
	case SC_PACKET_MOVE:
	{
		sc_packet_pos* pPacket = reinterpret_cast<sc_packet_pos*>(packet);

		string appendTag = to_string(pPacket->id);
		string objectTag = "Player" + appendTag;

		CGameObject* pGameObject = CGameObject::FindObject(objectTag);
		if (pGameObject == nullptr)
			break;

		Actor* pActor = pGameObject->FindComponentFromTypeName<Actor>("Actor");
		pActor->move(pPacket->x, pPacket->y, pPacket->z, pPacket->back);
		//SAFE_RELEASE(pGameObject);

		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		if(pPacket->id == NetworkManager::getInstance()->myClientID)
			pScene->GetLightCamera()->SetLightCenterPosToObject(pGameObject);


	}
	break;
	case SC_PACKET_MOVE_LEFT:
	{
		sc_packet_pos* pPacket = reinterpret_cast<sc_packet_pos*>(packet);

		string appendTag = to_string(pPacket->id);
		string objectTag = "Player" + appendTag;

		CGameObject* pGameObject = CGameObject::FindObject(objectTag);
		if (pGameObject == nullptr)
			break;

		Actor* pActor = pGameObject->FindComponentFromTypeName<Actor>("Actor");
		pActor->move(pPacket->x, pPacket->y, pPacket->z, pPacket->back);

		pActor->changeAnimation("RunRight");
		SAFE_RELEASE(pGameObject);


	}
	break;

	case SC_PACKET_ROTATE_CAMERA_PLAYER:
	{
		sc_packet_rotate_camera_player* pPacket = reinterpret_cast<sc_packet_rotate_camera_player*>(packet);

		string appendTag = to_string(pPacket->id);
		string objectTag = "Player" + appendTag;

		CGameObject* pGameObject = CGameObject::FindObject(objectTag);
		if (pGameObject == nullptr)
			break;

		CTransform* pTransform = pGameObject->GetTransform();
		Vector3 pos = pTransform->GetWorldPos();
		Vector3 axis = Vector3{ pPacket->axis_x, pPacket->axis_y, pPacket->axis_z };
		pTransform->LookAt(pos + axis * 1.5f);
		//				m_pTransform->LookAt(vPos + vCamAxisZ * 1.5f);
	}
		break;

	case SC_PACKET_MOVE_RIGHT:
	{
		sc_packet_pos* pPacket = reinterpret_cast<sc_packet_pos*>(packet);

		string appendTag = to_string(pPacket->id);
		string objectTag = "Player" + appendTag;

		CGameObject* pGameObject = CGameObject::FindObject(objectTag);
		if (pGameObject == nullptr)
			break;

		Actor* pActor = pGameObject->FindComponentFromTypeName<Actor>("Actor");
		pActor->move(pPacket->x, pPacket->y, pPacket->z, pPacket->back);
		pActor->changeAnimation("RunLeft");
		SAFE_RELEASE(pGameObject);

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

	case SC_PACKET_ROTATE_X:
	case SC_PACKET_ROTATE_Y:
	case SC_PACKET_ROTATE_Z:
	{
		sc_packet_rotate* pPacket = reinterpret_cast<sc_packet_rotate*>(packet);
		{
			string appendTag = to_string(pPacket->id);
			string objectTag = "Player" + appendTag;

			CGameObject* actor_object = CGameObject::FindObject(objectTag);
			if (nullptr == actor_object)
				break;

			CPlayer* player_component = actor_object->FindComponentFromTypeName<CPlayer>("Actor");
			player_component->rotate(0.f, pPacket->angle, 0.f);
			SAFE_RELEASE(player_component);
		}
	}
	break;
	case SC_PACKET_ROTATE_X_CI:
	case SC_PACKET_ROTATE_Y_CI:
	case SC_PACKET_ROTATE_Z_CI:
	{
	}
	break;
#pragma endregion

#pragma region Actor Animation
	case SC_PACKET_ATTACK_PLAYER:
	{
		sc_packet_attack_player* pPacket = reinterpret_cast<sc_packet_attack_player*>(packet);

		string appendTag = to_string(pPacket->id);
		string objectTag = "Player" + appendTag;

		string target_appendTag = to_string(pPacket->targetid);
		string target_objectTag = "Player" + target_appendTag;

		CGameObject* attacksubject_object = CGameObject::FindObject(objectTag);

		if (nullptr == attacksubject_object)
			break;
		
		if (nullptr != attacksubject_object->FindComponentFromTypeID<DemonLord>())
		{
			Actor* attacksubject_actor_component = attacksubject_object->FindComponentFromTypeName<Actor>("Actor");
			attacksubject_actor_component->attack_animation(target_objectTag, pPacket->animation_name);
		}
		else
		{
			Actor* attacksubject_actor_component = attacksubject_object->FindComponentFromTypeName<Actor>("Actor");
			attacksubject_actor_component->attack(target_objectTag);
		}


	}
	break;

	case SC_PACKET_ATTACK_SKILL1:
	{
		sc_packet_attack_player* pPacket = reinterpret_cast<sc_packet_attack_player*>(packet);
		string appendTag = to_string(pPacket->id);
		string objectTag = "Player" + appendTag;

		string target_appendTag = to_string(pPacket->targetid);
		string target_objectTag = "Player" + target_appendTag;

		CGameObject* attacksubject_object = CGameObject::FindObject(objectTag);
		if (nullptr == attacksubject_object)
			break;
		CGameObject* target_object = CGameObject::FindObject(target_objectTag);
		if (nullptr == target_object)
			break;

		CPlayer* attacksubject_player_component = attacksubject_object->FindComponentFromTypeName<CPlayer>("Actor");
		Actor* target_actor_component = target_object->FindComponentFromTypeName<Actor>("Actor");
		attacksubject_player_component->skill1_Attack(target_objectTag);

		GET_SINGLE(SoundManager)->Play("SwordAttack", SC_EFFECT);
	}
	break;

	case SC_PACKET_ATTACK_SKILL2:
	{
		sc_packet_attack_player* pPacket = reinterpret_cast<sc_packet_attack_player*>(packet);
		string appendTag = to_string(pPacket->id);
		string objectTag = "Player" + appendTag;

		string target_appendTag = to_string(pPacket->targetid);
		string target_objectTag = "Player" + target_appendTag;

		CGameObject* attacksubject_object = CGameObject::FindObject(objectTag);
		if (nullptr == attacksubject_object)
			break;
		CGameObject* target_object = CGameObject::FindObject(target_objectTag);
		if (nullptr == target_object)
			break;

		CPlayer* attacksubject_player_component = attacksubject_object->FindComponentFromTypeName<CPlayer>("Actor");
		Actor* target_actor_component = target_object->FindComponentFromTypeName<Actor>("Actor");
		attacksubject_player_component->skill2_Attack(target_objectTag);
	}
	break;

	case SC_PACKET_ATTACK_SKILL3:
	{
		sc_packet_attack_player* pPacket = reinterpret_cast<sc_packet_attack_player*>(packet);
		string appendTag = to_string(pPacket->id);
		string objectTag = "Player" + appendTag;

		string target_appendTag = to_string(pPacket->targetid);
		string target_objectTag = "Player" + target_appendTag;

		CGameObject* attacksubject_object = CGameObject::FindObject(objectTag);
		if (nullptr == attacksubject_object)
			break;

		CPlayer* attacksubject_player_component = attacksubject_object->FindComponentFromTypeName<CPlayer>("Actor");
		attacksubject_player_component->skill3_Attack(target_objectTag);

		GET_SINGLE(SoundManager)->Play("firecircle_first", SC_EFFECT);
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
		int sound_index = rand() % 4;
		if (0 == sound_index)
			GET_SINGLE(SoundManager)->Play("SwordAttack", SC_EFFECT);
		else if (1 == sound_index)
			GET_SINGLE(SoundManager)->Play("SwordAttack1", SC_EFFECT);
		else if (2 == sound_index)
			GET_SINGLE(SoundManager)->Play("SwordAttack2", SC_EFFECT);
		else if (2 == sound_index)
			GET_SINGLE(SoundManager)->Play("SwordAttack3", SC_EFFECT);

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

			GET_SINGLE(CEffectManager)->OperateEffect("Spell2", pPlayerObject, Vector3{0,0,0});

			SAFE_RELEASE(pPlayerTr);
			SAFE_RELEASE(pPlayerObject);
		}
		else if (id < NPC_START)
		{
			string appendTag = to_string(id);
			string objectTag = "Player" + appendTag;

			CGameObject* pPlayerObject = CGameObject::FindObject(objectTag);
			if (pPlayerObject != nullptr)
			{
				CTransform* pPlayerTr = pPlayerObject->GetTransform();
				Vector3 vPos = pPlayerTr->GetWorldPos();
				GET_SINGLE(CEffectManager)->OperateEffect("Spell2", nullptr, vPos);

				SAFE_RELEASE(pPlayerTr);
				SAFE_RELEASE(pPlayerObject);
			}
		}
		int sound_index = rand() % 4;
		if (0 == sound_index)
			GET_SINGLE(SoundManager)->Play("SwordAttack", SC_EFFECT);
		else if (1 == sound_index)
			GET_SINGLE(SoundManager)->Play("SwordAttack1", SC_EFFECT);
		else if (2 == sound_index)
			GET_SINGLE(SoundManager)->Play("SwordAttack2", SC_EFFECT);
		else if (2 == sound_index)
			GET_SINGLE(SoundManager)->Play("SwordAttack3", SC_EFFECT);
	}
	break;

	case SC_PACKET_ATTACK_SKILL4:
	{

		sc_packet_attack_skill_player * pPacket = reinterpret_cast<sc_packet_attack_skill_player*>(packet);
		int id = pPacket->playerID;

		string appendTag = to_string(id);
		string objectTag = "Player" + appendTag;

		CGameObject* pPlayerObject = CGameObject::FindObject(objectTag);
		CTransform* pPlayerTr = pPlayerObject->GetTransform();
		Vector3 pos = pPlayerTr->GetWorldPos();
		GET_SINGLE(CEffectManager)->OperateEffect("Spell4", nullptr, pos);
		GET_SINGLE(SoundManager)->Play("Tanker_Buff_FireCircle ", SC_EFFECT);
		CAnimation* pAnimation = pPlayerObject->FindComponentFromType<CAnimation>(CT_ANIMATION);
		pAnimation->ChangeClip("Spell4");


		GET_SINGLE(CEffectManager)->OperateEffect("Holy_Light", pPlayerObject, pos, true, false);
		SAFE_RELEASE(pAnimation);
		SAFE_RELEASE(pPlayerTr);
		SAFE_RELEASE(pPlayerObject);


	}
	break;
	case SC_PACKET_IDLE_ANIMATION:
	{
		sc_packet_animation_player* pPacket = reinterpret_cast<sc_packet_animation_player*>(packet);

		string appendTag = to_string(pPacket->id);
		string objectTag = "Player" + appendTag;
		SetWindowTextA(GET_SINGLE(CCore)->GetWindowHandle(), objectTag.c_str());
		CGameObject* pGameObject = CGameObject::FindObject(objectTag);
		if (pGameObject == nullptr)
			break;

		Actor* pActor = pGameObject->FindComponentFromTypeName<Actor>("Actor");
		if (pPacket->id < NPC_START)
			static_cast<CPlayer*>(pActor)->changeAnimation("Idle1");
		else
			pActor->changeAnimation("Idle3");



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
			if (id < NPC_START)
			{
				CPlayer* pActor = (CPlayer*)pGameObject->FindComponentFromTypeName<Actor>("Actor");
				pActor->changeAnimation("Die1");
			}
			else
			{
				Monster* monster_component = pGameObject->FindComponentFromTag<Monster>("Actor");
				monster_component->setDieState(true);
				GET_SINGLE(SoundManager)->Play("monster_death", SC_EFFECT);
			}
		}
	}
	break;
#pragma endregion


	case SC_PACKET_HP_REGENERATION:
	{
		sc_packet_hp_regeneration* pPacket = reinterpret_cast<sc_packet_hp_regeneration*>(packet);
		string appendTag = to_string(pPacket->targetID);
		string objectTag = "Player" + appendTag;
		CGameObject* pGameObject = CGameObject::FindObject(objectTag);
		if (nullptr != pGameObject)
		{
			CPlayer* pPlayer = pGameObject->FindComponentFromTypeName<CPlayer>("Actor");
			pPlayer->hpRegeneration(pPacket->hpregen);
			SAFE_RELEASE(pPlayer);
		}
	}
	break;

	case SC_PACKET_ADD_EXP:
	{
		sc_packet_add_exp* pPacket = reinterpret_cast<sc_packet_add_exp*>(packet);
		string appendTag = to_string(pPacket->targetID);
		string objectTag = "Player" + appendTag;

		CGameObject* pPlayerObject = CGameObject::FindObject(objectTag);
		if (pPlayerObject != nullptr)
		{
			CPlayer* pPlayer = pPlayerObject->FindComponentFromTypeName<CPlayer>("Actor");
			pPlayer->addExp(pPacket->exp);
			SAFE_RELEASE(pPlayer);
		}
	}
	break;

	case SC_PACKET_LEVELUP:
	{
		sc_packet_levelup_player* pPacket = reinterpret_cast<sc_packet_levelup_player*>(packet);

		string appendTag = to_string(myClientID);
		string objectTag = "Player" + appendTag;

		CGameObject* pPlayerObject = CGameObject::FindObject(objectTag);
		if (pPlayerObject != nullptr)
		{
			CPlayer* pPlayer = pPlayerObject->FindComponentFromTypeName<CPlayer>("Actor");
			pPlayer->settingStatus(pPacket->current_hp, pPacket->current_mp, pPacket->level, 0);
			
			Status* pStatus = GET_SINGLE(UserInterfaceManager)->getStatus();
			CTransform* pTr = pPlayerObject->GetTransform();
			Vector3 pos = pTr->GetWorldPos();
			GET_SINGLE(CEffectManager)->OperateEffect("Level_Up", nullptr, pos);
			GET_SINGLE(SoundManager)->Play("LevelUp", SC_EFFECT);
			SAFE_RELEASE(pTr);
			SAFE_RELEASE(pPlayer);
		}
	}
	break;

	case SC_PACKET_QUEST_ADD:
	{
		// ��Ŷ���κ��� �޾ƿ� ����Ʈ�� ã�´�.
		sc_packet_quest_add* pPacket = reinterpret_cast<sc_packet_quest_add*>(packet);
		string quest_name = strconv(pPacket->qeustname);
		Quest* quest = QuestManager::getInstance()->findQuest(quest_name.c_str());

		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer*	pLayer = pScene->GetLayer("UI_QUEST+2");

		CGameObject* QuestItemObject = CGameObject::CreateObject("QuestItem", pLayer);
		
		// ���ڿ��� �޾Ƽ� �������Ѵ�.
		//
		//
		QuestItemObject->Enable(false);
		QuestItem* quest_item_component = QuestItemObject->AddComponent<QuestItem>("QuestItem");
		quest_item_component->setQuest(quest);

		QuestBaseUI* quest_ui_component = QuestManager::getInstance()->getQuestUIComponent();
		quest_item_component->setQuestBaseUI(quest_ui_component);
		CRenderer2D* pRenderer = QuestItemObject->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "QuestItem",
			0, 0, L"UserInterface/UI_INVEN_BOX_1.png");

		int offset = quest_ui_component->getQuestList().size();

		CTransform* pDropTableTr = QuestItemObject->GetTransform();
		pDropTableTr->SetWorldScale(400, 50.f, 1.f);
		pDropTableTr->SetWorldPos(175.f, 180.f + (offset * 55), 1.f);
		wstring temp_text = strconv(quest->getQuestSubject());
		CFont* quest_item_text = QuestItemObject->AddComponent<CFont>("quest_ui_text_contents");
		quest_item_text->SetFont("�������N");
		quest_item_text->SetArea(50, 15.f, 680, 400.f);
		quest_item_text->SetText(temp_text);

		CColliderRect* pRC = QuestItemObject->FindComponentFromType<CColliderRect>(CT_COLLIDER);
		pRC->SetTag("quest_slot");
		pRC->SetRect(0, 0, 400.f, 50.f);

		SAFE_RELEASE(pRC);

		SAFE_RELEASE(pDropTableTr);
		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
		//MessageBox(NULL, L"check", L"test", MB_OK);
		quest_ui_component->getQuestList().push_back(QuestItemObject);
	}
	break;

#pragma region Network
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
			MessageBox(NULL, L"���Կ� �����Ͻ��ϴ�.\n�α��� �Ͻʽÿ�.", L"���� ����", MB_OK);
		}
		else if (pPacket->state == PACKETSTATE::PACKETSTATE_JOIN_FAIL)
		{
			MessageBox(NULL, L"�̹� �ش� ���̵� �����մϴ�.", L"���� ����", MB_OK);
		}

	}
	break;
	case SC_PACKET_LOGIN:
	{
		sc_packet_login* login_packet = reinterpret_cast<sc_packet_login*>(packet);

		// �α��� ������ �� ��ȯ.
		GET_SINGLE(SoundManager)->Stop(SC_BGM);
		if (PACKETSTATE::PACKETSTATE_LOGIN_SUCCESS == login_packet->state)
		{
			CScene*	pScene = GET_SINGLE(CSceneManager)->CreateNextScene("MainScene");

			pScene->CreateSceneScript<CMainScene>("MainScene", false);

			SAFE_RELEASE(pScene);
		}
		else if (PACKETSTATE::PACKETSTATE_LOGIN_ID_FAIL == login_packet->state)
		{
			MessageBox(NULL, L"���̵� �������� �ʽ��ϴ�.", L"�α��� ����", MB_OK);
		}
		else if (PACKETSTATE::PACKETSTATE_LOGIN_PW_FAIL == login_packet->state)
		{
			MessageBox(NULL, L"��й�ȣ�� Ʋ���ϴ�.", L"�α��� ����", MB_OK);
		}
	};
#pragma endregion

	case SC_PACKET_SCENECHANGE_SCENE1:
	{
		string appendTag = to_string(NetworkManager::getInstance()->myClientID);
		string objectTag = "Player" + appendTag;
		CGameObject* pGameObject = CGameObject::FindObject(objectTag);

		if (pGameObject != nullptr)
		{
			CTransform* pTr = pGameObject->GetTransform();

			//pTr->SetWorldPos(23, 0, 253);
			GET_SINGLE(SoundManager)->EraseSoundArea();
			GET_SINGLE(SoundManager)->Stop(SC_BGM);
			CScene*	pScene = GET_SINGLE(CSceneManager)->CreateNextScene("MainScene");

			pScene->CreateSceneScript<CMainScene>("MainScene", false);
			SAFE_RELEASE(pScene);
		}


	}
	break;

	case SC_PACKET_SCENECHANGE_SCENE2:
	{
		string appendTag = to_string(NetworkManager::getInstance()->myClientID);
		string objectTag = "Player" + appendTag;
		CGameObject* pGameObject = CGameObject::FindObject(objectTag);

		if (pGameObject != nullptr)
		{
			CTransform* pTr = pGameObject->GetTransform();

			//pTr->SetWorldPos(23, 0, 253);
			GET_SINGLE(SoundManager)->EraseSoundArea();
			GET_SINGLE(SoundManager)->Stop(SC_BGM);
			CScene*	pScene = GET_SINGLE(CSceneManager)->CreateNextScene("SecondScene");

			pScene->CreateSceneScript<SecondScene>("SecondScene", false);
			SAFE_RELEASE(pScene);
		}

		
	}
	break;

	case SC_PACKET_SCENECHANGE_DEVIL_DUNGEON:
	{
		int id = NetworkManager::getInstance()->myClientID;
		string appendTag = to_string(id);
		string objectTag = "Player" + appendTag;
		CGameObject* pGameObject = CGameObject::FindObject(objectTag);
		CTransform* pTr = pGameObject->GetTransform();
		if (pGameObject != nullptr)
		{
			//pTr->SetWorldPos(200, 0, 200);
			GET_SINGLE(SoundManager)->EraseSoundArea();
			GET_SINGLE(SoundManager)->Stop(SC_BGM);
			CScene*	pScene = GET_SINGLE(CSceneManager)->CreateNextScene("DungeonScene");
			pScene->CreateSceneScript<DungeonScene>("DungeonScene", false);
			SAFE_RELEASE(pScene);
		}
		
	}
	break;
#pragma UserInterface
	case SC_PACKET_PARTY_ADD:
	{
		sc_packet_party* pPacket = reinterpret_cast<sc_packet_party*>(packet);
		string objectID = "Player" + to_string(pPacket->toID);
		GET_SINGLE(UserInterfaceManager)->addPartyPlayer(objectID);
	}
	break;
	case SC_PACKET_PARTY_SYNCRONIZE:
	{
		sc_packet_put_player* pPacket = reinterpret_cast<sc_packet_put_player*>(packet);
		string appendTag = to_string(pPacket->id);
		string objectTag = "Player" + appendTag;
		//GET_SINGLE(UserInterfaceManager)->partySyncronize(objectTag, pPacket->x, pPacket->y, pPacket->z, pPacket->current_hp, pPacket->current_mp, pPacket->level, pPacket->exp);
	}
	break;

	case SC_PACKET_CHAT_PLAYER:
	{
		sc_packet_chat* pPacket = reinterpret_cast<sc_packet_chat*>(packet);
		wstring tag = to_wstring(pPacket->id);
		wstring chat = L"Player" + tag + L" : ";
		chat += pPacket->message;
		GET_SINGLE(UserInterfaceManager)->pushChatMessage(chat);
	}
	break;

	case SC_PACKET_ROOTING_TABLE:
	{
		sc_packet_require_itemtable* pPacket = reinterpret_cast<sc_packet_require_itemtable*>(packet);
		DropTableUI* pDropTableUI = GET_SINGLE(UserInterfaceManager)->getDropTableUI();

		for (int i = 0; i < pPacket->count; ++i)
		{
			pDropTableUI->addDropItemSlot(pPacket->itemname1 + (i * 100));
		}

		pDropTableUI->enableRender(true);
	}
	break;

	case SC_PACKET_EQUIPITEM_PLAYER:
	{
		sc_packet_equip_player* pPacket = reinterpret_cast<sc_packet_equip_player*>(packet);

		string appendTag = to_string(pPacket->playerID);
		string objectTag = "Player" + appendTag;
		CGameObject* pGameObject = CGameObject::FindObject(objectTag);
		if (nullptr != pGameObject)
		{
			CLayer*	pLayer = GET_SINGLE(CSceneManager)->GetCurrentScene()->GetLayer("Default");
			CGameObject *pSwordObj = CGameObject::CreateObject(objectTag + "Sword", pLayer);
			CSword	*pSword = pSwordObj->AddComponent<CSword>("Sword");
			pSword->setTargetPlayerID(pPacket->playerID);
			pSword->initialize();

			SAFE_RELEASE(pSword);
			SAFE_RELEASE(pSwordObj);
			SAFE_RELEASE(pLayer);
		}
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
#pragma endregion
	default:
		break;
	}


}

void NetworkManager::readPacket(SOCKET sock)
{
	DWORD iobyte, ioflag = 0;

	int ret = WSARecv(sock, &recv_wsabuf, 1, &iobyte, &ioflag, NULL, NULL);
	BYTE* ptr = reinterpret_cast<BYTE*>(recvBuffer);

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

void NetworkManager::connectMainServer()
{
	/// < mainserver���� ���ӿ�û�� �Ѵ�. >
	cs_packet_connect* pPacket = reinterpret_cast<cs_packet_connect*>(getSendBuffer());
	pPacket->size = sizeof(cs_packet_connect);
	pPacket->type = CS_PACKET_MAINSERVER_CONNECT;

	getSendWsaBuf().len = sizeof(cs_packet_connect);
	DWORD iobyte;
	int ret = WSASend(getSocket(), &getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);
}

#pragma region Getter&Setter
const SOCKET & NetworkManager::getSocket() { return sock; }
char * NetworkManager::getSendBuffer() { return sendBuffer; }
const char * NetworkManager::getRecvBuffer() { return recvBuffer; }
const char * NetworkManager::getPacketBuffer() { return packetBuffer; }
const DWORD NetworkManager::getInPacketSize() { return inPacketSize; }
const int NetworkManager::getSavedPacketSize() { return savedPacketSize; }
const size_t NetworkManager::getMyClientID() { return myClientID; }
const bool NetworkManager::getIsConnected() { return isConnected; }
WSABUF & NetworkManager::getSendWsaBuf() { return this->send_wsabuf; }
WSABUF & NetworkManager::getRecvWsaBuf() { return this->recv_wsabuf; }
void NetworkManager::setPlayer(CGameObject * pPlayer) { this->pPlayer = pPlayer; }
void NetworkManager::enableConnected(bool connected) { this->isConnected = connected; }
void NetworkManager::setMyClientID(size_t id) { this->myClientID = id; }
#pragma endregion
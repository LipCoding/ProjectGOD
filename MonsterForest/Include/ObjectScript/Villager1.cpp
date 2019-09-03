#include "Villager1.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Component/Collider.h"
#include "Component/Camera.h"
#include "Device.h"
#include "Core/Input.h"
#include "Component/ParticleSingle.h"
#include "Component/Animation2D.h"
#include "Component/Navigation3D.h"
#include "../ObjectScript/Effect.h"
#include "../SceneScript/MainScene.h"
#include "Scene/SceneManager.h"
#include "../Client.h"
#include "Player.h"
#include "../NetworkManager.h"
#include "../UserInterfaceManager.h"
#include "../DropItemSlot.h"
#include "../DropTableUI.h"
#include "../QuestManager.h"

Villager1::Villager1()
{
}


Villager1::~Villager1()
{
}

void Villager1::settingStatus(int current_hp, int current_mp, int level, int exp)
{
}

bool Villager1::Init()
{
	m_pAnimation = m_pGameObject->AddComponent<CAnimation>("PlayerAnimation");

	m_pAnimation->Load("99.Dynamic_Mesh\\01.NPC\\Villager1\\Villager1.anm");
	m_pAnimation->SetDefaultClip("Idle1");

	return true;
}

void Villager1::Input(float fTime)
{
}

int Villager1::Update(float fTime)
{
	return 0;
}

int Villager1::LateUpdate(float fTime)
{
	return 0;
}

Villager1 * Villager1::Clone()
{
	return nullptr;
}

void Villager1::OnCollisionEnter(CCollider * pSrc, CCollider * pDest, float fTime)
{

}

void Villager1::OnCollision(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseRay" && GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		//string tag = m_pGameObject->GetTag();
		//tag.erase(0, 6);
		//int id = atoi(tag.c_str());

		//char str[128];
		//string appendTag = _itoa(NetworkManager::getInstance()->getMyClientID(), str, 10);
		//string objectTag = "Player" + appendTag;
		//CGameObject* pGameObject = CGameObject::FindObject(objectTag);
		//CPlayer* pPlayer = pGameObject->FindComponentFromTypeName<CPlayer>("Actor");
		//pPlayer->clickedID = id;

		//CUIButton* pEnemyUIHearthBar = GET_SINGLE(UserInterfaceManager)->getEnemyUIHearthBar();
		//float ratio = (float)this->getCurrentHP() / (float)this->getMaxHP();
		//pEnemyUIHearthBar->setLengthRatio(ratio);

		//CGameObject* pUIObject = pEnemyUIHearthBar->GetGameObject();
		//GET_SINGLE(UserInterfaceManager)->getEnemyStatus()->enableRender(true);
		//GET_SINGLE(UserInterfaceManager)->getEnemyStatus()->setTarget(this);
		//SAFE_RELEASE(pUIObject);
	}

	if (KEYDOWN("MouseRButton"))
	{
		if (pDest->GetTag() == "MouseRay" && GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		{
			// NPC는 자기가 어떤 퀘스트를 줄건지 갖고있어야함.
			Quest* quest = QuestManager::getInstance()->findQuest("The Archiereus Of Flame");
			wstring quest_name = strconv(quest->getQuestSubject());
			/*
							cs_packet_require_itemtable* packet = reinterpret_cast<cs_packet_require_itemtable*>(NetworkManager::getInstance()->getSendBuffer());
				packet->size = sizeof(cs_packet_require_itemtable);
				packet->type = CS_PACKET_ROOTING_TABLE;
				packet->targetId = id;
				DWORD iobyte;
				NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_require_itemtable);
				int ret = WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);
			*/
			cs_packet_quest_add* packet = reinterpret_cast<cs_packet_quest_add*>(NetworkManager::getInstance()->getSendBuffer());
			packet->size = sizeof(cs_packet_quest_add);
			_cprintf("%d", packet->size);
			packet->type = CS_PACKET_QUEST_ADD;
			wcscpy_s(packet->qeustname, quest_name.c_str());
			packet->qeustname[MAX_STR_SIZE] = 0;
		
			DWORD iobyte;
			NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_quest_add);
			int ret = WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);
			// 갖고있는 퀘스트의 목록을 보여준다.
			{
				/*
				string tag = m_pGameObject->GetTag();
				tag.erase(0, 6);
				int id = atoi(tag.c_str());
				// 서버로부터 해당 아이템이 어떤 종류가 있는지 테이블을 얻어온다.
				cs_packet_require_itemtable* packet = reinterpret_cast<cs_packet_require_itemtable*>(NetworkManager::getInstance()->getSendBuffer());
				packet->size = sizeof(cs_packet_require_itemtable);
				packet->type = CS_PACKET_ROOTING_TABLE;
				packet->targetId = id;
				DWORD iobyte;
				NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_require_itemtable);
				int ret = WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);

				POINT mousePos = GET_SINGLE(CInput)->GetMousePos();
				DropTableUI* pDropTableUI = GET_SINGLE(UserInterfaceManager)->getDropTableUI();
				pDropTableUI->clear();
				//pDropTableUI->
				//pDropTableUI->enableRender(true);
				CTransform* pDropTableUITr = pDropTableUI->GetTransform();
				pDropTableUITr->SetWorldPos(Vector3(mousePos.x, mousePos.y, 0));
				SAFE_RELEASE(pDropTableUITr);
				*/
			}
		}
	}
}

void Villager1::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

#include "Inventory.h"
#include "Component/Renderer2D.h"
#include "Scene/Layer.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/Renderer2D.h"
#include "Component/ColliderRect.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "Slot.h"
#include "Item.h"
Inventory::Inventory()
{
}


Inventory::~Inventory()
{
}

bool Inventory::initialize()
{
	for (int i = 0; i < EQUIP_END; ++i)
		pEquipSlot[i] = nullptr;

	{
		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer*	pLayer = pScene->GetLayer("UI+1");

		this->pInventory = CGameObject::CreateObject("Inventorty", pLayer);

		CUIPanel*	pButton = pInventory->AddComponent<CUIPanel>("ChatEdit");
		SAFE_RELEASE(pButton);

		CTransform*	pInventoryTr = pInventory->GetTransform();

		//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
		pInventoryTr->SetWorldScale(250.f, 400.f, 1.f);
		pInventoryTr->SetWorldPos(400.f, 100.f, 0.f);

		SAFE_RELEASE(pInventoryTr);
		CRenderer2D* pRenderer = pInventory->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "Slot",
			0, 0, L"UserInterface/UI_BASIC_BOX_BIG.png");

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);

		CColliderRect* pRC = pInventory->FindComponentFromType<CColliderRect>(CT_COLLIDER);
		pRC->SetRect(0, 0, 250.f, 30.f);
		pRC->SetTag("InventoryRect");
		SAFE_RELEASE(pRC);

		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
	}

	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer*	pLayer = pScene->GetLayer("UI+2");
	for (int i = 0; i < SLOT_ROW; ++i)
	{
		for (int j = 0; j < SLOT_COL; ++j)
		{
			int index = (i * SLOT_COL) + j;
			char str[128];
			string appendTag = _itoa(index, str, 10);
			string slotTag = "Slot" + appendTag;
			CGameObject* pSlotObject = CGameObject::CreateObject(slotTag, pLayer);
			this->pSlot[i][j] = pSlotObject->AddComponent<Slot>("Slot");
			pSlot[i][j]->initialize(index);
			CTransform*	pSlotTr = pSlotObject->GetTransform();

			//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
			pSlotTr->SetWorldScale(32.f, 32.f, 1.f);
			//pSlotTr->SetWorldPos(300.f + ((index / SLOT_COL) * 32)+3, 100.f + ((index % SLOT_ROW) * 32)+3, 0.f);

			SAFE_RELEASE(pSlotTr);

			//SAFE_RELEASE(pButton);
			//this->pSlot[i][j] = new Slot;
			//this->pSlot[i][j]->initialize((i*SLOT_ROW) + j);
			CRenderer2D* pRenderer = pSlotObject->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
			CMaterial* pMaterial = pRenderer->GetMaterial();

			pMaterial->SetDiffuseTexInfo("Linear", "Slot",
				0, 0, L"UserInterface/UI_BASIC_BOX_MINI.png");
			
			SAFE_RELEASE(pMaterial);
			SAFE_RELEASE(pRenderer);

			CColliderRect* pRC = pSlotObject->FindComponentFromType<CColliderRect>(CT_COLLIDER);
			pRC->SetTag(slotTag);
			pRC->SetRect(0, 0, 32.f, 32.f);

			SAFE_RELEASE(pRC);
			SAFE_RELEASE(pSlotObject);
		}
	}

	// helmet
	{
		string appendTag = to_string(EQUIP_HELMET);
		string slotTag = "EquipSlot" + appendTag;
		CGameObject* pSlotObject = CGameObject::CreateObject(slotTag, pLayer);
		this->pEquipSlot[EQUIP_HELMET] = pSlotObject->AddComponent<Slot>("Slot");
		this->pEquipSlot[EQUIP_HELMET]->initialize(EQUIP_HELMET);
		this->pEquipSlot[EQUIP_HELMET]->setOffsetPos(Vector3(100.f, 30.f, 0.f));
		this->pEquipSlot[EQUIP_HELMET]->enableEquipSlot(true);
		CTransform*	pSlotTr = pSlotObject->GetTransform();

		//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
		pSlotTr->SetWorldScale(48.f, 48.f, 1.f);
		pSlotTr->SetWorldPos(300.f, 20, 0.f);

		SAFE_RELEASE(pSlotTr);

		//SAFE_RELEASE(pButton);
		//this->pSlot[i][j] = new Slot;
		//this->pSlot[i][j]->initialize((i*SLOT_ROW) + j);
		CRenderer2D* pRenderer = pSlotObject->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", slotTag,
			0, 0, L"UserInterface/UI_INVEN_HELMET.png");

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);

		CColliderRect* pRC = pSlotObject->FindComponentFromType<CColliderRect>(CT_COLLIDER);
		pRC->SetTag(slotTag);
		pRC->SetRect(0, 0, 32.f, 32.f);

		SAFE_RELEASE(pRC);
		SAFE_RELEASE(pSlotObject);
	}

	// Armor
	{
		string appendTag = to_string(EQUIP_ARMOR);
		string slotTag = "EquipSlot" + appendTag;
		CGameObject* pSlotObject = CGameObject::CreateObject(slotTag, pLayer);
		this->pEquipSlot[EQUIP_ARMOR] = pSlotObject->AddComponent<Slot>("Slot");
		this->pEquipSlot[EQUIP_ARMOR]->initialize(EQUIP_ARMOR);
		this->pEquipSlot[EQUIP_ARMOR]->setOffsetPos(Vector3(100.f, 80.f, 0.f));
		this->pEquipSlot[EQUIP_ARMOR]->enableEquipSlot(true);
		CTransform*	pSlotTr = pSlotObject->GetTransform();

		//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
		pSlotTr->SetWorldScale(48.f, 48.f, 1.f);
		pSlotTr->SetWorldPos(300.f, 20, 0.f);

		SAFE_RELEASE(pSlotTr);

		//SAFE_RELEASE(pButton);
		//this->pSlot[i][j] = new Slot;
		//this->pSlot[i][j]->initialize((i*SLOT_ROW) + j);
		CRenderer2D* pRenderer = pSlotObject->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", slotTag,
			0, 0, L"UserInterface/UI_INVEN_ARMOR.png");

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);

		CColliderRect* pRC = pSlotObject->FindComponentFromType<CColliderRect>(CT_COLLIDER);
		pRC->SetTag(slotTag);
		pRC->SetRect(0, 0, 32.f, 32.f);

		SAFE_RELEASE(pRC);
		SAFE_RELEASE(pSlotObject);
	}

	// Shoose
	{
		string appendTag = to_string(EQUIP_SHOOSE);
		string slotTag = "EquipSlot" + appendTag;
		CGameObject* pSlotObject = CGameObject::CreateObject(slotTag, pLayer);
		this->pEquipSlot[EQUIP_SHOOSE] = pSlotObject->AddComponent<Slot>("Slot");
		this->pEquipSlot[EQUIP_SHOOSE]->initialize(EQUIP_SHOOSE);
		this->pEquipSlot[EQUIP_SHOOSE]->setOffsetPos(Vector3(100.f, 130.f, 0.f));
		this->pEquipSlot[EQUIP_SHOOSE]->enableEquipSlot(true);
		CTransform*	pSlotTr = pSlotObject->GetTransform();

		//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
		pSlotTr->SetWorldScale(48.f, 48.f, 1.f);
		pSlotTr->SetWorldPos(300.f, 20, 0.f);

		SAFE_RELEASE(pSlotTr);

		//SAFE_RELEASE(pButton);
		//this->pSlot[i][j] = new Slot;
		//this->pSlot[i][j]->initialize((i*SLOT_ROW) + j);
		CRenderer2D* pRenderer = pSlotObject->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", slotTag,
			0, 0, L"UserInterface/UI_INVEN_SHOES.png");

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);

		CColliderRect* pRC = pSlotObject->FindComponentFromType<CColliderRect>(CT_COLLIDER);
		pRC->SetTag(slotTag);
		pRC->SetRect(0, 0, 32.f, 32.f);

		SAFE_RELEASE(pRC);
		SAFE_RELEASE(pSlotObject);
	}

	// WEAPON
	{
		string appendTag = to_string(EQUIP_WEAPON);
		string slotTag = "EquipSlot" + appendTag;
		CGameObject* pSlotObject = CGameObject::CreateObject(slotTag, pLayer);
		this->pEquipSlot[EQUIP_WEAPON] = pSlotObject->AddComponent<Slot>("Slot");
		this->pEquipSlot[EQUIP_WEAPON]->initialize(EQUIP_WEAPON);
		this->pEquipSlot[EQUIP_WEAPON]->setOffsetPos(Vector3(50.f, 80.f, 0.f));
		this->pEquipSlot[EQUIP_WEAPON]->enableEquipSlot(true);
		CTransform*	pSlotTr = pSlotObject->GetTransform();

		//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
		pSlotTr->SetWorldScale(48.f, 48.f, 1.f);
		pSlotTr->SetWorldPos(300.f, 20, 0.f);

		SAFE_RELEASE(pSlotTr);

		//SAFE_RELEASE(pButton);
		//this->pSlot[i][j] = new Slot;
		//this->pSlot[i][j]->initialize((i*SLOT_ROW) + j);
		CRenderer2D* pRenderer = pSlotObject->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", slotTag,
			0, 0, L"UserInterface/UI_INVEN_WEAPON.png");

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);

		CColliderRect* pRC = pSlotObject->FindComponentFromType<CColliderRect>(CT_COLLIDER);
		pRC->SetTag(slotTag);
		pRC->SetRect(0, 0, 32.f, 32.f);

		SAFE_RELEASE(pRC);
		SAFE_RELEASE(pSlotObject);
	}

	// RING
	{
		string appendTag = to_string(EQUIP_RING);
		string slotTag = "EquipSlot" + appendTag;
		CGameObject* pSlotObject = CGameObject::CreateObject(slotTag, pLayer);
		this->pEquipSlot[EQUIP_RING] = pSlotObject->AddComponent<Slot>("Slot");
		this->pEquipSlot[EQUIP_RING]->initialize(EQUIP_RING);
		this->pEquipSlot[EQUIP_RING]->setOffsetPos(Vector3(150.f, 80.f, 0.f));
		this->pEquipSlot[EQUIP_RING]->enableEquipSlot(true);
		CTransform*	pSlotTr = pSlotObject->GetTransform();

		//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
		pSlotTr->SetWorldScale(48.f, 48.f, 1.f);
		pSlotTr->SetWorldPos(300.f, 20, 0.f);

		SAFE_RELEASE(pSlotTr);

		//SAFE_RELEASE(pButton);
		//this->pSlot[i][j] = new Slot;
		//this->pSlot[i][j]->initialize((i*SLOT_ROW) + j);
		CRenderer2D* pRenderer = pSlotObject->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", slotTag,
			0, 0, L"UserInterface/UI_INVEN_RING.png");

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);

		CColliderRect* pRC = pSlotObject->FindComponentFromType<CColliderRect>(CT_COLLIDER);
		pRC->SetTag(slotTag);
		pRC->SetRect(0, 0, 32.f, 32.f);

		SAFE_RELEASE(pRC);
		SAFE_RELEASE(pSlotObject);
	}

	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pScene);
	enableShowInventory();
	return true;
}

void Inventory::itemMoveToAnotherSlot()
{
	// 슬롯이 유효한가, 원래 아이템 있던자리인가
}

// 안할수도있음.
void Inventory::dropItem()
{
	// 월드에 아이템을 떨어트린다.
}

void Inventory::addItem(CGameObject* pItem)
{
	// 인벤토리에 아이템을 추가한다.
	for (int i = 0; i < SLOT_ROW; ++i)
	{
		for (int j = 0; j < SLOT_COL; ++j)
		{
			if (pSlot[i][j]->getItem() == nullptr)
			{
				pSlot[i][j]->addItem(pItem);
				return;
			}
		}
	}
}

void Inventory::removeItem()
{
	// 해당 인벤토리 아이템을 삭제한다.

}

void Inventory::update(float time)
{
	CTransform*	pInventoryTr = pInventory->GetTransform();

	Vector3 pos = pInventoryTr->GetWorldPos();


	for (int i = 0; i < SLOT_ROW; ++i)
	{
		for (int j = 0; j < SLOT_COL; ++j)
		{
			pSlot[i][j]->move(pos);
		}
	}
	for (int i = 0; i < EQUIP_END; ++i)
	{
		if (nullptr != pEquipSlot[i])
			pEquipSlot[i]->move(pos);
	}
	for (int i = 0; i < SLOT_ROW; ++i)
	{
		for (int j = 0; j < SLOT_COL; ++j)
		{
			pSlot[i][j]->update(time);
		}
	}

	for (int i = 0; i < EQUIP_END; ++i)
	{
		if(nullptr != pEquipSlot[i])
			pEquipSlot[i]->update(time);
	}
	SAFE_RELEASE(pInventoryTr);
}

void Inventory::enableShowInventory()
{
	this->show = !this->show;

	if (false == this->show)
	{
		this->pInventory->Enable(false);

		for (int i = 0; i < SLOT_ROW; ++i)
		{
			for (int j = 0; j < SLOT_COL; ++j)
			{
				pSlot[i][j]->showItem(false);
				CGameObject* pSlotObject = pSlot[i][j]->GetGameObject();
				pSlotObject->Enable(false);
				SAFE_RELEASE(pSlotObject);
			}
		}
		for (int i = 0; i < EQUIP_END; ++i)
		{
			if (nullptr != pEquipSlot[i])
			{
				pEquipSlot[i]->showItem(false);
				CGameObject* pSlotObject = pEquipSlot[i]->GetGameObject();
				pSlotObject->Enable(false);
				SAFE_RELEASE(pSlotObject);
			}
		}
	}
	else
	{
		this->pInventory->Enable(true);

		for (int i = 0; i < SLOT_ROW; ++i)
		{
			for (int j = 0; j < SLOT_COL; ++j)
			{
				pSlot[i][j]->showItem(true);
				CGameObject* pSlotObject = pSlot[i][j]->GetGameObject();
				pSlotObject->Enable(true);
				SAFE_RELEASE(pSlotObject);
			}
		}

		for (int i = 0; i < EQUIP_END; ++i)
		{
			if (nullptr != pEquipSlot[i])
			{
				pEquipSlot[i]->showItem(true);
				CGameObject* pSlotObject = pEquipSlot[i]->GetGameObject();
				pSlotObject->Enable(true);
				SAFE_RELEASE(pSlotObject);
			}
		}


	}
}

void Inventory::enableRender(bool show)
{
	this->show = show;
	this->pInventory->Enable(show);

	for (int i = 0; i < SLOT_ROW; ++i)
	{
		for (int j = 0; j < SLOT_COL; ++j)
		{
			pSlot[i][j]->showItem(show);
			CGameObject* pSlotObject = pSlot[i][j]->GetGameObject();
			pSlotObject->Enable(show);
			SAFE_RELEASE(pSlotObject);
		}
	}
	for (int i = 0; i < EQUIP_END; ++i)
	{
		if (nullptr != pEquipSlot[i])
		{
			pEquipSlot[i]->showItem(show);
			CGameObject* pSlotObject = pEquipSlot[i]->GetGameObject();
			pSlotObject->Enable(show);
			SAFE_RELEASE(pSlotObject);
		}
	}
}
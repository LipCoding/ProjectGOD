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

Inventory::Inventory()
{
}


Inventory::~Inventory()
{
}

bool Inventory::initialize()
{
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
			0, 0, L"LoginEdit.bmp");

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);

		CColliderRect* pRC = pInventory->FindComponentFromType<CColliderRect>(CT_COLLIDER);
		pRC->SetRect(0, 0, 250.f, 30.f);
		pRC->SetTag("InventoryRect");
		SAFE_RELEASE(pRC);

		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
	}
	
	/*
		{
			char str[128];
			string appendTag = _itoa(index, str, 10);
			string slotTag = "Slot" + appendTag;


			this->pSlot = CGameObject::CreateObject(slotTag, pLayer);

			//CUIButton*	pButton = pSlot->AddComponent<CUIButton>("ChatEdit");
			//SAFE_RELEASE(pButton);

			CTransform*	pSlotTr = pSlot->GetTransform();

			//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
			pSlotTr->SetWorldScale(32.f, 32.f, 1.f);
			pSlotTr->SetWorldPos(400.f + ((index / 5)*32), 100.f + ((index%5)*32), 0.f);

			SAFE_RELEASE(pSlotTr);
		}
	*/
	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer*	pLayer = pScene->GetLayer("UI+1");
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
				0, 0, L"LoginEdit.bmp");

			SAFE_RELEASE(pMaterial);
			SAFE_RELEASE(pRenderer);

			CColliderRect* pRC = pSlotObject->FindComponentFromType<CColliderRect>(CT_COLLIDER);
			pRC->SetTag(slotTag);
			pRC->SetRect(0, 0, 32.f, 32.f);

			SAFE_RELEASE(pRC);
			SAFE_RELEASE(pSlotObject);
		}
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

	for (int i = 0; i < SLOT_ROW; ++i)
	{
		for (int j = 0; j < SLOT_COL; ++j)
		{
			pSlot[i][j]->update(time);
		}
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
	}
}

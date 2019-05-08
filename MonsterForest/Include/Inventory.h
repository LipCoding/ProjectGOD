#pragma once

#include "Component/Script.h"
#include "Component/UIPanel.h"
#include "Slot.h"
PG_USING

#define SLOT_COL 5
#define SLOT_ROW 5

enum EQUIP { EQUIP_HELMET=0, EQUIP_ARMOR, EQUIP_WEAPON, EQUIP_SHOOSE, EQUIP_RING,
	EQUIP_END};

class Inventory
{
	CGameObject* pInventory;
	Slot* pSlot[SLOT_ROW][SLOT_COL];
	Slot* pEquipSlot[EQUIP_END];
	int currentIndex = 0;
	CGameObject* pTempItem = nullptr;
	bool show = true;
public:
	Inventory();
	~Inventory();

public:
	bool initialize();
	void itemMoveToAnotherSlot();
	void dropItem();
	void addItem(CGameObject* pItem);
	void removeItem();
	void update(float time);
	void enableShowInventory();
	void enableRender(bool show);
public:

	void setTempItem(CGameObject* pItem) { this->pTempItem = pItem; }
	CGameObject* getTempItem() { return this->pTempItem; }
public:
	CGameObject* getInventoryObject() { return this->pInventory; }
	//int (*func(void))[4]
	Slot* (*getSlot())[SLOT_COL] { return pSlot; }
	void setCurrentIndex(int index) { this->currentIndex = index; }
	int getCurrentIndex() { return this->currentIndex; }
};


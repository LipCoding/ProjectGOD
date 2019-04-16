#pragma once

#include "Component/Script.h"
#include "Component/UIPanel.h"
#include "Slot.h"
PG_USING

#define SLOT_COL 5
#define SLOT_ROW 5

class Inventory
{
	CGameObject* pInventory;
	Slot* pSlot[SLOT_ROW][SLOT_COL];

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


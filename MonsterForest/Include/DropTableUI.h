#pragma once

#include "Component/Script.h"
#include "Component/UIPanel.h"
#include "Component/UIButton.h"
PG_USING

class DropTableUI :
	public CUIPanel
{
	//CUIButton* pDropItemSlot1 = nullptr;
	//CUIButton* pDropItemSlot2 = nullptr;
	//CUIButton* pDropItemSlot3 = nullptr;
	vector<CUIButton*> dropItemSlots;
public:
	DropTableUI();
	~DropTableUI();

public:
	bool initialize();

public:
	void enableRender(bool show);
public:
	void addDropItemSlot(const wstring& itemname);
	bool isEmpty() { return dropItemSlots.empty(); }
public:
	void detachDropItemSlot(int slotNumber);
public:
	/*CUIButton* getItemTable1() { return pDropItemSlot1; }
	CUIButton* getItemTable2() { return pDropItemSlot2; }
	CUIButton* getItemTable3() { return pDropItemSlot3; }*/
	void clear();
public:
	int Update(float fTime);

};


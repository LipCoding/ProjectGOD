#pragma once

#include "Component/Script.h"
#include "Component/UIPanel.h"
#include "Component/UIButton.h"
PG_USING

class DropTableUI :
	public CUIPanel
{
	CUIButton* pDropItemSlot1 = nullptr;
	CUIButton* pDropItemSlot2 = nullptr;
	CUIButton* pDropItemSlot3 = nullptr;

public:
	DropTableUI();
	~DropTableUI();

public:
	bool initialize();

public:
	void enableRender(bool show);
public:
	void addDropItemSlot(int itemid);
public:
	void detachDropItemSlot(int slotNumber);
public:
	CUIButton* getItemTable1() { return pDropItemSlot1; }
	CUIButton* getItemTable2() { return pDropItemSlot2; }
	CUIButton* getItemTable3() { return pDropItemSlot3; }
public:
	int Update(float fTime);
public:
	virtual void OnCollisionEnter(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollision(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollisionLeave(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
};


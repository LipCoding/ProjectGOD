#pragma once

#include "Component/Script.h"
#include "Component/UIPanel.h"
#include "Component/UIButton.h"
PG_USING

class DropTableUI :
	public CUIPanel
{
	CUIButton* pDropItemSlot1;
	CUIButton* pDropItemSlot2;
	CUIButton* pDropItemSlot3;

public:
	DropTableUI();
	~DropTableUI();

public:
	bool initialize();

public:
	void enableRender(bool show);

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


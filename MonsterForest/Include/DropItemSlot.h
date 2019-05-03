#pragma once

#include "Component/Script.h"
#include "Component/UIPanel.h"
#include "Component/UIButton.h"

PG_USING

class DropItemSlot
	: public CUIButton

{
private:
	class DropTableUI* pDropTableUI = nullptr;
	int index = -1;
public:
	DropItemSlot();
	~DropItemSlot();

public:
	virtual void OnCollisionEnter(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollision(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollisionLeave(class CCollider* pSrc, class CCollider* pDest,
		float fTime);

	void setDropTableUI(class DropTableUI* pDropTableUI) { this->pDropTableUI = pDropTableUI; };
	void setIndex(int index) { this->index = index; }
	void deleteSlot();
};


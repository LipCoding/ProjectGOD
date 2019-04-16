#pragma once
#include "Component/Script.h"
#include "Component/UIButton.h"

PG_USING

class Slot :
	public CUIButton
{
private:
	int index = 0;
	//CGameObject* pSlot = nullptr;
	CGameObject* pItem = nullptr;
public:
	Slot();
	~Slot();

public:
	bool initialize(int index);
	void addItem(CGameObject* pItem);
	void dropItem();
	void removeItem();
	bool isHaveItem();
	void update(float time);
	void move(const Vector3& movement);

public:
	CGameObject* getItem() { return this->pItem; }
	void showItem(bool show);
public:
	virtual void OnCollisionEnter(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollision(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollisionLeave(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
};


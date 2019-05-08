#pragma once
#include "Component/Script.h"
#include "Component/UIButton.h"

PG_USING

class Slot :
	public CUIButton
{
private:
	int index = 0;
	bool equipslot = false;
	//CGameObject* pSlot = nullptr;
	CGameObject* pItem = nullptr;
	Vector3 offsetPos = Vector3::Zero;
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
	bool isEquipSlot() { return equipslot; }
	void enableEquipSlot(bool enable) { this->equipslot = enable; }

public:
	Vector3 getOffsetPos() { return offsetPos; }
	void setOffsetPos(const Vector3& pos) { this->offsetPos = pos; }

public:
	virtual void OnCollisionEnter(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollision(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollisionLeave(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
};


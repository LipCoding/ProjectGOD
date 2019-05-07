#pragma once

#include "Component/Script.h"
#include "Component/UIButton.h"

PG_USING

class UserInfoUI :
	public CUIButton
{
	class TargetPlayerUI* pBaseUI = nullptr;
public:
	class TargetPlayerUI* getBaseUI() { return pBaseUI; }
	void setBaseUI(class TargetPlayerUI* pBaseUI) { this->pBaseUI = pBaseUI; }
public:
	UserInfoUI();
	~UserInfoUI();

public:
	void initialize();

public:
	virtual void OnCollisionEnter(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollision(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollisionLeave(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
};


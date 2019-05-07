#pragma once

#include "Component/Script.h"
#include "Component/UIButton.h"

PG_USING


class PGConfirm :
	public CUIButton
{
public:
	PGConfirm();
	~PGConfirm();
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


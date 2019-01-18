#pragma once

#include "Component/Script.h"
#include "Component/Transform.h"
#include "Component/Animation.h"
#include "Component/Camera.h"
#include "GameObject/GameObject.h"

PG_USING

class CCharacterPanel :
	public CScript
{
public:
	CCharacterPanel();
	CCharacterPanel(const CCharacterPanel& panel);
	~CCharacterPanel();

private:
	CGameObject*	m_pTarget;
	CTransform*		m_pTargetTransform;
	CCamera*		m_pTargetCamera;
	CTransform*		m_pCameraTransform;
	bool			m_bAddResourceView;

public:
	void SetTarget(CGameObject* pTarget)
	{
		m_pTarget = pTarget;

		m_pTargetTransform = m_pTarget->GetTransform();
		m_pTargetTransform->Release();
	}

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CCharacterPanel* Clone();
	virtual void OnCollisionEnter(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollision(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollisionLeave(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
};


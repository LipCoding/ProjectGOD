#pragma once
#include "../Obj.h"

PG_BEGIN

class PG_DLL CBoneSocket :
	public CObj
{
	friend class CAnimation;

private:
	CBoneSocket();
	CBoneSocket(const CBoneSocket& socket);
	~CBoneSocket();

private:
	Vector3		m_vOffset;
	class CGameObject*	m_pTarget;
	class CTransform*	m_pTargetTr;

public:
	void SetTarget(class CGameObject* pTarget);
	void Update(const Matrix& matWorld, const Matrix& matBone);
	CBoneSocket* Clone()	const;
};

PG_END

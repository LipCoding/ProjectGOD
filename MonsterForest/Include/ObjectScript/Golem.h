#pragma once

#include "Component/Script.h"
#include "Component/Transform.h"
#include "Component/Animation.h"
#include "Component/Navigation3D.h"
#include "GolemState.h"
PG_USING

class Golem	:
	public CScript
{
	using GOLEMSTATE = pair<string, GolemState*>;
public:
	Golem();
	Golem(const Golem& goblin);
	~Golem();
private:
	Vector3 initial_pos;
	Vector3 roaming_pos;
	bool returnToInitialPos{false};
	float hearth_point{ 50 };
private:
	CTransform*		m_pTarget;
	CAnimation*		m_pAnimation;
	CNavigation3D*	m_pNavigation;
	bool            m_bEffectCheck = false;
	float           m_fEffectTime = 0.f;
protected:
	GolemState* pState{nullptr};
	unordered_map<int, GOLEMSTATE> stateMap;
	size_t currentAnimation{ 0 };
	size_t nextAnimation{ 0 };
public:
	GolemState* getCurrentState();
	GolemState* getState(int stateID);
public:
	size_t getCurrentAnimation();
	size_t getNextAnimation();
	void changeAnimation();
	const Vector3& getInitialPos() { return initial_pos; }
	const Vector3& getRoamingPos() { return roaming_pos; }
public:
	void setCurrentAnimation(size_t currentAnimation);
	void setNextAnimation(size_t nextAnimation);
	void setCurrentState(GolemState* pState);
	void setInitialPos(const Vector3& initial_pos) { this->initial_pos = initial_pos; }
	void setRoamingPos(const Vector3& roaming_pos) { this->roaming_pos = roaming_pos; }

public:
	void SetTarget(const string& strTag);
	CAnimation* GetAnimation();
	CTransform* getTargetTransform() { return m_pTarget; }
public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual Golem* Clone();
	virtual void OnCollisionEnter(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollision(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollisionLeave(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
};


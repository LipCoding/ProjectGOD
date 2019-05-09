#pragma once

#include "Monster.h"

PG_USING

class DemonLord :
	public Monster
{
public:
	DemonLord();
	~DemonLord();

private:
	int currentHP = 1500;
	int currentMP = 300;

	int maxHP = 1500;
	int maxMP = 300;

	int attackDamage = 10;

	Vector3 initial_pos;
	Vector3 roaming_pos;
	bool returnToInitialPos{ false };
	float hearth_point{ 50 };

private:
	CTransform*		m_pTarget;
	CAnimation*		m_pAnimation;
	CNavigation3D*	m_pNavigation;
	bool            m_bEffectCheck = false;
	float           m_fEffectTime = 0.f;
protected:
	//GolemState* pState{ nullptr };
	//unordered_map<int, GOLEMSTATE> stateMap;
	size_t currentAnimation{ 0 };
	size_t nextAnimation{ 0 };
public:
	//GolemState* getCurrentState();
	//GolemState* getState(int stateID);
public:
	size_t getCurrentAnimation();
	size_t getNextAnimation();
	void changeAnimation();
	const Vector3& getInitialPos() { return initial_pos; }
	const Vector3& getRoamingPos() { return roaming_pos; }
public:
	//void setCurrentAnimation(size_t currentAnimation);
	//void setNextAnimation(size_t nextAnimation);
	//void setCurrentState(GolemState* pState);
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
	virtual DemonLord* Clone();
	virtual void OnCollisionEnter(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollision(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollisionLeave(class CCollider* pSrc, class CCollider* pDest,
		float fTime);

public:
public:
	int getCurrentHP() { return currentHP; }
	int getCurrentMP() { return currentMP; }
	int getMaxHP() { return maxHP; }
	int getMaxMP() { return maxMP; }
	int getAttackDamage() { return attackDamage; }
public:
	void setCurrentHP(int currentHP) { this->currentHP = currentHP; }
	void setCurrentMP(int currentMP) { this->currentMP = currentMP; }
	void setMaxHP(int maxHP) { this->maxHP = maxHP; }
	void setMaxMP(int maxMP) { this->maxMP = maxMP; }
	void setAttackDamag(int attackDamage) { this->attackDamage = attackDamage; }
};


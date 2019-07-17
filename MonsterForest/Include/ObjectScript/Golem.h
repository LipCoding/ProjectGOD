#pragma once

#include "Monster.h"
#include "GolemState.h"
PG_USING

class Golem :
	public Monster
{
	using GOLEMSTATE = pair<string, GolemState*>;
public:
	Golem();
	Golem(const Golem& goblin);
	~Golem();

	Vector3 initial_pos;
	Vector3 roaming_pos;
	bool returnToInitialPos{ false };
	float hearth_point{ 0 };
private:
	CTransform*		m_pTarget;
	CAnimation*		m_pAnimation;
	CNavigation3D*	m_pNavigation;
	bool            m_bEffectCheck = false;
	float           m_fEffectTime = 0.f;
protected:
	GolemState* pState{ nullptr };
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
	virtual void settingStatus(int current_hp, int current_mp, int level, int exp);
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
};


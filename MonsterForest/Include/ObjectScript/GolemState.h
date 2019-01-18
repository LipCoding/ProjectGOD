#pragma once

#include "GameObject/GameObject.h"

PG_USING

enum GOLEM_STATE
{
	GOLEM_STATE_NONE,
	GOLEM_STATE_IDLE,
	GOLEM_STATE_ATTACK,
	GOLEM_STATE_MOVE,
	GOLEM_STATE_DIE
};

class GolemState
{
protected:
	float progressTime;
	const type_info* pTypeID;
public:
	GolemState() {};
	virtual ~GolemState() {};

public:
	float getProgressTime() { return progressTime; }
	const type_info* getTypeID() const { return this->pTypeID; }

public:
	void setProgressTime(float time) { this->progressTime = time; }
	template<typename T>
	void setTypeID() { pTypeID = &typeid(T); }
public:
	virtual void enter(CGameObject* pGameObject) = 0;
	virtual void execute(CGameObject* pGameObject) = 0;
	virtual void exit(CGameObject* pGameObject) = 0;

};


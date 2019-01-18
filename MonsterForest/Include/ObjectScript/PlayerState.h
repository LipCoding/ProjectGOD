#pragma once

#include "GameObject/GameObject.h"

PG_USING

enum PLAYER_STATE
{
	PLAYER_STATE_NONE,
	PLAYER_STATE_IDLE,
	PLAYER_STATE_ATTACK,
	PLAYER_STATE_MOVE
};

class PlayerState
{
protected:
	float progressTime;
	const type_info* pTypeID;
public:
	PlayerState() {};
	virtual ~PlayerState() {};

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


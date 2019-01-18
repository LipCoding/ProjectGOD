#pragma once

#include "PlayerState.h"

class PlayerIdle :
	public PlayerState
{
public:
	PlayerIdle();
	~PlayerIdle();
public:
	virtual void enter(CGameObject* pGameObject);
	virtual void execute(CGameObject* pGameObject);
	virtual void exit(CGameObject* pGameObject);
};


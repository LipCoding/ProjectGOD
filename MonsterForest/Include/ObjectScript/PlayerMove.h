#pragma once

#include "PlayerState.h"

class PlayerMove :
	public PlayerState
{
public:
	PlayerMove();
	~PlayerMove();
public:
	virtual void enter(CGameObject* pGameObject);
	virtual void execute(CGameObject* pGameObject);
	virtual void exit(CGameObject* pGameObject);
};


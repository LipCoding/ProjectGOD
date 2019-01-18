#pragma once

#include "PlayerState.h"

class PlayerAttack :
	public PlayerState
{
public:
	PlayerAttack();
	~PlayerAttack();
public:
	virtual void enter(CGameObject* pGameObject);
	virtual void execute(CGameObject* pGameObject);
	virtual void exit(CGameObject* pGameObject);
};


#pragma once

#include "GolemState.h"

class GolemAttack : 
	public GolemState
{
public:
	GolemAttack();
	~GolemAttack();

public:
	virtual void enter(CGameObject* pGameObject);
	virtual void execute(CGameObject* pGameObject);
	virtual void exit(CGameObject* pGameObject);
};


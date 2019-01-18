#pragma once

#include "GolemState.h"

class GolemMove :
	public GolemState
{
public:
	GolemMove();
	~GolemMove();

public:
	virtual void enter(CGameObject* pGameObject);
	virtual void execute(CGameObject* pGameObject);
	virtual void exit(CGameObject* pGameObject);
};


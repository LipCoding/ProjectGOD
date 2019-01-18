#pragma once
#include "GolemState.h"
class GolemIdle :
	public GolemState
{
public:
	GolemIdle();
	~GolemIdle();
public:
	virtual void enter(CGameObject* pGameObject);
	virtual void execute(CGameObject* pGameObject);
	virtual void exit(CGameObject* pGameObject);
};


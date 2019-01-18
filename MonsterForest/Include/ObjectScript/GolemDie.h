#pragma once
#include "GolemState.h"
class GolemDie :
	public GolemState
{
public:
	GolemDie();
	~GolemDie();

public:
	virtual void enter(CGameObject* pGameObject);
	virtual void execute(CGameObject* pGameObject);
	virtual void exit(CGameObject* pGameObject);
};


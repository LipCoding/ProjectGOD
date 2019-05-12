#pragma once

#include "Component/Script.h"
#include "Component/Transform.h"
#include "Component/Animation.h"
#include "Component/Navigation3D.h"

PG_USING

class Monster :
	public CScript
{
protected:
	OBJECT_SET_TYPE objectSetType = OBJECT_SET_TYPE::NONE;
	bool dieState = false;
	int experience = 0;
public:
	Monster();
	~Monster();

public:
	void setExperience(int experience) { this->experience = experience; }
	virtual void setDieState(bool state) { this->dieState = state; }
	void setObjectSetType(OBJECT_SET_TYPE type) { objectSetType = type; }
	OBJECT_SET_TYPE getObjectSetType() { return objectSetType; }
};


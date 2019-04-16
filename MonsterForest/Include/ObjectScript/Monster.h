#pragma once

#include "Component/Script.h"
#include "Component/Transform.h"
#include "Component/Animation.h"
#include "Component/Navigation3D.h"

PG_USING

class Monster :
	public CScript
{
	OBJECT_SET_TYPE objectSetType = OBJECT_SET_TYPE::NONE;
public:
	Monster();
	~Monster();

public:
	void setObjectSetType(OBJECT_SET_TYPE type) { objectSetType = type; }
	OBJECT_SET_TYPE getObjectSetType() { return objectSetType; }
};


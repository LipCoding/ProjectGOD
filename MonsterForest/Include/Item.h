#pragma once

#include "Component/Script.h"

PG_USING

class Item :
	public CScript
{
public:
	Item();
	~Item();

public:
	bool initialize();


};


#pragma once

#include "global.h"

PG_USING

class QuestResourcesManager :
	Singleton<QuestResourcesManager>
{
	vector<CImage> reward_items_icon;
public:
	QuestResourcesManager();
	~QuestResourcesManager();
};


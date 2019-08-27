#pragma once

#include "PGEngine.h"
#include "Quest.h"
PG_USING

class QuestManager : 
	public Singleton<QuestManager>
{
	// 퀘스트 오브젝트를 들고 있는다.
	unordered_map<string, Quest*> quests;

public:
	QuestManager();
	~QuestManager();

public:
	bool initialize();

public:
	Quest* findQuest(const string& quest_name);

};


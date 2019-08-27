#pragma once

#include "PGEngine.h"
#include "Quest.h"
PG_USING

class QuestManager : 
	public Singleton<QuestManager>
{
	// ����Ʈ ������Ʈ�� ��� �ִ´�.
	unordered_map<string, Quest*> quests;

public:
	QuestManager();
	~QuestManager();

public:
	bool initialize();

public:
	Quest* findQuest(const string& quest_name);

};


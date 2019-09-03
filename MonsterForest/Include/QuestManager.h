#pragma once

#include "PGEngine.h"
#include "Quest.h"
#include "QuestBaseUI.h"
PG_USING

class QuestManager : 
	public Singleton<QuestManager>
{
	// 퀘스트 오브젝트를 들고 있는다.
	unordered_map<string, Quest*> quests;
	CGameObject* pQuestUIObject;
	QuestBaseUI* pQuestUIComponent;
	bool ui_show = false;
public:
	QuestManager();
	~QuestManager();

public:
	bool initialize();

public:
	Quest* findQuest(const string& quest_name);
	bool isShow() { return ui_show; }
	void enableShow(bool ui_show);
	CGameObject* getQuestUIObject() { return pQuestUIObject; };
	const unordered_map<string, Quest*>& getQuests() { return quests; }
	QuestBaseUI* getQuestUIComponent() { return pQuestUIComponent; }
};


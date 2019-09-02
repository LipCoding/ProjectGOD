#pragma once

#include "Component/Script.h"
#include "Component/UIPanel.h"
#include "Component/UIButton.h"
#include "GameObject/GameObject.h"
#include "Quest.h"
PG_USING

using REWARD_ITEM = pair<CGameObject*, CGameObject*>;
class QuestBaseUI :
	public CUIPanel
{
	vector<CGameObject*> quest_list;
	CGameObject* QuestUIBackGroundObject;
	CGameObject* quest_view_object;
	vector<REWARD_ITEM> reward_list;
	CGameObject* reward_item_text_object;
	Quest* current_quest;
	bool ui_show = false;

public:
	QuestBaseUI();
	~QuestBaseUI();

public:
	bool initialize();
	void updateQuestView();

public:
	void setCurrentQuest(Quest* current_quest) { this->current_quest = current_quest; }
	Quest* getCurrentQuest() { return this->current_quest; }
	vector<CGameObject*>& getQuestList() { return quest_list; }
	void enableShow(bool ui_show);
};


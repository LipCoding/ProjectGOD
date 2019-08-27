#pragma once

#include "Component/Script.h"
#include "Component/UIPanel.h"
#include "Component/UIButton.h"
#include "GameObject/GameObject.h"
#include "Quest.h"
PG_USING


class QuestBaseUI :
	public CUIPanel
{
	vector<CGameObject*> quest_list;
	CGameObject* quest_view_object;
	vector<Quest*> my_player_quests;
	Quest* current_quest;

public:
	QuestBaseUI();
	~QuestBaseUI();

public:
	bool initialize();
	void updateQuestView();

public:
	void setCurrentQuest(Quest* current_quest) { this->current_quest = current_quest; }
	Quest* getCurrentQuest() { return this->current_quest; }
};


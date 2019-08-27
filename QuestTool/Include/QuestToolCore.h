#pragma once

#include "global.h"
#include "GameObject/GameObject.h"
#include "Component/Font.h"
PG_USING

class QuestToolCore :
	public Singleton<QuestToolCore>
{
private:
	HINSTANCE hInstance;
	HWND hWnd;

	CGameObject* quest_ui_object;

	CFont* edit_quest_subject_text;
	HWND edit_quest_subject_handle;
	string edit_quest_subject;

	CFont* edit_quest_summary_text;
	HWND edit_quest_summary_handle;
	string edit_quest_summary;
	
	CFont* edit_quest_contents_text;
	HWND edit_quest_contents_handle;
	string edit_quest_contents;
	vector<string> contents_strings;
	
	HWND edit_minimum_level_handle;
	int edit_minimum_level;
	HWND edit_maximum_level_handle;
	int edit_maximum_level;
	HWND edit_reward_experience_handle;
	int edit_reward_experience;
	HWND edit_reward_gold_handle;
	int edit_reward_gold;

	HWND button_reward_items_add_handle;
	HWND button_reward_items_remove_handle;

	HWND listbox_reward_items_handle;
	vector<string> listbox_reward_items;

	HWND listbox_quest_lists;
	HWND button_quest_add_handle;
	HWND button_quest_remove_handle;

public:
	HWND button_quest_save_handle;
	HWND button_quest_load_handle;
	HWND scroll_item_render_offset_handle;
	HWND scroll_quest_contents_offset_handle;
	HWND combo_quest_type;
public:
	bool save(const string& save_file_name);
	bool load(const string& load_file_name);

public:
	bool initialize(HINSTANCE instance);

public:
	HWND getEditQuestSubjectHandle() { return edit_quest_subject_handle; }
	HWND getEditQuestSummaryHandle() { return edit_quest_summary_handle; }
	HWND getEditQuestContentsHandle() { return edit_quest_contents_handle; }
	HWND getEditMinimumLevelHandle() { return edit_minimum_level_handle; }
	HWND getEditMaximumLevelHandle() { return edit_maximum_level_handle; }
	HWND getEditRewardExperienceHandle() { return edit_reward_experience_handle; }
	HWND getEditRewardGoldHandle() { return edit_reward_gold_handle; }
	HWND getButtonRewardItemsAddHandle() { return button_reward_items_add_handle; }
	HWND getButtonRewardItemsRemoveHandle() { return button_reward_items_remove_handle; }
	HWND getListBoxRewardItemsHandle() { return listbox_reward_items_handle; }
	HWND getListBoxQuestListsHandle() { return listbox_quest_lists; }
	HWND getButtonQuestAddHandle() { return button_quest_add_handle; }
	HWND getButtonQuestRemoveHandle() { return button_quest_remove_handle; }


	string getEditQuestSubject() { return edit_quest_subject; }
	string getEditQuestSummary() { return edit_quest_summary; }
	string getEditQuestContents() { return edit_quest_contents; }
	int getEditMinimumLevel() { return  edit_minimum_level; }
	int getEditMaximumLevel() { return edit_maximum_level; }
	int getEditRewardExperience() { return  edit_reward_experience; }
	int getEditRewardGold() { return edit_reward_gold; }
	CGameObject* getQuestUIObject() { return quest_ui_object; }
	vector<string>& getRewardItems() { return listbox_reward_items; }
	vector<string>& getContentsStrings() { return contents_strings; }

	void setEditQuestSubject(const string& edit_quest_subject) { this->edit_quest_subject = edit_quest_subject; }
	void setEditQuestSummary(const string& edit_quest_summary) { this->edit_quest_summary = edit_quest_summary; }
	void setEditQuestContents(const string& edit_quest_contents) { this->edit_quest_contents = edit_quest_contents; }
	void setEditMinimumLevel(int edit_minimum_level) { this->edit_minimum_level = edit_minimum_level; }
	void setEditMaximumLevel(int edit_maximum_level) { this->edit_maximum_level = edit_maximum_level; }
	void setEditRewardExperience(int edit_reward_experience) { this->edit_reward_experience = edit_reward_experience; }
	void setEditRewardGold(int edit_reward_gold) { this->edit_reward_gold = edit_reward_gold; }
	void setQuestUIObject(CGameObject* quest_object) { this->quest_ui_object = quest_object; }

	void setEditQuestSubjectText(CFont* font_component) { edit_quest_subject_text = font_component; }
	void setEditQuestSummaryText(CFont* font_component) { edit_quest_summary_text = font_component; }
	void setEditQuestContentsText(CFont* font_component) { edit_quest_contents_text = font_component; }
	/*
	CFont* edit_quest_subject_text;
	CFont* edit_quest_summary_text;
	CFont* edit_quest_contents_text;
	*/
};

LRESULT clientProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
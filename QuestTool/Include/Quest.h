#pragma once

#include "global.h"

class Quest
{
	string quest_subject;
	string quest_summary;
	string quest_contents;
	int minimum_level;
	int maximum_level;
	int reward_experience;
	int reward_gold;
	vector<string> contents_strings;
	vector<string> reward_items;
public:
	Quest();
	~Quest();

public:
	bool initialize(const string& quest_subject, const string& quest_summary, const string& quest_contents, int minimum_level, int maximum_level,
		int reward_experience, int reward_gold, const vector<string>& reward_items);
	void update(float deltaTime);
public:
	const string& getQuestSubject() { return quest_subject; }
	const string& getQuestSummary() { return quest_summary; }
	const string& getQuestContents() { return quest_contents; }
	const vector<string>& getRewardItems() { return this->reward_items; }
	vector<string>& getContentStrings() { return this->contents_strings; }
	int getMinimumLevel() { return minimum_level; }
	int getMaximumLevel() { return maximum_level; }
	int getRewardExperience() { return reward_experience; }
	int getRewardGold() { return reward_gold; }
public:
	void setQuestSubject(const string& quest_subject) { this->quest_subject = quest_subject; }
	void setQuestSummary(const string& quest_summary) { this->quest_summary = quest_summary; }
	void setQuestContents(const string& quest_contents) { this->quest_contents = quest_contents; }
public:
	bool save(ofstream& save_file);
	bool load(ifstream& load_file);
};


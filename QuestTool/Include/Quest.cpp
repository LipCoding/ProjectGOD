#include "Quest.h"



Quest::Quest()
{
}


Quest::~Quest()
{
}

bool Quest::initialize(const string & quest_subject, const string & quest_summary, const string & quest_contents, int minimum_level, int maximum_level, int reward_experience, int reward_gold, const vector<string>& reward_items)
{
	this->quest_subject = quest_subject;
	this->quest_summary = quest_summary;
	this->quest_contents = quest_contents;
	this->minimum_level = minimum_level;
	this->maximum_level = maximum_level;
	this->reward_experience = reward_experience;
	this->reward_gold = reward_gold;
	copy(reward_items.begin(), reward_items.end(), back_inserter(this->reward_items));
	return true;
}

void Quest::update(float deltaTime)
{

}

bool Quest::save(const ofstream& save_file)
{
	return true;
}

bool Quest::load(const ifstream& load_file)
{
	return true;
}

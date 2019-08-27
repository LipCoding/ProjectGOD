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

bool Quest::save(ofstream& save_file)
{
	save_file << quest_subject << " ";
	save_file << quest_summary << " ";
	save_file << quest_contents << " ";
	save_file << minimum_level << " ";
	save_file << maximum_level << " ";
	save_file << reward_experience << " ";
	save_file << reward_gold << " ";

	save_file << reward_items.size() << " ";
	for (auto& reward_item : reward_items)
		save_file << reward_item << " ";

	return true;
}

bool Quest::load(ifstream& load_file)
{
	string file_string, temp_string;
	while (load_file >> file_string)
	{
		if (file_string == "#")
			break;
		temp_string += file_string + " ";

	}
	quest_subject = temp_string;
	temp_string.clear();
	while (load_file >> file_string)
	{
		if (file_string == "#")
			break;
		temp_string += file_string + " ";
	}
	quest_summary = temp_string;
	temp_string.clear();
	while (load_file >> file_string)
	{
		if (file_string == "#")
			break;
		temp_string += file_string + " ";
	}
	quest_contents = temp_string;
	temp_string.clear();

	load_file >> minimum_level;
	load_file >> maximum_level;
	load_file >> reward_experience;
	load_file >> reward_gold;

	int count;
	load_file >> count;
	for (int i = 0; i < count; ++i)
	{
		string item_name;
		load_file >> item_name;
		reward_items.push_back(item_name);
	}
	return true;
}

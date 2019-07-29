#pragma once

#include "global.h"

PG_USING
class Quest;

class UserInterfaceManager :
	public Singleton<UserInterfaceManager>
{
	/*
	퀘스트는 빨간색에서 녹색으로 표시될것.
	NPC는 ?를 표시하고 클릭하면 플레이어를 바라본다.
	퀘스트관련 UI가 표시.
	?와 퀘스트 제목이 표시.
	
	맵에서 세부맵(해당지역)을 확인하면 퀘스트관련 지역은 해당 퀘스트 넘버로 표시되게 됨.

	퀘스트 종류
	수집퀘스트,처치퀘스트,호위퀘스트

	
	*/
	list<Quest*> quests;
public:
	UserInterfaceManager();
	~UserInterfaceManager();

public:
	bool initialize();
	bool addQuest(const string& quest_subject, const string& quest_summary, const string& quest_contents, int minimum_level, int maximum_level,
		int reward_experience, int reward_gold, const vector<string>& reward_items);
	Quest* findQuest(const string& quest_subject);

public:
	bool save(const string& save_file_name);
	bool load(const string& load_file_name);
private:
	void GetFindFileList(char* pszDirectory, const char* pszFilter, std::vector <std::string> *vtList, int iLevel);
};


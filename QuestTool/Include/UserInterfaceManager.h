#pragma once

#include "global.h"

PG_USING
class Quest;

class UserInterfaceManager :
	public Singleton<UserInterfaceManager>
{
	/*
	����Ʈ�� ���������� ������� ǥ�õɰ�.
	NPC�� ?�� ǥ���ϰ� Ŭ���ϸ� �÷��̾ �ٶ󺻴�.
	����Ʈ���� UI�� ǥ��.
	?�� ����Ʈ ������ ǥ��.
	
	�ʿ��� ���θ�(�ش�����)�� Ȯ���ϸ� ����Ʈ���� ������ �ش� ����Ʈ �ѹ��� ǥ�õǰ� ��.

	����Ʈ ����
	��������Ʈ,óġ����Ʈ,ȣ������Ʈ

	
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


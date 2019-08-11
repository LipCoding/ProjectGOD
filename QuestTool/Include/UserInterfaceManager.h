#pragma once

#include "global.h"
#include "GameObject/GameObject.h"
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
	CGameObject* scroll_object;

	int scroll_offset{ 0 };
	
public:
	CGameObject* quest_exp_object;
	CGameObject* quest_gold_object;
	vector<wstring> contents_text_cont;
public:
	UserInterfaceManager();
	~UserInterfaceManager();

public:
	bool initialize();
	bool addQuest(const string& quest_subject, const string& quest_summary, const string& quest_contents, int minimum_level, int maximum_level,
		int reward_experience, int reward_gold, const vector<string>& reward_items);
	bool removeQuest(const string& quest_subject);
	Quest* findQuest(const string& quest_subject);
	const list<Quest*>& getQuests() { return quests; }
public:
	bool save(const string& save_file_name);
	bool load(const string& load_file_name);
private:
	void GetFindFileList(char* pszDirectory, const char* pszFilter, std::vector <std::string> *vtList, int iLevel);
	int getScrollOffset() { return scroll_offset; }
	void setScrollOffset(int scroll_offset) { this->scroll_offset = scroll_offset; }
};


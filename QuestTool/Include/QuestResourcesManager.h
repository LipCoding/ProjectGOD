#pragma once

#include "global.h"

PG_USING
using TEXTURE = pair<string, CImage*>;
class QuestResourcesManager :
	public Singleton<QuestResourcesManager>
{
public:
	vector<TEXTURE> reward_items_texture;
public:
	QuestResourcesManager();
	QuestResourcesManager(const QuestResourcesManager&) = delete;
	QuestResourcesManager(QuestResourcesManager&&) = delete;
	void operator=(const QuestResourcesManager&) = delete;
	void operator=(QuestResourcesManager&&) = delete;
	~QuestResourcesManager();
public:
	bool initialize();

public:
	CImage* findTexture(const string& texture_key);


};



#pragma once


#include "Scene/SceneScript.h"
#include <chrono>

using namespace chrono;
PG_USING

class QuestScene :
	public CSceneScript
{

public:
	QuestScene();
	~QuestScene();

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual void Input(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
};


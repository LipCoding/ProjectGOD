#pragma once
#include "Scene/SceneScript.h"

PG_USING

class CStartScene :
	public CSceneScript
{
public:
	CStartScene();
	~CStartScene();

public:
	virtual bool Init();
	virtual int Update(float fTime);

public:
	void StartCallback(float fTime);
	void ScheduleStartCallback(float fTime);
};


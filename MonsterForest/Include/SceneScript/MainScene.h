#pragma once

#include "Scene/SceneScript.h"

PG_USING

class CMainScene	:
	public CSceneScript
{
public:
	CMainScene();
	~CMainScene();

public:
	virtual bool Init();
	virtual int Update(float fTime);
};


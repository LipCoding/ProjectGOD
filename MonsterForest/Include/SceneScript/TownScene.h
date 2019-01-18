#pragma once
#include "Scene\SceneScript.h"

PG_USING

class CTownScene :
	public CSceneScript
{
public:
	CTownScene();
	~CTownScene();

public:
	virtual bool Init();
	virtual int Update(float fTime);
};


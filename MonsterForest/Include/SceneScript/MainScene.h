#pragma once

#include "Scene/SceneScript.h"
//#include "Font.h"
#include "GameObject/GameObject.h"
#include "Component/Font.h"
#include <chrono>
#include "Component/UIButton.h"
using namespace chrono;
PG_USING

class CMainScene :
	public CSceneScript
{
	bool isInitialize = false;
public:
	CMainScene();
	~CMainScene();

	bool isInitComplete = false;
	bool move_enable = false;
	float move_time = 0.f;
public:
public:
	void chat_callback(float fTime);

private:
	bool m_isCheckColliderQuadTree = false;
	bool m_isCheckColliderNaviMesh = false;

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual void Input(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
};


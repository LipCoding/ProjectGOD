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
	//chrono::steady_clock::time_point inputTime;
	//Vector3 prevPos;

	//CGameObject* pPlayer;
	bool isInitComplete = false;
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


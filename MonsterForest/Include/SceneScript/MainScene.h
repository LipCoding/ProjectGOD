#pragma once

#include "Scene/SceneScript.h"
//#include "Font.h"
#include "GameObject/GameObject.h"
#include "Component/Font.h"
#include <chrono>
using namespace chrono;
PG_USING

class CMainScene :
	public CSceneScript
{
public:
	CMainScene();
	~CMainScene();
	chrono::steady_clock::time_point inputTime;
	Vector3 prevPos;

	// id edit
	//CFont* testFont;
	//WCHAR testString[128];
	CGameObject* pPlayer;
	//	CText* pUIEditText;
	//	wstring pEditIDString;
	//	CGameObject* pEditIDObject;
	//	vector<WORD> ID_inputCont;
	//
	//
	//	//// pw edit
	//	CText* pUIEditText_PW;
	//	wstring pEditPWString;
	//	CGameObject* pEditPWObject;
	//	vector<WORD> PW_inputCont;
	//public:
	//	bool id_write = false;
	//	bool pw_write = false;
	//
	//public:
	//	void login_ID_callback(float fTime);
	//	void login_PW_callback(float fTime);
public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual void Input(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
};


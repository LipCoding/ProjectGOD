#pragma once
#include "Scene/SceneScript.h"
#include "GameObject/GameObject.h"
#include "Component/Font.h"
PG_USING

class LoginScene :
	public CSceneScript
{
public:
	LoginScene();
	~LoginScene();
	unordered_map<string, vector<CGameObject*>> UICont;
public:
	virtual bool Init();
	virtual int Update(float fTime);
	CFont* pUIEditText = nullptr;
	wstring pEditIDString;
	//CGameObject* pEditIDObject;
	vector<WORD> ID_inputCont;


	//// pw edit
	CFont* pUIEditText_PW = nullptr;
	wstring pEditPWString;
	//CGameObject* pEditPWObject;
	vector<WORD> PW_inputCont;

public:
	bool id_write = false;
	bool pw_write = false;

public:
	void gameConnectCallBack(float time);
	void closeGameConnectUICallBack(float time);
	void loginIDEditCallback(float fTime);
	void loginPWEditCallback(float fTime);
	void login_JOIN_callback(float fTime);
public:
	void connectGameServer(float fTime);
	void ScheduleStartCallback(float fTime);
};


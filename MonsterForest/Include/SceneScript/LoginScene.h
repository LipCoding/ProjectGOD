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

public:
	virtual bool Init();
	virtual int Update(float fTime);

	CFont* pUIEditText;
	wstring pEditIDString;
	CGameObject* pEditIDObject;
	vector<WORD> ID_inputCont;


	//// pw edit
	CFont* pUIEditText_PW;
	wstring pEditPWString;
	CGameObject* pEditPWObject;
	vector<WORD> PW_inputCont;

public:
	bool id_write = false;
	bool pw_write = false;

public:
	void login_ID_callback(float fTime);
	void login_PW_callback(float fTime);
	void login_JOIN_callback(float fTime);
public:
	void StartCallback(float fTime);
	void ScheduleStartCallback(float fTime);
};


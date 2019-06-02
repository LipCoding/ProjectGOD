#pragma once

#include "Scene/SceneScript.h"
#include "GameObject/GameObject.h"

PG_USING

class CTestScene :
	public CSceneScript
{
public:
	CTestScene();
	~CTestScene();

	CGameObject *pPlayer;

private:
	bool	m_isCheckColliderQuadTree = false;
	bool	m_isCheckColliderNaviMesh = false;

	bool	m_isFirstCheck = true;
public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual void Input(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
};


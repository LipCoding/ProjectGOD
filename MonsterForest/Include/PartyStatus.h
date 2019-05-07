#pragma once

#include "Component/Script.h"
#include "Component/UIPanel.h"
#include "Component/UIButton.h"
PG_USING

class PartyStatus :
	public CUIButton
{
	// ���� ĳ���͸� ������
	// ĳ���� �ɷ�ġ�� ��ġ�� ������.
	CUIButton*  pUIHearthBarBackground;
	CUIButton*	pUIHearthBar;

	CUIButton*  pUIManaBarBackground;
	CUIButton*	pUIManaBar;

	CUIButton*  pUIPureBarBackground;
	CUIButton*	pUIPureBar;
	//CUIButton*  pUIManaBar;
	float scale = 1.f;
	string playerName;
	bool used = false;

public:
	void setUsed(bool used) { this->used = used; }
	bool getUsed() { return used; }

public:

	PartyStatus();
	~PartyStatus();
public:
	bool initialize();
public:
	void setScale(float ratio) { this->scale = ratio; }
	float getScale() { return this->scale; }
	CUIButton* getUIHearthBar() { return pUIHearthBar; }
	void setUIHeearthBar(CUIButton* pUIHearthBar) { this->pUIHearthBar = pUIHearthBar; }
	void setPlayerName(const string& playerName) { this->playerName = playerName; }
public:
	void enableRender(bool show);
	string getPlayerName() { return playerName; }
public:
	virtual void OnCollisionEnter(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollision(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollisionLeave(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
};


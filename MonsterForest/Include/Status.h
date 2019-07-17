#pragma once

#include "Component/Script.h"
#include "Component/UIPanel.h"
#include "Component/UIButton.h"
PG_USING

class CPlayer;
class Actor;

class Status :
	public CUIButton
{
	// ���� ĳ���͸� ������
	// ĳ���� �ɷ�ġ�� ��ġ�� ������.
	Actor* target_component{nullptr};
	CUIButton*  pUIHearthBarBackground;
	CUIButton*	pUIHearthBar;

	CUIButton*  pUIManaBarBackground;
	CUIButton*	pUIManaBar;

	CUIButton*  pUIPureBarBackground;
	CUIButton*	pUIPureBar;
	//CUIButton*  pUIManaBar;
	float scale = 1.f;
public:

	Status();
	~Status();
public:
	bool initialize();
	void update(float deltaTime);
public:
	void setTarget(Actor* target_component) { this->target_component = target_component; }
	void setScale(float ratio) { this->scale = ratio; }
	float getScale() { return this->scale; }
	CUIButton* getUIHearthBar() { return pUIHearthBar; }
	void setUIHeearthBar(CUIButton* pUIHearthBar) { this->pUIHearthBar = pUIHearthBar; }
	CUIButton* getUIManaBar() { return pUIManaBar; }
	CUIButton* getUIPureBar() { return pUIPureBar; }
	
public:
	void enableRender(bool show);
public:
	virtual void OnCollisionEnter(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollision(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollisionLeave(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
};


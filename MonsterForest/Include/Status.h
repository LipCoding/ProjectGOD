#pragma once

#include "Component/Script.h"
#include "Component/UIPanel.h"
#include "Component/UIButton.h"
PG_USING

class Status :
	public CUIButton
{
	// 현재 캐릭터를 렌더링
	// 캐릭터 능력치를 수치로 보여줌.
	CUIButton*  pUIHearthBarBackground;
	CUIButton*	pUIHearthBar;

	CUIButton*  pUIManaBarBackground;
	CUIButton*	pUIManaBar;

	CUIButton*  pUIPureBarBackground;
	CUIButton*	pUIPureBar;
	//CUIButton*  pUIManaBar;
public:

	Status();
	~Status();
public:
	bool initialize();
public:
	CUIButton* getUIHearthBar() { return pUIHearthBar; }
	void setUIHeearthBar(CUIButton* pUIHearthBar) { this->pUIHearthBar = pUIHearthBar; }
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


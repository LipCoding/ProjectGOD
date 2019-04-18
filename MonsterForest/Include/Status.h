#pragma once

#include "Component/Script.h"
#include "Component/UIPanel.h"
#include "Component/UIButton.h"
PG_USING

class Status :
	public CUIButton
{
	// ���� ĳ���͸� ������
	// ĳ���� �ɷ�ġ�� ��ġ�� ������.
	CUIButton*  pUIHearthBarBackground;
	CUIButton*	pUIHearthBar;

	CUIButton*  pUIManaBarBackground;
	CUIButton*	pUIManaBar;

	CUIButton*  pUISkillBarBackground;

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
	virtual void OnCollisionEnter(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollision(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollisionLeave(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
};


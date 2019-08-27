#pragma once

#include "Component/Script.h"
#include "Component/UIPanel.h"
#include "Component/UIButton.h"
#include "GameObject/GameObject.h"
#include "Quest.h"

PG_USING

class QuestItem :
	public CUIButton
{
	QuestBaseUI* pBaseUI;
	Quest* quest;
public:
	QuestItem();
	~QuestItem();
public:
	void setQuestBaseUI(QuestBaseUI* pBaseUI) { this->pBaseUI = pBaseUI; }
	QuestBaseUI* getQuestBaseUI() { return pBaseUI; }
	void setQuest(Quest* quest) { this->quest = quest; }
	Quest* getQuest() { return quest; }
public:
	virtual void OnCollisionEnter(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollision(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollisionLeave(class CCollider* pSrc, class CCollider* pDest,
		float fTime);



};


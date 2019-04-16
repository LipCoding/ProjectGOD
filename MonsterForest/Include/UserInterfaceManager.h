#pragma once

#include "PGEngine.h"
#include "Component/UIButton.h"
#include "Component/Font.h"

PG_USING

class UserInterfaceManager
{
#pragma region framerender
	CFont* pFrameText;
	wstring frameString;
	CGameObject* pFrameRender;
	vector<WORD> pFrameCont;
#pragma endregion

	CUIButton*	pUIHearthBar;
	CUIButton*	pEnemyUIHearthBar;


	class Inventory* pInventory;
	class MiniMap* pMiniMap;
	class WorldMap* pWorldMap;
	class Status* pStatus;
	class Chatting* pChatting;
	class QuickSlot* pQuickSlot;

public:
	bool initialize();
	void update(float time);
public:
	CUIButton* getUIHeartBar() { return pUIHearthBar; }
	CUIButton* getEnemyUIHearthBar() { return pEnemyUIHearthBar; }

	void setUIHearthBar(CUIButton* ui) { this->pUIHearthBar = ui; }
	void setEnemyUIHearthBar(CUIButton* ui) { this->pEnemyUIHearthBar = ui; }
public:
	Inventory* getInventory() { return this->pInventory; }
	MiniMap* getMiniMap() { return this->pMiniMap; }
	WorldMap* getWorldMap() { return this->pWorldMap; }
	Status* getStatus() { return this->pStatus; }
	Chatting* getChatting() { return this->pChatting; }
	QuickSlot* getQuickSlot() { return this->pQuickSlot; }


	DECLARE_SINGLE(UserInterfaceManager);
};


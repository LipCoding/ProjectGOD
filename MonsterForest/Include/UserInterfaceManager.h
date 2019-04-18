#pragma once

#include "PGEngine.h"
#include "Component/UIButton.h"
#include "Component/Font.h"
#include "Status.h"
PG_USING

class UserInterfaceManager
{
#pragma region framerender
	CFont* pFrameText;
	wstring frameString;
	CGameObject* pFrameRender;
	vector<WORD> pFrameCont;
#pragma endregion

	//CUIButton*	pUIHearthBar;
	//CUIButton*	pEnemyUIHearthBar;


	class Inventory* pInventory;
	class MiniMap* pMiniMap;
	class WorldMap* pWorldMap;
	Status* pStatus;
	Status* pEnemyStatus;
	class Chatting* pChatting;
	class QuickSlot* pQuickSlot;

	class CPlayer* pPlayer = nullptr;

public:
	bool initialize();
	void update(float time);
public:
	CUIButton* getUIHeartBar() { return this->pStatus->getUIHearthBar(); }
	CUIButton* getEnemyUIHearthBar() { return this->pEnemyStatus->getUIHearthBar(); }
	class CPlayer* getPlayer() { return this->pPlayer; }

	void setUIHearthBar(CUIButton* ui) { this->pStatus->setUIHeearthBar(ui); }
	void setEnemyUIHearthBar(CUIButton* ui) { this->pEnemyStatus->setUIHeearthBar(ui);}
	void setPlayer(class CPlayer* pPlayer) { this->pPlayer = pPlayer; }
public:
	Inventory* getInventory() { return this->pInventory; }
	MiniMap* getMiniMap() { return this->pMiniMap; }
	WorldMap* getWorldMap() { return this->pWorldMap; }
	Status* getStatus() { return this->pStatus; }
	Chatting* getChatting() { return this->pChatting; }
	QuickSlot* getQuickSlot() { return this->pQuickSlot; }


	DECLARE_SINGLE(UserInterfaceManager);
};


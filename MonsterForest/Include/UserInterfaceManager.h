#pragma once

#include "PGEngine.h"
#include "Component/UIButton.h"
#include "Component/Font.h"
#include "Status.h"

PG_USING

// �÷��̾�� Ÿ�ٿ� ���� ������ ���´�.
// �� ������Ʈ���� �ش� Ÿ�ٿ� ���ؼ� ������ ������Ʈ�Ͽ� ü�¹ٸ� �����Ѵ�.

class UserInterfaceManager
{
#pragma region framerender
	CFont* pFrameText;
	wstring frameString;
	CGameObject* pFrameRender;
	vector<WORD> pFrameCont;
#pragma endregion
	array<class PartyStatus*, 4> partyCont;

	class Inventory* pInventory;
	class MiniMap* pMiniMap;
	class WorldMap* pWorldMap;
	Status* pStatus;
	Status* pEnemyStatus;
	class Chatting* pChatting;
	class QuickSlot* pQuickSlot;
	class DropTableUI* pDropTableUI;
	class TargetPlayerUI* pTargetPlayerUI;
	class CPlayer* pPlayer = nullptr;
	class PGMessageBox* pPGMessageBox= nullptr;
	class Actor* pTarget = nullptr;

public:
	bool initialize();
	void update(float time);

public:
	PartyStatus* findPartyState(const string& playerName);

	void pushChatMessage(const wstring& chat_message);
	void partySyncronize(const string& object_tag, int x, int y, int z, int current_hp, int current_mp, int level, int exp);

	

public:
	CUIButton* getUIHeartBar() { return this->pStatus->getUIHearthBar(); }
	CUIButton* getEnemyUIHearthBar() { return this->pEnemyStatus->getUIHearthBar(); }
	class CPlayer* getPlayer() { return this->pPlayer; }
	class TargetPlayerUI* getTargetPlayerUI() { return this->pTargetPlayerUI; }
	void setUIHearthBar(CUIButton* ui) { this->pStatus->setUIHeearthBar(ui); }
	void setEnemyUIHearthBar(CUIButton* ui) { this->pEnemyStatus->setUIHeearthBar(ui);}
	void setPlayer(class CPlayer* pPlayer);
	void setTarget(class Actor* pTarget);
	void setCameraTarget(CGameObject* target_object);

public:
	class Actor* getTarget() { return pTarget; }
	void addPartyPlayer(const string& playerName);
	void removePartyPlayer(const string& playerName);
	Inventory* getInventory() { return this->pInventory; }
	MiniMap* getMiniMap() { return this->pMiniMap; }
	WorldMap* getWorldMap() { return this->pWorldMap; }
	Status* getStatus() { return this->pStatus; }
	Status* getEnemyStatus() { return this->pEnemyStatus; }
	Chatting* getChatting() { return this->pChatting; }
	QuickSlot* getQuickSlot() { return this->pQuickSlot; }
	DropTableUI* getDropTableUI() { return this->pDropTableUI; }
	PGMessageBox* getPGMessageBox() { return this->pPGMessageBox; }
	

	DECLARE_SINGLE(UserInterfaceManager);
};


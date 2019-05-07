#pragma once

#include "Component/Script.h"
#include "Component/UIPanel.h"
#include "Component/UIButton.h"

PG_USING

class TargetPlayerUI :
	public CUIPanel
{
	class UserInfoUI* pUserInfoUI;
	class InviteToParty* pInviteToPartyUI;
	class TradeUI* pTradeUI;
	int playerNumber = -1;

	int fromID{ -1 };
	int toID{ -1 };
public:
	TargetPlayerUI();
	~TargetPlayerUI();

public:
	void initialize();
	int Update(float fTime);
public:
	void enableRender(bool show);

public:
	int getFromID() { return fromID; }
	int getToID() { return toID; }
	void setFromID(int id) { this->fromID = id; }
	void setToID(int id) { this->toID = id; }
	class UserInfoUI* getUserInfoUI() { return pUserInfoUI; }
	class InviteToParty* getInviteToPartyUI() { return pInviteToPartyUI; }
	class TradeUI* getTradeUI() { return pTradeUI; }
	int getPlayerNumber() { return playerNumber; }
	void setPlayerNumber(int playerNumber) { this->playerNumber = playerNumber; }

};


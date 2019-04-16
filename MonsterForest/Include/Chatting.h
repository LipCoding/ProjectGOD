#pragma once

#include "Scene/SceneScript.h"
#include "GameObject/GameObject.h"
#include "Component/Font.h"
#include "Component/UIButton.h"

PG_USING

class Chatting
{
#pragma region ChattingEditor
	CFont* pUIChatText;
	wstring chatString;
	CGameObject* pUIChat;
	vector<WORD> pChatCont;
#pragma endregion
#pragma region ChattingLog
	CFont* pUIChatLogText;
	CGameObject* pUIChatLog;
#pragma endregion
	bool chat_write = false;
	vector<wstring> UserChatLog;
public:
	Chatting();
	~Chatting();

public:
	bool initialize();

public:
	vector<wstring>& getUserChatLogCont() { return this->UserChatLog; }
	CFont* getUIChatLogText() { return this->pUIChatLogText; }
public:
	CFont* getUIChatText() { return this->pUIChatText; }
	wstring& getChatString() { return this->chatString; }
	CGameObject* getUIChat() { return this->pUIChat; }
	vector<WORD>& getChatCont() { return this->pChatCont; }

public:
	bool isChatWrite() { return this->chat_write; }
	void setChatString(wstring& chatString) { this->chatString = chatString; }
	void enableChatWrite(bool write) { this->chat_write = write; }
};


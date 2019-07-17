#pragma once

#include "PGEngine.h"
#include "GameObject/GameObject.h"
#include "Component/Font.h"
#include <chrono>
#include "Component/UIButton.h"

PG_USING

class NetworkManager :
	public Singleton<NetworkManager>
{
private:
	WSADATA wsaData; // 윈도우즈에서 서버를 하기 위한 정보를 담는 구조체.
	SOCKET sock{ 0 }; // 서버와 통신 목적의 소켓.
	SOCKADDR_IN serverAddr;
	DWORD inPacketSize { 0 };
	int savedPacketSize { 0 };
	size_t myClientID { 0 };
	bool isConnected { false };
	thread* pRecvThread { nullptr };
	WSABUF	send_wsabuf;
	WSABUF	recv_wsabuf;
	char sendBuffer[MAX_BUFF_SIZE];
	char recvBuffer[MAX_BUFF_SIZE];
	char packetBuffer[MAX_BUFF_SIZE];
	static queue<void*> clientPacketQueue;
	string serverip_address;

public:
	chrono::steady_clock::time_point inputTime;

public:
	static CGameObject* pPlayer;
	queue<void*>& getClientPacketQueue() { return clientPacketQueue; }
	static void  popData() { clientPacketQueue.pop(); }
	void enableConnected(bool connected);
	void setMyClientID(size_t id);
	void connectMainServer();
public:
	NetworkManager();
	NetworkManager(const NetworkManager&) = delete;
	NetworkManager(const NetworkManager&&) = delete;
	void operator=(const NetworkManager&) = delete;
	void operator=(const NetworkManager&&) = delete;
	~NetworkManager();

public:
	bool initialize();
	void shutDownServer();
	void processPacket(void* ptr);
	void readPacket(SOCKET sock);

	/// getter function
public:
	const SOCKET& getSocket();
	char* getSendBuffer();
	const char* getRecvBuffer();
	const char* getPacketBuffer();
	const DWORD getInPacketSize();
	const int getSavedPacketSize();
	const size_t getMyClientID();
	const bool getIsConnected();
	WSABUF& getSendWsaBuf();
	WSABUF& getRecvWsaBuf();
	/// setter function
public:
	void setPlayer(CGameObject* pPlayer);
};
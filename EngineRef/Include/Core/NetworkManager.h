//#pragma once
//
//#include "../PGEngine.h"
//#include "../Singleton.h"
//#include "../Singleton.cpp"
//
//PG_BEGIN
//
//class PG_DLL NetworkManager :
//	public Singleton<NetworkManager>
//{
//private:
//	WSADATA wsaData; // 윈도우즈에서 서버를 하기 위한 정보를 담는 구조체.
//	SOCKET sock; // 서버와 통신 목적의 소켓.
//	SOCKADDR_IN serverAddr;
//	DWORD inPacketSize;
//	int savedPacketSize = 0;
//	size_t myClientID;
//	bool isConnected;
//	thread* pRecvThread;
//	WSABUF	send_wsabuf;
//	WSABUF	recv_wsabuf;
//	char sendBuffer[MAX_BUFF_SIZE];
//	char recvBuffer[MAX_BUFF_SIZE];
//	char packetBuffer[MAX_BUFF_SIZE];
//	queue<void*> clientPacketQueue;
//	string serverip_address;
//	//queue<void*> clientPacketQueue;
//public:
//	void* fetchToClientPacket();
//	//void* fetchToClientPacket();
//	queue<void*>& getClientPacketQueue() { return clientPacketQueue; }
//	void  popData() { clientPacketQueue.pop(); }
//	void enableConnected(bool connected);
//	void setMyClientID(size_t id);
//public:
//	NetworkManager();
//	NetworkManager(const NetworkManager&) = delete;
//	NetworkManager(const NetworkManager&&) = delete;
//	void operator=(const NetworkManager&) = delete;
//	void operator=(const NetworkManager&&) = delete;
//	~NetworkManager();
//
//public:
//	bool initialize();
//	void shutDownServer();
//	void processPacket(char* ptr);
//	void readPacket(SOCKET sock);
//
//	/// getter function
//public:
//	const SOCKET& getSocket();
//	char* getSendBuffer();
//	const char* getRecvBuffer();
//	const char* getPacketBuffer();
//	const DWORD getInPacketSize();
//	const int getSavedPacketSize();
//	const size_t getMyClientID();
//	const bool getIsConnected();
//	WSABUF& getSendWsaBuf();
//	WSABUF& getRecvWsaBuf();
//	/// setter function
//public:
//
//	//private:
//		//static void recvThread();
//
//};
//
//PG_END
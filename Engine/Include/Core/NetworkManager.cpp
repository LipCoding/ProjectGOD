#include "NetworkManager.h"
#include "../Core.h"
#include "PathManager.h"
PG_USING

NetworkManager::NetworkManager() :
	sock{ 0 },
	inPacketSize{ 0 },
	savedPacketSize{ 0 },
	myClientID{ 0 },
	isConnected{ false },
	pRecvThread{ nullptr }
{
}

void * NetworkManager::fetchToClientPacket()
{
	void* packet = NULL;

	if (false == clientPacketQueue.empty())
		packet = clientPacketQueue.front();


	return packet;
}

NetworkManager::~NetworkManager()
{
}

bool NetworkManager::initialize()
{
	// 파일로부터 ip주소를 읽어들인다.
	//

	string path = GET_SINGLE(CPathManager)->FindPathToMultiByte(DATA_PATH);
	path += "serverip.txt";
	ifstream in(path.c_str());
	in >> serverip_address;



	inPacketSize = 0;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

	if (sock == INVALID_SOCKET)
	{
		//에러처리.
	}

	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(serverip_address.c_str());
	serverAddr.sin_port = htons(SERVER_PORT);

	int retval = WSAConnect(sock, (sockaddr *)&serverAddr, sizeof(serverAddr), NULL, NULL, NULL, NULL);

	WSAAsyncSelect(sock, GET_SINGLE(CCore)->GetWindowHandle(), WM_SOCKET, FD_CLOSE | FD_READ);

	send_wsabuf.buf = sendBuffer;
	send_wsabuf.len = MAX_BUFF_SIZE;
	recv_wsabuf.buf = recvBuffer;
	recv_wsabuf.len = MAX_BUFF_SIZE;

	if (retval == SOCKET_ERROR)
	{
		MessageBox(NULL, L"서버와의 연결에 실패하였습니다.", L"서버에러", MB_OK);
	}

	//pRecvThread = new thread{ recvThread };

	return true;
}

void NetworkManager::shutDownServer()
{
	//pRecvThread->join();
	//delete pRecvThread;
	closesocket(sock);
	WSACleanup();
}

void NetworkManager::processPacket(char * ptr)
{
	void* packet = (void*)malloc(sizeof(char) * ((unsigned short*)ptr)[0]);
	memcpy(packet, ptr, ((unsigned short*)ptr)[0]);

	clientPacketQueue.emplace(move(packet));
}

void NetworkManager::readPacket(SOCKET sock)
{
	DWORD iobyte, ioflag = 0;

	//int ret = recv(sock, recvBuffer, MAX_BUFF_SIZE, 0);
	int ret = WSARecv(sock, &recv_wsabuf, 1, &iobyte, &ioflag, NULL, NULL);

	if (ret)
	{
		int err_code = WSAGetLastError();
		// 에러처리.
	}

	BYTE* ptr = reinterpret_cast<BYTE*>(recvBuffer);
	//(int)((unsigned short*)ptr)[0]
	while (0 != iobyte)
	{
		if (0 == inPacketSize)
			inPacketSize = ((unsigned short*)ptr)[0];
		if (iobyte + savedPacketSize >= inPacketSize)
		{
			memcpy(packetBuffer + savedPacketSize, ptr, inPacketSize - savedPacketSize);

			processPacket(packetBuffer);

			ptr += inPacketSize - savedPacketSize;
			iobyte -= inPacketSize - savedPacketSize;

			inPacketSize = 0;
			savedPacketSize = 0;
		}
		else
		{
			memcpy(packetBuffer + savedPacketSize, ptr, iobyte);
			savedPacketSize += iobyte;
			iobyte = 0;
		}
	}
}

const SOCKET & NetworkManager::getSocket() { return sock; }
char * NetworkManager::getSendBuffer() { return sendBuffer; }
const char * NetworkManager::getRecvBuffer() { return recvBuffer; }
const char * NetworkManager::getPacketBuffer() { return packetBuffer; }
const DWORD NetworkManager::getInPacketSize() { return inPacketSize; }
const int NetworkManager::getSavedPacketSize() { return savedPacketSize; }
const size_t NetworkManager::getMyClientID() { return myClientID; }
const bool NetworkManager::getIsConnected() { return isConnected; }

WSABUF & NetworkManager::getSendWsaBuf()
{
	return this->send_wsabuf;
}

WSABUF & NetworkManager::getRecvWsaBuf()
{
	return this->recv_wsabuf;
}

void NetworkManager::enableConnected(bool connected)
{
	this->isConnected = connected;
}
void NetworkManager::setMyClientID(size_t id)
{
	this->myClientID = id;
}

//void NetworkManager::recvThread()
//{
//	SOCKET sock = NetworkManager::getInstance()->getSocket();
//
//	while (1)
//	{
//		NetworkManager::getInstance()->readPacket(sock);
//	}
//}
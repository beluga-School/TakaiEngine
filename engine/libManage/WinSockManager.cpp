#include "WinSockManager.h"

void WinSockManager::PreWinSockStart(int portNum,const char* ipAdress) 
{
	WSAStartup(MAKEWORD(1, 1), &WinSockDateStorage);
	ServerInfo.sin_family = AF_INET;
	ServerInfo.sin_port = htons(portNum);
	ServerInfo.sin_addr.S_un.S_addr = inet_addr(ipAdress);
	ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	ConnectSocket = socket(AF_INET, SOCK_STREAM, 0);
}

void WinSockManager::PreAsync()
{
	acceptSwitch = WSACreateEvent();
	WSAEventSelect(ListenSocket, acceptSwitch, FD_ACCEPT | FD_CONNECT);
	WSAEventSelect(ConnectSocket, acceptSwitch, FD_READ | FD_WRITE);
}

void WinSockManager::End()
{
	shutdown(CommunicationSocket, 2);
	closesocket(CommunicationSocket);
	WSACleanup();
}

void WinSockManager::Listen()
{
	bind(ListenSocket, (SOCKADDR*)&ServerInfo, sizeof(ServerInfo));
	listen(ListenSocket, 0);
}

void WinSockManager::MySend(const char* msg)
{
	send(CommunicationSocket, msg, strlen(msg), 0);
}

void WinSockManager::MySendClinet(const char* msg)
{
	send(ConnectSocket, msg, strlen(msg), 0);
}

void WinSockManager::MyAccept()
{
	CommunicationSocket = accept(ListenSocket, (SOCKADDR*)&ClientInfo, &ClientInfoLEN);
	closesocket(ListenSocket);
}

void WinSockManager::MyConnect()
{
	connect(ConnectSocket, (struct sockaddr*)&ServerInfo, sizeof(ServerInfo));
}

void WinSockManager::MyRecv()
{
	recvRETURN = recv(CommunicationSocket, DateBuffer, sizeof(DateBuffer) - 1, 0);
	if (recvRETURN == -1) recvRETURN = 0;
	if (DateBuffer[recvRETURN] != '\0')	DateBuffer[recvRETURN] = '\0';
}

void WinSockManager::MyRecvClient()
{
	recvRETURN = recv(ConnectSocket, DateBuffer, sizeof(DateBuffer) - 1, 0);
	if (recvRETURN == -1) recvRETURN = 0;
	if (DateBuffer[recvRETURN] != '\0')	DateBuffer[recvRETURN] = '\0';
}

bool WinSockManager::AcceptJudgment()
{
	return WaitForSingleObject(acceptSwitch, 0) == WAIT_OBJECT_0;
}

char WinSockManager::GetMessageDate()
{
	return *DateBuffer;
}

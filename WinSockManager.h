#pragma once
#pragma comment(lib,"Ws2_32.lib")
#pragma warning(disable:4996)
#include <WinSock2.h>

class WinSockManager
{
private:
	WSADATA WinSockDateStorage = { 0 };	//WinSockのバージョンとか
	SOCKET ListenSocket = { 0 };		//ホスト用:受け付けソケット
	SOCKET CommunicationSocket = { 0 };	//ホスト用:やり取りソケット
	SOCKET ConnectSocket = { 0 };		//クライアント用ソケット
	SOCKADDR_IN ServerInfo = { 0 };		//ホスト側の情報
	SOCKADDR_IN ClientInfo = { 0 };		//クライアント側の情報
	WSAEVENT acceptSwitch;				//非同期受付用イベント

public:
	char DateBuffer[1024] = { 0 };		//送られた文字列を保管する
	int recvRETURN = 0;					//送られた文字数を保管する
	int ClientInfoLEN = (int)sizeof(ClientInfo);//acceptにいるやつ
	
	//でかめの処理をまとめたやつ
	void PreWinSockStart(int portNum, const char* ipAdress);
	void PreAsync();
	void End();

	//小さめの処理を使いやすくしたやつ
	void MyAccept();
	void MySend(const char* msg);
	void MySendClinet(const char* msg);
	void MyRecv();
	void MyRecvClient();
	void Listen();
	void MyConnect();
	bool AcceptJudgment();
	char GetMessageDate();

	static WinSockManager* GetInstance() {
		static WinSockManager instance;
		return &instance;
	};

private:
	WinSockManager();
	~WinSockManager() {};

	WinSockManager(const WinSockManager& a) = delete;
	WinSockManager& operator=(const WinSockManager&) = delete;
};


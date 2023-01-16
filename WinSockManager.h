#pragma once
#pragma comment(lib,"Ws2_32.lib")
#pragma warning(disable:4996)
#include <WinSock2.h>

class WinSockManager
{
private:
	WSADATA WinSockDateStorage = { 0 };	//WinSock�̃o�[�W�����Ƃ�
	SOCKET ListenSocket = { 0 };		//�z�X�g�p:�󂯕t���\�P�b�g
	SOCKET CommunicationSocket = { 0 };	//�z�X�g�p:�����\�P�b�g
	SOCKET ConnectSocket = { 0 };		//�N���C�A���g�p�\�P�b�g
	SOCKADDR_IN ServerInfo = { 0 };		//�z�X�g���̏��
	SOCKADDR_IN ClientInfo = { 0 };		//�N���C�A���g���̏��
	WSAEVENT acceptSwitch;				//�񓯊���t�p�C�x���g

public:
	char DateBuffer[1024] = { 0 };		//����ꂽ�������ۊǂ���
	int recvRETURN = 0;					//����ꂽ��������ۊǂ���
	int ClientInfoLEN = (int)sizeof(ClientInfo);//accept�ɂ�����
	
	//�ł��߂̏������܂Ƃ߂����
	void PreWinSockStart(int portNum, const char* ipAdress);
	void PreAsync();
	void End();

	//�����߂̏������g���₷���������
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


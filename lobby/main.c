#pragma comment(lib, "ws2_32")
#include "include.h"
#define SERVERPORT	22122

// Queue�� front & rear
WORK* workfront = NULL;
WORK* workrear = NULL;

int main(int argc, char* argv[])
{
	SOCKET sock;
	SOCKADDR_IN srvAddr, cltAddr;
	int cltAddrSize = sizeof(cltAddr);
	MESSAGE buf;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		printf("WSA startup failed\n");

	sock = OpenLobby(&srvAddr, SERVERPORT); // �κ� UDP ���� ����

	HANDLE workerThread = NULL;
	UINT dwThreadIDwo = NULL;
	workerThread = (HANDLE)_beginthreadex(NULL, 0, DoWork, (void*)&sock, 0, dwThreadIDwo); // ��Ŀ ������ ����

	// �߰�
	//HANDLE chattingThread = NULL;
	//UINT dwThreadIDch = NULL;
	//chattingThread = (HANDLE)_beginthreadex(NULL, 0, Chatting, NULL , 0, dwThreadIDch);
	
	memset(buf.msg, 0, sizeof(buf.msg));
	// ������ ���� �����Ͱ� Ÿ�̹������� ���ļ� ���� ���ɼ��� �ִ��� �����ϱ� ���� �ʴ� �ݺ�Ƚ���� �ִ������� ����÷��� ��
	// -> �����ϰ� �ٸ� ���� ���� �ʵ��� �Ͽ���
	while (1) {
		if (recvfrom(sock, (char*)&buf, sizeof(buf), 0, (SOCKADDR*)&cltAddr, &cltAddrSize) > 0) { // ������ �����Ͱ� ������
			AddWork(&workfront, &workrear, buf.roomnum, buf.msg, cltAddr);						  // ť�� ����
		}
	}
	closesocket(sock);
	WSACleanup();

	return 0;
}


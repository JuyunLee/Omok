#pragma comment(lib, "ws2_32")
#include "include.h"
#define SERVERPORT	22122
#define DEBUG

// Queue�� front & rear
WORK* workfront = NULL;
WORK* workrear = NULL;

int main(int argc, char* argv[]) {
	SOCKET sock;
	SOCKADDR_IN srvAddr, cltAddr;
	int cltAddrSize = sizeof(cltAddr);
	MESSAGE buf;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		printf("WSA startup failed\n");

	sock = OpenLobby(&srvAddr, SERVERPORT); // �κ� UDP ���� ����

	// ��Ŀ ������ ����
	HANDLE workerThread = NULL;
	UINT dwThreadIDwo = NULL;
	workerThread = (HANDLE)_beginthreadex(NULL, 0, DoWork, (void*)&sock, 0, dwThreadIDwo);
	
	// ���� �����ڰ� ���� �����͸� �ޱ� ���� �ʴ� �ݺ�Ƚ���� �ִ������� ����÷��� ��
	// -> �����ϰ� �ٸ� ���� ���� �ʵ��� �Ͽ���
	while (1) {
		memset(buf.msg, 0, sizeof(buf.msg));
		// ������ �����Ͱ� ������ ť�� ����
		if (recvfrom(sock, (char*)&buf, sizeof(buf), 0, (SOCKADDR*)&cltAddr, &cltAddrSize) > 0) {
#ifdef DEBUG
			printf("received : %s\n", buf.msg);
#endif
			AddWork(&workfront, &workrear, buf.roomnum, buf.msg, cltAddr);						 
		}
	}
	WaitForSingleObjectEx(workerThread, INFINITE, TRUE);
	closesocket(sock);
	WSACleanup();

	return 0;
}



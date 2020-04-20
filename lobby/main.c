#pragma comment(lib, "ws2_32")
#include "include.h"
#define SERVERPORT	22122

// Queue의 front & rear
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

	sock = OpenLobby(&srvAddr, SERVERPORT); // 로비 UDP 소켓 구성

	HANDLE workerThread = NULL;
	UINT dwThreadIDwo = NULL;
	workerThread = (HANDLE)_beginthreadex(NULL, 0, DoWork, (void*)&sock, 0, dwThreadIDwo); // 워커 스레드 시작

	// 추가
	//HANDLE chattingThread = NULL;
	//UINT dwThreadIDch = NULL;
	//chattingThread = (HANDLE)_beginthreadex(NULL, 0, Chatting, NULL , 0, dwThreadIDch);
	
	memset(buf.msg, 0, sizeof(buf.msg));
	// 상대방이 보낸 데이터가 타이밍적으로 겹쳐서 씹힐 가능성을 최대한 배제하기 위해 초당 반복횟수를 최대한으로 끌어올려야 함
	// -> 자잘하게 다른 일을 하지 않도록 하였음
	while (1) {
		if (recvfrom(sock, (char*)&buf, sizeof(buf), 0, (SOCKADDR*)&cltAddr, &cltAddrSize) > 0) { // 수신한 데이터가 있으면
			AddWork(&workfront, &workrear, buf.roomnum, buf.msg, cltAddr);						  // 큐에 저장
		}
	}
	closesocket(sock);
	WSACleanup();

	return 0;
}


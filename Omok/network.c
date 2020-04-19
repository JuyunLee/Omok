#pragma comment(lib, "ws2_32")
#include "network.h"
#include "control.h"

///////////////////////////////////////////////////////////서버 사이드 API///////////////////////////////////////////////////////////

/*
서버를 오픈합니다
@param		servAddr		서버의 정보를 담을 구조체의 주소
@return		listenSock		연결 준비가 완료된 소켓
*/
SOCKET WINAPI LaunchServer(SOCKADDR_IN* servAddr)
{
	int retval;

	// 소켓 생성
	SOCKET listenSock = socket(PF_INET, SOCK_STREAM, 0);
	if (listenSock == INVALID_SOCKET) err_quit("socket()");

	// 등록(bind)
	ZeroMemory(servAddr, sizeof(*servAddr));
	servAddr->sin_family = PF_INET;
	servAddr->sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr->sin_port = htons(SERVERPORT);
	retval = bind(listenSock, (SOCKADDR*)servAddr, sizeof(*servAddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	return listenSock;
}
/*
클라이언트의 연결 요청을 받아 연결합니다
@param		listenSock		연결 준비가 완료된 소켓
@param		clntAddr		클라이언트의 정보를 담을 구조체의 주소
@return		client_sock		클라이언트와의 연결이 성사된 소켓
*/
SOCKET WINAPI AcceptLinkFromClient(SOCKET listenSock, SOCKADDR_IN* clntAddr)
{
	int nResult; // 함수를 호출한 결과값을 담기 위한 변수

	// 접속 요청 확인(listen)
	nResult = listen(listenSock, SOMAXCONN);
	if (nResult == SOCKET_ERROR) err_quit("listen()"); // 연결 중 에러가 발생하는 경우 에러를 출력하고 종료

	// 데이터 통신에 사용할 변수
	SOCKET clientSock;	// 클라이언트와의 연결이 성사된 소켓
	int nAddrLen;		// IP주소의 길이를 담을 변수
	clntAddr->sin_family = PF_INET;
	clntAddr->sin_addr.s_addr = htonl(INADDR_ANY);
	clntAddr->sin_port = htons(SERVERPORT);

	// 클라이언트 접속 요청 대기 및 허락
	nAddrLen = sizeof(*clntAddr);
	clientSock = accept(listenSock, (SOCKADDR*)clntAddr, &nAddrLen);
	if (clientSock != INVALID_SOCKET)
	{
		////디버그용
		// 접속한 클라이언트 정보 출력
		//printf("\n[TCP 서버] 클라이언트 접속 완료: IP 주소=%s, 포트 번호=%d\n", inet_ntoa(clntAddr->sin_addr), ntohs(clntAddr->sin_port));
		return clientSock;
	}
	else
	{
		Sleep(500);
		printf(".");
	}
}

/*
서버의 소켓을 닫습니다
@param		opponentSock	상대방과의 연결이 성사된 소켓의 주소
@param		listenSock		연결을 준비하는 동안 사용한 소켓의 주소
@param		clntAddr		클라이언트의 정보를 담은 구조체의 주소
@return		없음
*/
void CloseServerSocket(SOCKET* opponentSock, SOCKET* listenSock, SOCKADDR_IN* clntAddr)
{
	shutdown(*opponentSock, SD_SEND);
	shutdown(*opponentSock, SD_RECEIVE);
	////디버그용
	//gotoxy(0, 22);
	//printf("\n[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n", inet_ntoa(clntAddr->sin_addr), ntohs(clntAddr->sin_port));
	closesocket(*listenSock);
}

////////////////////////////////////////////////////////클라이언트 사이드 API////////////////////////////////////////////////////////

/*
서버 접속을 요청합니다
@param		szServerIP		서버 IP 문자열
@param		servAddr		서버의 정보를 담을 구조체의 주소
@return		server_sock		서버와의 연결이 성사된 소켓
*/
SOCKET WINAPI RequestLinkToServer(char* szServerIP, SOCKADDR_IN* servAddr)
{
	int retval;
	// 데이터 통신에 사용할 변수
	SOCKET serverSock = socket(PF_INET, SOCK_STREAM, 0);
	memset(servAddr, 0, sizeof(*servAddr));
	servAddr->sin_family = PF_INET;
	servAddr->sin_addr.s_addr = inet_addr(szServerIP);
	servAddr->sin_port = htons(SERVERPORT);

	while (1)
	{
		// 서버 접속(connect)
		if (connect(serverSock, (SOCKADDR*)servAddr, sizeof(*servAddr)) != SOCKET_ERROR)
		{
			////디버그용
			// 접속한 클라이언트 정보 출력
			//printf("\n[TCP] 서버 접속 완료: IP 주소=%s, 포트 번호=%d\n", inet_ntoa(servAddr->sin_addr), ntohs(servAddr->sin_port));
			return serverSock;
		}
		else
		{
			Sleep(500);
			printf(".");
		}
	}
}

/*
클라이언트의 소켓을 닫습니다
@param		opponentSock		상대방과의 연결이 성사된 구조체의 주소
@param		servAddr			서버의 정보를 담은 구조체의 주소(출력에만 쓰이기 때문에 추후 필요없으면 지워도 됨)
*/
void CloseClientSocket(SOCKET* opponentSock, SOCKADDR_IN* servAddr)
{
	shutdown(*opponentSock, SD_SEND);
	shutdown(*opponentSock, SD_RECEIVE);
	
	////디버그용
	//gotoxy(0, 22);
	//printf("\n[TCP] 서버 접속 종료: IP 주소=%s, 포트 번호=%d\n", inet_ntoa(servAddr->sin_addr), ntohs(servAddr->sin_port));
}

////////////////////////////////////////////////////////////////통용 API////////////////////////////////////////////////////////////////

/*
상대방의 키보드 입력을 수신합니다
@param		opponentSock	소켓
@param		buf				수신한 내용을 저장할 버퍼의 주소
@return		nResult			버퍼에 저장하고 난 후의 인덱스(인...듯?)
*/
int Recv(SOCKET opponentSock, char* buf)
{	
	int strLen = 0;
	while (1) {
		if ((strLen = recv(opponentSock, buf, sizeof(buf), 0)) > 0)
			return strLen;
	}
}

/*
연결 상대에게 데이터를 전송합니다
@prarm		opponentSock	상대방 소켓
@param		data			보낼 데이터
@return		없음
*/
void Send(SOCKET opponentSock, char* data)
{
	send(opponentSock, data, strlen(data), 0);
}

/*
소켓 함수 오류를 출력합니다
@param		msg		출력할 메시지
@return		없음
*/
void err_display(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
}

/*
소켓 함수 오류를 출력한 후 종료합니다
@param		msg		출력할 메시지
@return		없음

*/
void err_quit(char* msg)
{
	err_display(msg);
	exit(1);
}

//udp
int SendTo(SOCKET srvSock, char* buf, SOCKADDR_IN* addr)
{
	return sendto(srvSock, *buf, strlen(*buf), NULL, (SOCKADDR*)addr, sizeof(SOCKADDR_IN));
}

int RecvFrom(SOCKET srvSock, char* buf, SOCKADDR_IN* addr, int* addrSize)
{
	return recvfrom(srvSock, buf, sizeof(*buf), NULL, (SOCKADDR*)addr, addrSize);
}
#pragma comment(lib, "ws2_32")
#include "network.h"
#include "control.h"

///////////////////////////////////////////////////////////���� ���̵� API///////////////////////////////////////////////////////////

/*
������ �����մϴ�
@param		servAddr		������ ������ ���� ����ü�� �ּ�
@return		listenSock		���� �غ� �Ϸ�� ����
*/
SOCKET WINAPI LaunchServer(SOCKADDR_IN* servAddr)
{
	int retval;

	// ���� ����
	SOCKET listenSock = socket(PF_INET, SOCK_STREAM, 0);
	if (listenSock == INVALID_SOCKET) err_quit("socket()");

	// ���(bind)
	ZeroMemory(servAddr, sizeof(*servAddr));
	servAddr->sin_family = PF_INET;
	servAddr->sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr->sin_port = htons(SERVERPORT);
	retval = bind(listenSock, (SOCKADDR*)servAddr, sizeof(*servAddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	return listenSock;
}
/*
Ŭ���̾�Ʈ�� ���� ��û�� �޾� �����մϴ�
@param		listenSock		���� �غ� �Ϸ�� ����
@param		clntAddr		Ŭ���̾�Ʈ�� ������ ���� ����ü�� �ּ�
@return		client_sock		Ŭ���̾�Ʈ���� ������ ����� ����
*/
SOCKET WINAPI AcceptLinkFromClient(SOCKET listenSock, SOCKADDR_IN* clntAddr)
{
	int nResult; // �Լ��� ȣ���� ������� ��� ���� ����

	// ���� ��û Ȯ��(listen)
	nResult = listen(listenSock, SOMAXCONN);
	if (nResult == SOCKET_ERROR) err_quit("listen()"); // ���� �� ������ �߻��ϴ� ��� ������ ����ϰ� ����

	// ������ ��ſ� ����� ����
	SOCKET clientSock;	// Ŭ���̾�Ʈ���� ������ ����� ����
	int nAddrLen;		// IP�ּ��� ���̸� ���� ����
	clntAddr->sin_family = PF_INET;
	clntAddr->sin_addr.s_addr = htonl(INADDR_ANY);
	clntAddr->sin_port = htons(SERVERPORT);

	// Ŭ���̾�Ʈ ���� ��û ��� �� ���
	nAddrLen = sizeof(*clntAddr);
	clientSock = accept(listenSock, (SOCKADDR*)clntAddr, &nAddrLen);
	if (clientSock != INVALID_SOCKET)
	{
		////����׿�
		// ������ Ŭ���̾�Ʈ ���� ���
		//printf("\n[TCP ����] Ŭ���̾�Ʈ ���� �Ϸ�: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", inet_ntoa(clntAddr->sin_addr), ntohs(clntAddr->sin_port));
		return clientSock;
	}
	else
	{
		Sleep(500);
		printf(".");
	}
}

/*
������ ������ �ݽ��ϴ�
@param		opponentSock	������� ������ ����� ������ �ּ�
@param		listenSock		������ �غ��ϴ� ���� ����� ������ �ּ�
@param		clntAddr		Ŭ���̾�Ʈ�� ������ ���� ����ü�� �ּ�
@return		����
*/
void CloseServerSocket(SOCKET* opponentSock, SOCKET* listenSock, SOCKADDR_IN* clntAddr)
{
	shutdown(*opponentSock, SD_SEND);
	shutdown(*opponentSock, SD_RECEIVE);
	////����׿�
	//gotoxy(0, 22);
	//printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", inet_ntoa(clntAddr->sin_addr), ntohs(clntAddr->sin_port));
	closesocket(*listenSock);
}

////////////////////////////////////////////////////////Ŭ���̾�Ʈ ���̵� API////////////////////////////////////////////////////////

/*
���� ������ ��û�մϴ�
@param		szServerIP		���� IP ���ڿ�
@param		servAddr		������ ������ ���� ����ü�� �ּ�
@return		server_sock		�������� ������ ����� ����
*/
SOCKET WINAPI RequestLinkToServer(char* szServerIP, SOCKADDR_IN* servAddr)
{
	int retval;
	// ������ ��ſ� ����� ����
	SOCKET serverSock = socket(PF_INET, SOCK_STREAM, 0);
	memset(servAddr, 0, sizeof(*servAddr));
	servAddr->sin_family = PF_INET;
	servAddr->sin_addr.s_addr = inet_addr(szServerIP);
	servAddr->sin_port = htons(SERVERPORT);

	while (1)
	{
		// ���� ����(connect)
		if (connect(serverSock, (SOCKADDR*)servAddr, sizeof(*servAddr)) != SOCKET_ERROR)
		{
			////����׿�
			// ������ Ŭ���̾�Ʈ ���� ���
			//printf("\n[TCP] ���� ���� �Ϸ�: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", inet_ntoa(servAddr->sin_addr), ntohs(servAddr->sin_port));
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
Ŭ���̾�Ʈ�� ������ �ݽ��ϴ�
@param		opponentSock		������� ������ ����� ����ü�� �ּ�
@param		servAddr			������ ������ ���� ����ü�� �ּ�(��¿��� ���̱� ������ ���� �ʿ������ ������ ��)
*/
void CloseClientSocket(SOCKET* opponentSock, SOCKADDR_IN* servAddr)
{
	shutdown(*opponentSock, SD_SEND);
	shutdown(*opponentSock, SD_RECEIVE);
	
	////����׿�
	//gotoxy(0, 22);
	//printf("\n[TCP] ���� ���� ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", inet_ntoa(servAddr->sin_addr), ntohs(servAddr->sin_port));
}

////////////////////////////////////////////////////////////////��� API////////////////////////////////////////////////////////////////

/*
������ Ű���� �Է��� �����մϴ�
@param		opponentSock	����
@param		buf				������ ������ ������ ������ �ּ�
@return		nResult			���ۿ� �����ϰ� �� ���� �ε���(��...��?)
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
���� ��뿡�� �����͸� �����մϴ�
@prarm		opponentSock	���� ����
@param		data			���� ������
@return		����
*/
void Send(SOCKET opponentSock, char* data)
{
	send(opponentSock, data, strlen(data), 0);
}

/*
���� �Լ� ������ ����մϴ�
@param		msg		����� �޽���
@return		����
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
���� �Լ� ������ ����� �� �����մϴ�
@param		msg		����� �޽���
@return		����

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
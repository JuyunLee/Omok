#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#define SERVERPORT	22122
#define BUFSIZE		512

///////////////////////////////////////////////////////////���� ���̵� API///////////////////////////////////////////////////////////

/*
������ �����մϴ�
@param		servAddr		������ ������ ���� ����ü�� �ּ�
@return		listenSock		���� �غ� �Ϸ�� ����
*/
SOCKET WINAPI LaunchServer(SOCKADDR_IN* servAddr);

/*
Ŭ���̾�Ʈ�� ���� ��û�� �޾� �����մϴ�
@param		listenSock		���� �غ� �Ϸ�� ����
@param		clntAddr		Ŭ���̾�Ʈ�� ������ ���� ����ü�� �ּ�
@return		client_sock		Ŭ���̾�Ʈ���� ������ ����� ����
*/
SOCKET WINAPI AcceptLinkFromClient(SOCKET listenSock, SOCKADDR_IN* clntAddr);

/*
������ ������ �ݽ��ϴ�
@param		opponentSock	������� ������ ����� ������ �ּ�
@param		listenSock		������ �غ��ϴ� ���� ����� ������ �ּ�
@param		clntAddr		Ŭ���̾�Ʈ�� ������ ���� ����ü�� �ּ�
@return		����
*/
void CloseServerSocket(SOCKET* opponentSock, SOCKET* listenSock, SOCKADDR_IN* clntAddr);

////////////////////////////////////////////////////////Ŭ���̾�Ʈ ���̵� API////////////////////////////////////////////////////////

/*
���� ������ ��û�մϴ�
@param		szServerIP		���� IP ���ڿ�
@param		servAddr		������ ������ ���� ����ü�� �ּ�
@return		server_sock		�������� ������ ����� ����
*/
SOCKET WINAPI RequestLinkToServer(char* szServerIP, SOCKADDR_IN* servAddr);

/*
Ŭ���̾�Ʈ�� ������ �ݽ��ϴ�
@param		opponentSock		������� ������ ����� ����ü�� �ּ�
@param		servAddr			������ ������ ���� ����ü�� �ּ�(��¿��� ���̱� ������ ���� �ʿ������ ������ ��)
*/
void CloseClientSocket(SOCKET* opponentSock, SOCKADDR_IN* servAddr);

////////////////////////////////////////////////////////////////��� API////////////////////////////////////////////////////////////////

/*
������ Ű���� �Է��� �����մϴ�
@param		opponentSock	����
@param		buf				������ ������ ������ ������ �ּ�
@return		���ŵ� ���ڿ��� ����
*/
int Recv(SOCKET opponentSock, char* buf);

/*
���� ��뿡�� �����͸� �����մϴ�
@prarm		opponentSock	���� ����
@param		data			���� ������
@return		����
*/
void Send(SOCKET opponentSock, char* data);

/*
���� �Լ� ������ ����մϴ�
@param		msg		����� �޽���
@return		����
*/
void err_display(char* msg);

/*
���� �Լ� ������ ����� �� �����մϴ�
@param		msg		����� �޽���
@return		����

*/
void err_quit(char* msg);



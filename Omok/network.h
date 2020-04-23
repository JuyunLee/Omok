#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#define SERVERPORT	22122
#define BUFSIZE		512

///////////////////////////////////////////////////////////서버 사이드 API///////////////////////////////////////////////////////////

/*
서버를 오픈합니다
@param		servAddr		서버의 정보를 담을 구조체의 주소
@return		listenSock		연결 준비가 완료된 소켓
*/
SOCKET WINAPI LaunchServer(SOCKADDR_IN* servAddr);

/*
클라이언트의 연결 요청을 받아 연결합니다
@param		listenSock		연결 준비가 완료된 소켓
@param		clntAddr		클라이언트의 정보를 담을 구조체의 주소
@return		client_sock		클라이언트와의 연결이 성사된 소켓
*/
SOCKET WINAPI AcceptLinkFromClient(SOCKET listenSock, SOCKADDR_IN* clntAddr);

/*
서버의 소켓을 닫습니다
@param		opponentSock	상대방과의 연결이 성사된 소켓의 주소
@param		listenSock		연결을 준비하는 동안 사용한 소켓의 주소
@param		clntAddr		클라이언트의 정보를 담은 구조체의 주소
@return		없음
*/
void CloseServerSocket(SOCKET* opponentSock, SOCKET* listenSock, SOCKADDR_IN* clntAddr);

////////////////////////////////////////////////////////클라이언트 사이드 API////////////////////////////////////////////////////////

/*
서버 접속을 요청합니다
@param		szServerIP		서버 IP 문자열
@param		servAddr		서버의 정보를 담을 구조체의 주소
@return		server_sock		서버와의 연결이 성사된 소켓
*/
SOCKET WINAPI RequestLinkToServer(char* szServerIP, SOCKADDR_IN* servAddr);

/*
클라이언트의 소켓을 닫습니다
@param		opponentSock		상대방과의 연결이 성사된 구조체의 주소
@param		servAddr			서버의 정보를 담은 구조체의 주소(출력에만 쓰이기 때문에 추후 필요없으면 지워도 됨)
*/
void CloseClientSocket(SOCKET* opponentSock, SOCKADDR_IN* servAddr);

////////////////////////////////////////////////////////////////통용 API////////////////////////////////////////////////////////////////

/*
상대방의 키보드 입력을 수신합니다
@param		opponentSock	소켓
@param		buf				수신한 내용을 저장할 버퍼의 주소
@return		수신된 문자열의 길이
*/
int Recv(SOCKET opponentSock, char* buf);

/*
연결 상대에게 데이터를 전송합니다
@prarm		opponentSock	상대방 소켓
@param		data			보낼 데이터
@return		없음
*/
void Send(SOCKET opponentSock, char* data);

/*
소켓 함수 오류를 출력합니다
@param		msg		출력할 메시지
@return		없음
*/
void err_display(char* msg);

/*
소켓 함수 오류를 출력한 후 종료합니다
@param		msg		출력할 메시지
@return		없음

*/
void err_quit(char* msg);



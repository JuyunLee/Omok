#pragma once
#include "network.h"
#include "control.h"
#include "structs.h"
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>

#define		Y_CENTER			12	// 중앙 y좌표

#define		ROOM_X				7	// 방 선택 모드(좌측)
#define		ROOM_Y				12

#define		BTN_X				50	// 버튼 모드(우측)
#define		BTN_Y_CREATE		8	// 방 만들기
#define		BTN_Y_RAND			12	// 랜덤 접속
#define		BTN_Y_EXIT			16	// 나가기

#define		BOX_BTN_X			48
#define		BOX_BTN_Y_CREARE	7
#define		BOX_BTN_Y_EXIT		13

#define		CHAT_X				-1
#define		CHAT_Y				20

extern char nickname[24]; // 채팅에서 이용할 내 닉네임

/*
로비에 접근할 준비를 합니다(UDP; 소켓 생성 및 주소정보 저장)
@param		szServerIP		로비 서버의 IP 주소
@param		srvAddr			로비 서버의 주소 정보를 담을 SOCKADDR_IN형 변수의 주소
@output		srvSock			로비 서버에 연결할 수 있는 소켓
*/
SOCKET Setup(char* szServerIP, SOCKADDR_IN* srvAddr);

/*
받은 방 리스트를 출력합니다
@param		buf		방 리스트가 담긴 문자열
*/
void DrawRoomList(char* buf);

/*
로비를 출력합니다
*/
void DrawLobby();

/*
로비에서 커서를 이동합니다
@param		x		현재 x좌표가 저장된 변수의 주소
@param		y		현재 y좌표가 저장된 변수의 주소
@param		dst_x	목적지의 x좌표
@param		dst_y	목적지의 y좌표
*/
void MoveCursorInLobby(int* x, int* y, int dst_x, int dst_y);

/*
사용자로부터 방 이름 입력을 받아옵니다
@param		buf			방 이름이 저장될 버퍼
@param		sizeofbuf	버퍼의 크기(그냥 sizeof(buf))
*/
void GetRoomNameFromUser(char* buf, int sizeofbuf);

/*
로비를 실행합니다
@param		szServerIP		로비 서버의 IP
@param		szServerIpBuf	다른 사람 방에 입장하는 경우 상대방 ip를 저장할 버퍼
@param		chatalso		채팅도 같이 켤 지(1 : 켬 / 0 : 안켬)
@output		동작 결과 번호	0 : 게임 시작 / -1 : 돌아감
*/
int Lobby(char* szServerIP, char* szServerIpBuf, int chatalso);
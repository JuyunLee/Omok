#pragma once
#include "network.h"
#include "control.h"
#include "structs.h"
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>

#define		Y_CENTER			12	// �߾� y��ǥ

#define		ROOM_X				7	// �� ���� ���(����)
#define		ROOM_Y				12

#define		BTN_X				50	// ��ư ���(����)
#define		BTN_Y_CREATE		8	// �� �����
#define		BTN_Y_RAND			12	// ���� ����
#define		BTN_Y_EXIT			16	// ������

#define		BOX_BTN_X			48
#define		BOX_BTN_Y_CREARE	7
#define		BOX_BTN_Y_EXIT		13

#define		CHAT_X				-1
#define		CHAT_Y				20

extern char nickname[24];

SOCKET Setup(char* szServerIP, SOCKADDR_IN* srvAddr);

void DrawRoomList(char* buf);

void DrawLobby();

void MoveCursorInLobby(int* x, int* y, int dst_x, int dst_y);

void GetRoomNameFromUser(char* buf, int sizeofbuf);

int Lobby(char* szServerIP, char* szServerIpBuf, int chatalso);
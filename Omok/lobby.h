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

extern char nickname[24]; // ä�ÿ��� �̿��� �� �г���

/*
�κ� ������ �غ� �մϴ�(UDP; ���� ���� �� �ּ����� ����)
@param		szServerIP		�κ� ������ IP �ּ�
@param		srvAddr			�κ� ������ �ּ� ������ ���� SOCKADDR_IN�� ������ �ּ�
@output		srvSock			�κ� ������ ������ �� �ִ� ����
*/
SOCKET Setup(char* szServerIP, SOCKADDR_IN* srvAddr);

/*
���� �� ����Ʈ�� ����մϴ�
@param		buf		�� ����Ʈ�� ��� ���ڿ�
*/
void DrawRoomList(char* buf);

/*
�κ� ����մϴ�
*/
void DrawLobby();

/*
�κ񿡼� Ŀ���� �̵��մϴ�
@param		x		���� x��ǥ�� ����� ������ �ּ�
@param		y		���� y��ǥ�� ����� ������ �ּ�
@param		dst_x	�������� x��ǥ
@param		dst_y	�������� y��ǥ
*/
void MoveCursorInLobby(int* x, int* y, int dst_x, int dst_y);

/*
����ڷκ��� �� �̸� �Է��� �޾ƿɴϴ�
@param		buf			�� �̸��� ����� ����
@param		sizeofbuf	������ ũ��(�׳� sizeof(buf))
*/
void GetRoomNameFromUser(char* buf, int sizeofbuf);

/*
�κ� �����մϴ�
@param		szServerIP		�κ� ������ IP
@param		szServerIpBuf	�ٸ� ��� �濡 �����ϴ� ��� ���� ip�� ������ ����
@param		chatalso		ä�õ� ���� �� ��(1 : �� / 0 : ����)
@output		���� ��� ��ȣ	0 : ���� ���� / -1 : ���ư�
*/
int Lobby(char* szServerIP, char* szServerIpBuf, int chatalso);
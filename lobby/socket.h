#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

/*
�κ� ������ ���ϴ�
@param		srvAddr		���� �ּ� ������ ���� ������ �ּ�
@param		port		������ ��Ʈ
@output		������ ���� �� ���� ����
*/
SOCKET OpenLobby(SOCKADDR_IN* srvAddr, int port);
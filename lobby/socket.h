#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

/*
로비 서버를 엽니다
@param		srvAddr		서버 주소 정보를 담을 변수의 주소
@param		port		서버의 포트
@output		서버를 열고 난 후의 소켓
*/
SOCKET OpenLobby(SOCKADDR_IN* srvAddr, int port);
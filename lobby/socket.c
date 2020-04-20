#include "socket.h"
/*
로비 서버를 엽니다
@param	srvAddr		서버 주소 정보를 담을 변수의 주소
@param	port		서버의 포트
@output		서버를 열고 난 후의 소켓
*/
SOCKET OpenLobby(SOCKADDR_IN* srvAddr, int port)
{
	SOCKET srvSock = socket(AF_INET, SOCK_DGRAM, 0);
	memset(srvAddr, 0, sizeof(*srvAddr));
	(*srvAddr).sin_family = AF_INET;
	(*srvAddr).sin_port = htons(port);
	(*srvAddr).sin_addr.s_addr = htonl(INADDR_ANY);

	bind(srvSock, (SOCKADDR*)srvAddr, sizeof(*srvAddr));
	
	return srvSock;
}
#include "socket.h"

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
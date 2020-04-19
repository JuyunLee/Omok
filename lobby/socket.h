#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

SOCKET OpenLobby(SOCKADDR_IN* srvAddr, int port);
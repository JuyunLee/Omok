#pragma comment(lib, "ws2_32")
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <windows.h>	// 커맨드창에서 커서의 위치로 이동하기 위해 필요
#include <process.h>

void gotoxy(int x, int y) //특정 위치에 출력을 하고 싶을 때 사용하는 함수
{
	COORD Pos;

	Pos.X = x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos); //를 통해 gotoxy의 위치 지정
}

int main(int argc, char* argv[])
{
	HWND hCurr = GetConsoleWindow();
	SetWindowPos(hCurr, HWND_NOTOPMOST, 200, 250, 0, 0, SWP_NOSIZE);	// 콘솔창 위치 지정

	DWORD prevMode = 0;
	HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(handle, &prevMode);
	SetConsoleMode(handle, prevMode & ~ENABLE_QUICK_EDIT_MODE);			// 빠른편집모드 해제 -> 창을 선택상태로 바꿔서 프로그램이 멈추는 문제 해결

	system("mode con cols=55 lines=25");
	system("title 채팅 | color F0");
	COORD bufferSize = { 55, 25 }; // 가로, 세로
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize);

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa); // 윈속 초기화

	SOCKET sock;
	SOCKADDR_IN addr;
	socklen_t addrSize = sizeof(addr);
	BOOL opt = TRUE;
	//char buf[21][1024] = { 0 };
	char recvbuf[1024] = { 0 };
	int strLen;
	

	sock = socket(AF_INET, SOCK_DGRAM, 0);

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(22121);
	addr.sin_addr.s_addr = inet_addr(argv[1]);

	bind(sock, (SOCKADDR*)&addr, sizeof(addr));

	while (1)
	{
		if ((strLen = recvfrom(sock, recvbuf, sizeof(recvbuf) - 1, NULL, (SOCKADDR*)&addr, &addrSize)) > 0)
		{
			recvbuf[strLen] = '\0';
			printf("%s", recvbuf);
			memset(recvbuf, 0, sizeof(recvbuf));
		}
	}
	WSACleanup();
	return 0;
}

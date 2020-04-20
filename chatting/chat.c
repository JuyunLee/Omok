#pragma comment(lib, "ws2_32")
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <windows.h>	// Ŀ�ǵ�â���� Ŀ���� ��ġ�� �̵��ϱ� ���� �ʿ�
#include <process.h>

void gotoxy(int x, int y) //Ư�� ��ġ�� ����� �ϰ� ���� �� ����ϴ� �Լ�
{
	COORD Pos;

	Pos.X = x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos); //�� ���� gotoxy�� ��ġ ����
}

int main(int argc, char* argv[])
{
	HWND hCurr = GetConsoleWindow();
	SetWindowPos(hCurr, HWND_NOTOPMOST, 200, 250, 0, 0, SWP_NOSIZE);	// �ܼ�â ��ġ ����

	DWORD prevMode = 0;
	HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(handle, &prevMode);
	SetConsoleMode(handle, prevMode & ~ENABLE_QUICK_EDIT_MODE);			// ����������� ���� -> â�� ���û��·� �ٲ㼭 ���α׷��� ���ߴ� ���� �ذ�

	system("mode con cols=55 lines=25");
	system("title ä�� | color F0");
	COORD bufferSize = { 55, 25 }; // ����, ����
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize);

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa); // ���� �ʱ�ȭ

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

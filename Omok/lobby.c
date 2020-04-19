#include "lobby.h"
//#define HOME

char nickname[24];

SOCKET Setup(char* szServerIP, SOCKADDR_IN* srvAddr) //여기321
{
	SOCKET srvSock;
	srvSock = socket(AF_INET, SOCK_DGRAM, 0);
	memset(srvAddr, 0, sizeof(*srvAddr));
	(*srvAddr).sin_family = AF_INET;
	(*srvAddr).sin_addr.s_addr = inet_addr(szServerIP);
	(*srvAddr).sin_port = htons(SERVERPORT);

	return srvSock;
}

void DrawRoomList(char* buf)
{
	char roomname[21];
	strcpy(roomname, strtok(buf, "^"));
	gotoxy(ROOM_X, ROOM_Y - 3);
	printf("                    ");
	if (strcmp(roomname, "!@#$NULL$#@!"))
	{
		gotoxy(ROOM_X, ROOM_Y - 3);
		for (int i = 0; i < (20 - strlen(roomname)) / 2; i++) // 중앙정렬
			printf(" ");
		printf("%s", roomname);
	}

	strcpy(roomname, strtok(NULL, "^"));
	gotoxy(ROOM_X, ROOM_Y);
	printf("                    ");
	if (strcmp(roomname, "!@#$NULL$#@!"))
	{
		gotoxy(ROOM_X, ROOM_Y);
		for (int i = 0; i < (20 - strlen(roomname)) / 2; i++)
			printf(" ");
		printf("%s", roomname);
	}

	strcpy(roomname, strtok(NULL, "\0"));
	gotoxy(ROOM_X, ROOM_Y + 3);
	printf("                    ");
	if (strcmp(roomname, "!@#$NULL$#@!"))
	{
		gotoxy(ROOM_X, ROOM_Y + 3);
		for (int i = 0; i < (20 - strlen(roomname)) / 2; i++)
			printf(" ");
		printf("%s", roomname);
	}
}

void DrawLobby() {
	gotoxy(32, 3);
	printf("로 비\n");
	gotoxy(ROOM_X, ROOM_Y - 5);
	printf("         △");
	gotoxy(ROOM_X - 3, ROOM_Y);
	printf("▶                      ◀");
	gotoxy(ROOM_X - 2, ROOM_Y - 1);
	printf("━━━━━━━━━━━━━━━━━━━━━━━");
	gotoxy(ROOM_X - 2, ROOM_Y + 1);
	printf("━━━━━━━━━━━━━━━━━━━━━━━");
	gotoxy(ROOM_X, ROOM_Y + 5);
	printf("         ▽");
	gotoxy(BTN_X, BTN_Y_CREATE);
	printf("방만들기");
	gotoxy(BTN_X, BTN_Y_RAND);
	printf("랜덤매칭");
	gotoxy(BTN_X, BTN_Y_EXIT);
	printf("돌아가기");
	gotoxy(CHAT_X, CHAT_Y);
	printf("─────────────────────────────── 채팅─────────────────────────────────\n");
	printf("                                                                    \n");
	printf("                                                                    \n");
	printf("                                                                    ");
}

void MoveCursorInLobby(int* x, int* y, int dst_x, int dst_y) {
	if (*x == BTN_X)
	{
		gotoxy(BOX_BTN_X, (*y) - 1);
		printf("                     ");
		gotoxy(BOX_BTN_X, *y);
		printf(" ");
		gotoxy(BOX_BTN_X + 10, *y);
		printf(" ");
		gotoxy(BOX_BTN_X, (*y) + 1);
		printf("                     ");
	}
	else if (*x == CHAT_X)
	{
		gotoxy(CHAT_X, CHAT_Y);
		printf("─────────────────────────────── 채팅─────────────────────────────────\n");
		printf("                                                                    \n");
		printf("                                                                    \n");
		printf("                                                                    ");
	}
	*x = dst_x;
	*y = dst_y;
	if (dst_x == BTN_X)
	{
		gotoxy(BOX_BTN_X, dst_y - 1);
		printf("┌─────────┐");
		gotoxy(BOX_BTN_X, dst_y);
		printf("│");
		gotoxy(BOX_BTN_X + 10, dst_y);
		printf("│");
		gotoxy(BOX_BTN_X, dst_y + 1);
		printf("└─────────┘");
	}
	if (dst_x == CHAT_X)
	{
		gotoxy(CHAT_X, CHAT_Y);
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 채팅━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf("                                                                    \n");
		printf("                                                                    \n");
		printf("                                                                    ");
	}
	gotoxy(dst_x, dst_y);
}

void GetRoomNameFromUser(char* buf, int sizeofbuf) // 방이름 입력 받기
{
	system("cls");
	system("mode con cols=35 lines=25 | title 몰래오목");
	gotoxy(1, 4);
	printf("방 이름을 입력하세요:\n  ");
	memset(buf, 0, sizeofbuf);
	fgets(buf, sizeofbuf, stdin);
	strtok(buf, "\n");
}

int Lobby(char* szServerIP, char* szServerIpBuf, int chatalso)
{
	system("mode con cols=70 lines=25 | title 몰래오목"); // 창 크기 변경
	
	char msg[1024] = { 0 };
	char gotmsg[1000] = { 0 };
	SOCKADDR_IN srvAddr;
	socklen_t srvAddrSize = sizeof(srvAddr);
	MESSAGE buf;
	char longbuf[1024] = { 0 };
	int key;
	int x = ROOM_X, y = ROOM_Y;
	char command[50] = { 0 };
	
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa); 

	SOCKET srvSock = Setup(szServerIP, &srvAddr);
	SOCKET chatSock = socket(AF_INET, SOCK_DGRAM, 0);
	SOCKADDR_IN chatAddr;
	BOOL opt = TRUE;
	setsockopt(chatSock, SOL_SOCKET, SO_BROADCAST, (char*)& opt, sizeof(opt)); // broadcast
	memset(&chatAddr, 0, sizeof(chatAddr));
	chatAddr.sin_family = AF_INET;
	chatAddr.sin_port = htons(22121);
#ifdef HOME
	chatAddr.sin_addr.s_addr = inet_addr("192.168.219.255");
#else
	chatAddr.sin_addr.s_addr = inet_addr("192.168.0.255");
#endif


	char nicknamebuf[100] = { 0 };
	// 닉네임 설정
	HWND hCurr = GetConsoleWindow();
	do {
		memset(nicknamebuf, 0, sizeof(nicknamebuf));
		system("cls");
		gotoxy(1, 2);
		printf("닉네임을 입력하세요(최대10자): \n ");
		fgets(nicknamebuf, sizeof(nicknamebuf), stdin);
		strtok(nicknamebuf, "\n");
	} while (strlen(nicknamebuf) >= 21);
	memset(nickname, 0, sizeof(nickname));
	strcpy(nickname, "[");
	strcat(nickname, nicknamebuf);
	strcat(nickname, "] ");
	system("cls");

	strcpy(buf.msg, "r~");
	buf.roomnum = 0;
	sendto(srvSock, (char*)&buf, sizeof(buf), NULL, (SOCKADDR*)&srvAddr, sizeof(srvAddr));
	memset(buf.msg, 0, sizeof(buf.msg));
	recvfrom(srvSock, (char*)&buf, sizeof(buf), NULL, (SOCKADDR*)& srvAddr, &srvAddrSize);
	char* rooms = strtok(buf.msg, "~");
	strcpy(command, "start chatting.exe ");
	strcat(command, strtok(NULL, "\0"));
	if (chatalso > 0) {
		system(command);
		Sleep(500);
		keybd_event(VK_MENU, 0, 0, 0);
		keybd_event(VK_TAB, 0, 0, 0);
		keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
		BringWindowToTop(hCurr);
		SetWindowPos(hCurr, HWND_TOP, 640, 250, 0, 0, SWP_NOSIZE);
	}

	DrawLobby();
	DrawRoomList(rooms); // 방 목록 출력
	while (1)
	{
		key = _getch();
		if (key == 224) {
			switch (_getch()) {
			case 72: // 위
				if (x == ROOM_X) {
					strcpy(buf.msg, "mv~up");
					sendto(srvSock, (char*)&buf, sizeof(buf), NULL, (SOCKADDR*)& srvAddr, sizeof(srvAddr));
					memset(buf.msg, 0, sizeof(buf.msg));
					recvfrom(srvSock, (char*)&buf, sizeof(buf), NULL, (SOCKADDR*)& srvAddr, &srvAddrSize);
					DrawRoomList(buf.msg);
				} else if (y != BTN_Y_CREATE) {
					if (y == BTN_Y_EXIT && buf.roomnum == 0)
						MoveCursorInLobby(&x, &y, BTN_X, BTN_Y_CREATE);
					else
						MoveCursorInLobby(&x, &y, BTN_X, y - 4);
				}
				break;

			case 80: // 아래
				if (x == ROOM_X) {
					strcpy(buf.msg, "mv~down");
					sendto(srvSock, (char*)& buf, sizeof(buf), NULL, (SOCKADDR*)& srvAddr, sizeof(srvAddr));
					memset(buf.msg, 0, sizeof(buf.msg));
					recvfrom(srvSock, (char*)& buf, sizeof(buf), NULL, (SOCKADDR*)& srvAddr, &srvAddrSize);
					DrawRoomList(buf.msg);
				}
				else if (y != CHAT_Y) {
						if (y == BTN_Y_CREATE && buf.roomnum == 0)
							MoveCursorInLobby(&x, &y, BTN_X, BTN_Y_EXIT);
						else if (y == BTN_Y_EXIT)
						{
							MoveCursorInLobby(&x, &y, CHAT_X, CHAT_Y);
							while (1) {
								gotoxy(CHAT_X, CHAT_Y);
								printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 채팅━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
								printf("                                                                    \n");
								printf("                                                                    \n");
								printf("                                                                    ");
								gotoxy(CHAT_X, CHAT_Y + 1);
								strcpy(msg, nickname);
								fgets(gotmsg, sizeof(gotmsg), stdin);
								if (!strcmp(gotmsg, "\n"))
								{
									memset(msg, 0, sizeof(msg));
									memset(gotmsg, 0, sizeof(gotmsg));
									system("cls");
									DrawLobby();
									strcpy(buf.msg, "gr~");
									sendto(srvSock, (char*)&buf, sizeof(buf), NULL, (SOCKADDR*)&srvAddr, sizeof(srvAddr));
									memset(buf.msg, 0, sizeof(buf.msg));
									recvfrom(srvSock, (char*)&buf, sizeof(buf), NULL, (SOCKADDR*)&srvAddr, &srvAddrSize);
									DrawRoomList(buf.msg); // 방 목록 출력
									gotoxy(CHAT_X, CHAT_Y);
									printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 채팅━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
									printf("                                                                    \n");
									printf("                                                                    \n");
									printf("                                                                    ");
									MoveCursorInLobby(&x, &y, BTN_X, BTN_Y_EXIT);
									break;
								}
								else {
									strcat(msg, gotmsg);
									sendto(chatSock, msg, strlen(msg), NULL, (SOCKADDR*)& chatAddr, sizeof(chatAddr));
									memset(msg, 0, sizeof(msg));
									memset(gotmsg, 0, sizeof(gotmsg));
									system("cls");
									DrawLobby();
									strcpy(buf.msg, "gr~");
									sendto(srvSock, (char*)&buf, sizeof(buf), NULL, (SOCKADDR*)&srvAddr, sizeof(srvAddr));
									memset(buf.msg, 0, sizeof(buf.msg));
									recvfrom(srvSock, (char*)&buf, sizeof(buf), NULL, (SOCKADDR*)&srvAddr, &srvAddrSize);
									DrawRoomList(buf.msg); // 방 목록 출력
									gotoxy(CHAT_X, CHAT_Y);
									printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 채팅━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
									printf("                                                                    \n");
									printf("                                                                    \n");
									printf("                                                                    ");
								}
							}
						}
						else
							MoveCursorInLobby(&x, &y, BTN_X, y + 4);
				}
				break;

			case 75: // 왼쪽
				MoveCursorInLobby(&x, &y, ROOM_X, ROOM_Y);
				break;

			case 77: // 오른쪽
				if(x != BTN_X)
					MoveCursorInLobby(&x, &y, BTN_X, BTN_Y_CREATE);
				break;
			}
		} else if (key == 32 || key == '\r') { // 선택
			SOCKET sock = -1, listenSock = -1, cltSock = -1;
			SOCKADDR_IN opponentAddr;
			char roomname[100] = { 0 };
			switch (y) {
			case Y_CENTER: 
				if (x == ROOM_X) // 방 들어가기
				{
					strcpy(buf.msg, "e~");
					sendto(srvSock, (char*)& buf, sizeof(buf), NULL, (SOCKADDR*)& srvAddr, sizeof(srvAddr));
					memset(buf.msg, 0, sizeof(buf.msg));
					recvfrom(srvSock, (char*)& buf, sizeof(buf), NULL, (SOCKADDR*)& srvAddr, &srvAddrSize);
					if (!strcmp(buf.msg, "ALREADY")) {
						gotoxy(25, 18);
						printf("아... 방 없음");
						memset(buf.msg, 0, sizeof(buf.msg));
						memset(roomname, 0, sizeof(roomname));
						_getch();
						gotoxy(24, 18);
						printf("              ");
						continue; // 여기32123
					}
					// 클라이언트쪽 출력문
					system("mode con cols=35 lines=25 | title 몰래오목");
					system("cls");
					gotoxy(0, 3);
					printf("입장중...");
					strcpy(szServerIpBuf, buf.msg);
				}
				else // 랜덤매칭
				{
					strcpy(buf.msg, "rd~");
					sendto(srvSock, (char*)&buf, sizeof(buf), NULL, (SOCKADDR*)&srvAddr, sizeof(srvAddr));
					memset(buf.msg, 0, sizeof(buf.msg));
					recvfrom(srvSock, (char*)&buf, sizeof(buf), NULL, (SOCKADDR*)&srvAddr, &srvAddrSize);
					if (!strcmp(buf.msg, "ALREADY")) {
						gotoxy(25, 22);
						printf("아... 방 없음");
						memset(buf.msg, 0, sizeof(buf.msg));
						memset(roomname, 0, sizeof(roomname));
						_getch();
						gotoxy(24, 22);
						printf("              ");
						continue; // 여기32123
					}
					// 클라이언트쪽 출력문
					system("mode con cols=35 lines=25 | title 몰래오목");
					system("cls");
					gotoxy(0, 3);
					printf("입장중...");
					strcpy(szServerIpBuf, buf.msg);
				}
				break;

			case BTN_Y_CREATE: // 방 만들기
				system("mode con cols=35 lines=25 | title 몰래오목");
				while (1)
				{
					memset(longbuf, 0, sizeof(longbuf));
					GetRoomNameFromUser(longbuf, sizeof(longbuf));
					if (strlen(longbuf) > 20) {
						gotoxy(1, 15);
						printf("방 이름이 너무 깁니다");
						_getch();
					} else {
						strcpy(roomname, longbuf);
						break;
					}
				}
				memset(buf.msg, 0, sizeof(buf.msg));
				strcpy(buf.msg, "m~");
				strcat(buf.msg, roomname);
				sendto(srvSock, (char*)& buf, sizeof(buf), NULL, (SOCKADDR*)& srvAddr, sizeof(srvAddr));
				//서버쪽 출력문
				system("mode con cols=35 lines=25 | title 몰래오목");
				system("cls");
				gotoxy(0,3);
				printf("매칭중...");
				break;
			case BTN_Y_EXIT: // 나가기
				closesocket(sock);
				return -1;
				break;
			}
			closesocket(sock);
			WSACleanup();
			return 0;
		}
	}
}
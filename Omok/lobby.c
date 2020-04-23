#include "lobby.h"
//#define HOME

char nickname[24]; // ä�ÿ��� �̿��� �� �г���

/*
�κ� ������ �غ� �մϴ�(UDP; ���� ���� �� �ּ����� ����)
@param		szServerIP		�κ� ������ IP �ּ�
@param		srvAddr			�κ� ������ �ּ� ������ ���� SOCKADDR_IN�� ������ �ּ�
@output		srvSock			�κ� ������ ������ �� �ִ� ����
*/
SOCKET Setup(char* szServerIP, SOCKADDR_IN* srvAddr)
{
	SOCKET srvSock;
	srvSock = socket(AF_INET, SOCK_DGRAM, 0);
	memset(srvAddr, 0, sizeof(*srvAddr));
	(*srvAddr).sin_family = AF_INET;
	(*srvAddr).sin_addr.s_addr = inet_addr(szServerIP);
	(*srvAddr).sin_port = htons(SERVERPORT);

	return srvSock;
}

/*
���� �� ����Ʈ�� ����մϴ�
@param		buf		�� ����Ʈ�� ��� ���ڿ�
*/
void DrawRoomList(char* buf)
{
	char roomname[21];
	strcpy(roomname, strtok(buf, "^"));
	gotoxy(ROOM_X, ROOM_Y - 3);
	printf("                    ");
	if (strcmp(roomname, "!@#$NULL$#@!"))
	{
		gotoxy(ROOM_X, ROOM_Y - 3);
		for (int i = 0; i < (20 - strlen(roomname)) / 2; i++) // �߾�����
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

/*
�κ� ����մϴ�
*/
void DrawLobby() {
	gotoxy(32, 3);
	printf("�� ��\n");
	gotoxy(ROOM_X, ROOM_Y - 5);
	printf("         ��");
	gotoxy(ROOM_X - 3, ROOM_Y);
	printf("��                      ��");
	gotoxy(ROOM_X - 2, ROOM_Y - 1);
	printf("����������������������������������������������");
	gotoxy(ROOM_X - 2, ROOM_Y + 1);
	printf("����������������������������������������������");
	gotoxy(ROOM_X, ROOM_Y + 5);
	printf("         ��");
	gotoxy(BTN_X, BTN_Y_CREATE);
	printf("�游���");
	gotoxy(BTN_X, BTN_Y_RAND);
	printf("������Ī");
	gotoxy(BTN_X, BTN_Y_EXIT);
	printf("���ư���");
	gotoxy(CHAT_X, CHAT_Y);
	printf("�������������������������������������������������������������� ä�æ�����������������������������������������������������������������\n");
	printf("                                                                    \n");
	printf("                                                                    \n");
	printf("                                                                    ");
}

/*
�κ񿡼� Ŀ���� �̵��մϴ�
@param		x		���� x��ǥ�� ����� ������ �ּ�
@param		y		���� y��ǥ�� ����� ������ �ּ�
@param		dst_x	�������� x��ǥ
@param		dst_y	�������� y��ǥ
*/
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
		printf("�������������������������������������������������������������� ä�æ�����������������������������������������������������������������\n");
		printf("                                                                    \n");
		printf("                                                                    \n");
		printf("                                                                    ");
	}
	*x = dst_x;
	*y = dst_y;
	if (dst_x == BTN_X)
	{
		gotoxy(BOX_BTN_X, dst_y - 1);
		printf("����������������������");
		gotoxy(BOX_BTN_X, dst_y);
		printf("��");
		gotoxy(BOX_BTN_X + 10, dst_y);
		printf("��");
		gotoxy(BOX_BTN_X, dst_y + 1);
		printf("����������������������");
	}
	if (dst_x == CHAT_X)
	{
		gotoxy(CHAT_X, CHAT_Y);
		printf("�������������������������������������������������������������� ä�æ�����������������������������������������������������������������\n");
		printf("                                                                    \n");
		printf("                                                                    \n");
		printf("                                                                    ");
	}
	gotoxy(dst_x, dst_y);
}

/*
����ڷκ��� �� �̸� �Է��� �޾ƿɴϴ�
@param		buf			�� �̸��� ����� ����
@param		sizeofbuf	������ ũ��(�׳� sizeof(buf))
*/
void GetRoomNameFromUser(char* buf, int sizeofbuf) // ���̸� �Է� �ޱ�
{
	system("cls");
	system("mode con cols=35 lines=25 | title ��������");
	gotoxy(1, 4);
	printf("�� �̸��� �Է��ϼ���:\n  ");
	memset(buf, 0, sizeofbuf);
	fgets(buf, sizeofbuf, stdin);
	strtok(buf, "\n");
}

/*
�κ� �����մϴ�
@param		szServerIP		�κ� ������ IP
@param		szServerIpBuf	�ٸ� ��� �濡 �����ϴ� ��� ���� ip�� ������ ����
@param		chatalso		ä�õ� ���� �� ��(1 : �� / 0 : ����)
@output		���� ��� ��ȣ	0 : ���� ���� / -1 : ���ư�
*/
int Lobby(char* szServerIP, char* szServerIpBuf, int chatalso)
{
	system("mode con cols=70 lines=25 | title ��������"); // â ũ�� ����
	
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
	// �г��� ����
	HWND hCurr = GetConsoleWindow();
	do {
		memset(nicknamebuf, 0, sizeof(nicknamebuf));
		system("cls");
		gotoxy(1, 2);
		printf("�г����� �Է��ϼ���(�ִ�10��): \n ");
		fgets(nicknamebuf, sizeof(nicknamebuf), stdin);
		strtok(nicknamebuf, "\n");
	} while (strlen(nicknamebuf) >= 21);
	memset(nickname, 0, sizeof(nickname));
	strcpy(nickname, "[");
	strcat(nickname, nicknamebuf);
	strcat(nickname, "] ");
	system("cls");

	strcpy(buf.msg, "i~");
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
	DrawRoomList(rooms); // �� ��� ���
	while (1)
	{
		key = _getch();
		if (key == 224) {
			switch (_getch()) {
			case 72: // ��
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

			case 80: // �Ʒ�
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
								printf("�������������������������������������������������������������� ä�æ�����������������������������������������������������������������\n");
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
									DrawRoomList(buf.msg); // �� ��� ���
									gotoxy(CHAT_X, CHAT_Y);
									printf("�������������������������������������������������������������� ä�æ�����������������������������������������������������������������\n");
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
									DrawRoomList(buf.msg); // �� ��� ���
									gotoxy(CHAT_X, CHAT_Y);
									printf("�������������������������������������������������������������� ä�æ�����������������������������������������������������������������\n");
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

			case 75: // ����
				MoveCursorInLobby(&x, &y, ROOM_X, ROOM_Y);
				break;

			case 77: // ������
				if(x != BTN_X)
					MoveCursorInLobby(&x, &y, BTN_X, BTN_Y_CREATE);
				break;
			}
		} else if (key == 32 || key == '\r') { // ����
			SOCKET sock = -1, listenSock = -1, cltSock = -1;
			SOCKADDR_IN opponentAddr;
			char roomname[100] = { 0 };
			switch (y) {
			case Y_CENTER: 
				if (x == ROOM_X) // �� ����
				{
					strcpy(buf.msg, "e~");
					sendto(srvSock, (char*)& buf, sizeof(buf), NULL, (SOCKADDR*)& srvAddr, sizeof(srvAddr));
					memset(buf.msg, 0, sizeof(buf.msg));
					recvfrom(srvSock, (char*)& buf, sizeof(buf), NULL, (SOCKADDR*)& srvAddr, &srvAddrSize);
					if (!strcmp(buf.msg, "ALREADY")) {
						gotoxy(25, 18);
						printf("��... �� ����");
						memset(buf.msg, 0, sizeof(buf.msg));
						memset(roomname, 0, sizeof(roomname));
						_getch();
						gotoxy(24, 18);
						printf("              ");
						continue; // ����32123
					}
					// Ŭ���̾�Ʈ�� ��¹�
					system("mode con cols=35 lines=25 | title ��������");
					system("cls");
					gotoxy(0, 3);
					printf("������...");
					strcpy(szServerIpBuf, buf.msg);
				}
				else // ������Ī
				{
					strcpy(buf.msg, "rd~");
					sendto(srvSock, (char*)&buf, sizeof(buf), NULL, (SOCKADDR*)&srvAddr, sizeof(srvAddr));
					memset(buf.msg, 0, sizeof(buf.msg));
					recvfrom(srvSock, (char*)&buf, sizeof(buf), NULL, (SOCKADDR*)&srvAddr, &srvAddrSize);
					if (!strcmp(buf.msg, "ALREADY")) {
						gotoxy(25, 22);
						printf("��... �� ����");
						memset(buf.msg, 0, sizeof(buf.msg));
						memset(roomname, 0, sizeof(roomname));
						_getch();
						gotoxy(24, 22);
						printf("              ");
						continue; // ����32123
					}
					// Ŭ���̾�Ʈ�� ��¹�
					system("mode con cols=35 lines=25 | title ��������");
					system("cls");
					gotoxy(0, 3);
					printf("������...");
					strcpy(szServerIpBuf, buf.msg);
				}
				break;

			case BTN_Y_CREATE: // �� �����
				system("mode con cols=35 lines=25 | title ��������");
				while (1)
				{
					memset(longbuf, 0, sizeof(longbuf));
					GetRoomNameFromUser(longbuf, sizeof(longbuf));
					if (strlen(longbuf) > 20) {
						gotoxy(1, 15);
						printf("�� �̸��� �ʹ� ��ϴ�");
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
				//������ ��¹�
				system("mode con cols=35 lines=25 | title ��������");
				system("cls");
				gotoxy(0,3);
				printf("��Ī��...");
				break;
			case BTN_Y_EXIT: // ������
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
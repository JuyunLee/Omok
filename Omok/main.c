#pragma comment(lib, "ws2_32") // ���� �Լ� �̿��� ���� �߰�
#include "include.h" // include�� �͵� ��Ƶ�
//#define HOME

int main(int argc, char* argv[])
{
	int nNextKey;									// �ڽ��� Ű �Է��� �޾� ������ ����(_getch())
	char cNextKey[2];								// Ű �Է��� ���ڿ��� �ٲ� ��Ʈ��ũ�� �̿��ϱ� ���� ����
	char cMenuMode = 1, cGameMode = 0;				// ���� ��� ������ ���� ����(������ ���� �ʿ� ����)
	char cMyTurn;									// �� ���ʰ� �������� (1 : �浹�� �� �� ���� / -1 : �鵹�� �� �� ����)
	char cWhosTurn = 1;								// ù �õ��� �׻� �浹
	int vnDolMatrix[15][15];						// �� ������ ��� 15 X 15 �迭 ����
	int vnPanMatrix[15][15];						// �� ����� ��� 15 X 15 �迭 ����

	CURSOR stCursor;
	stCursor.pvnCursorOnDolMat = &vnDolMatrix[7][7];// �� �迭�� ����� Ŀ�� �����Ϳ� �� �迭�� �ּҸ� ����(vnDolMatrix[0][0]�� �ּҰ� ���)
	stCursor.vnCursorCoords[0] = 2 * 7;
	stCursor.vnCursorCoords[1] = 7;			// �ʱ� ��ǥ ����
	stCursor.pHistory[0] = NULL;
	stCursor.pHistory[1] = NULL;
	stCursor.nHistory[0][0] = -1;
	stCursor.nHistory[0][1] = -1;
	stCursor.nHistory[1][0] = -1;
	stCursor.nHistory[1][1] = -1;
	int nReturnFromIntro = 0;
	SOCKET listenSock, opponentSock;
	SOCKADDR_IN servAddr, cltAddr;
	char szServerIp[20] = { 0 };
	char cNetworkMode = 0;
	int chance = 1;
	int gameresult = 0;
	int chatalso = 1;

	HWND hCurr = GetConsoleWindow();
	SetWindowPos(hCurr, HWND_TOP, 640, 250, 0, 0, SWP_NOSIZE);

	cursor(0); // Ŀ�� �����
	system("mode con cols=35 lines=25 | title ��������");
	system("color 60");
	COORD bufferSize = { 35, 1 }; // ����, ����
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize);
	//SayHi();
	while (1)
	{
		while (1)
		{
			// �ʱ� ȭ�� ����
			if (10 * cGameMode + cMenuMode == 1)			// ��� ����. 10������ Ư���� �̿��Ͽ� 10�� �ڸ��� ���������� ����, 1�� �ڸ��� �޴��� �������� ���θ� ��Ÿ��
			{
				// ���� �ʱ�ȭ ����
				system("cls");
				InitializeDolMatrix(vnDolMatrix, 15);			// �� �迭 �ʱ�ȭ
				InitializePanMatrix(vnPanMatrix, 15);			// �� �迭 �ʱ�ȭ
				cNetworkMode = -1;
				stCursor.vnCursorCoords[0] = 2 * 7;
				stCursor.vnCursorCoords[1] = 7;
				stCursor.pvnCursorOnDolMat = &vnDolMatrix[7][7];
				cWhosTurn = 1;
				chance = 1;
				// ���� �ʱ�ȭ ��

				// ���� �޴�
				if ((nReturnFromIntro = RunIntro()) == 1) // ���� ����
				{
					cGameMode = 1;
					cMenuMode = 0;
				}
				else if (nReturnFromIntro == 0)
				{
					system("TASKKILL /IM chatting.exe");
					return 0;
				}
			}
			else if (10 * cGameMode + cMenuMode == 10)
			{
				// ���� ȭ��
				while (cNetworkMode != '0' && cNetworkMode != '1') // �������� Ŭ���̾�Ʈ���� ����... 0 : server / 1 : client
				{
					system("cls");
					SOCKET resSocks[2] = { 0 };
					memset(szServerIp, 0, sizeof(szServerIp));
#ifdef HOME
					if (Lobby("192.168.219.104", szServerIp, chatalso--) == -1)
#else
					if (Lobby("192.168.0.28", szServerIp, chatalso--) == -1)
#endif
					{
						cMenuMode = 1;
						cGameMode = 0;
						cNetworkMode = 0;
						break;
					}

					// ���� �ʱ�ȭ
					WSADATA wsaData;
					if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
					{
						return 1;
					}

					if (strcmp(szServerIp, "")) // Ŭ���̾�Ʈ
					{
						opponentSock = RequestLinkToServer(szServerIp, &cltAddr);
						cNetworkMode = '1';
						cMyTurn = -1;
						system("color 80");
					}
					else // ����
					{
						// ������
						EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_GRAYED);
						DrawMenuBar(GetConsoleWindow());

						listenSock = LaunchServer(&servAddr);
						opponentSock = AcceptLinkFromClient(listenSock, &cltAddr);
						// ���� ��
						EnableMenuItem(GetSystemMenu(GetConsoleWindow(), TRUE), SC_CLOSE, MF_GRAYED);
						DrawMenuBar(GetConsoleWindow());

						cNetworkMode = '0';
						cMyTurn = 1;
					}
					if (cNetworkMode == 0)
					{
						cMenuMode = 1;
						cGameMode = 0;
						break;
					}
					system("cls");
					printf("\n");
					gotoxy(-1, 0);
					PrintPan(vnPanMatrix, vnDolMatrix, cWhosTurn);
					gotoxy(stCursor.vnCursorCoords[0], stCursor.vnCursorCoords[1]);
				}
				break;
			}
			else if (10 * cGameMode + cMenuMode == 11)
			{
				if (cWhosTurn == cMyTurn)
				{
					int nResult = RunMenu(chance, &stCursor);
					if (nResult == 2) { // ������
						if (stCursor.nHistory[1][0] != -1) {
							Murugi(&stCursor);
							chance = 0;
						} else {
							system("cls");
							gotoxy(1, 7);
							printf("����� �� �����ϴ�");
						}
						system("cls");
						gotoxy(-1, 0);
						PrintPan(vnPanMatrix, vnDolMatrix, cWhosTurn);
						gotoxy(stCursor.vnCursorCoords[0], stCursor.vnCursorCoords[1] - 1);
						cGameMode = 1;
						cMenuMode = 0;
						Send(opponentSock, "m"); // ������
					} else if (nResult == 1) { // ���� �簳
						gotoxy(-1, 0);
						PrintPan(vnPanMatrix, vnDolMatrix, cWhosTurn);
						gotoxy(stCursor.vnCursorCoords[0], stCursor.vnCursorCoords[1] - 1);
						cGameMode = 1;
						cMenuMode = 0;
						Send(opponentSock, "c"); // continue
					} else if (nResult == 0) { // ���� ����
						cGameMode = 0;
						cMenuMode = 1;
						Send(opponentSock, "a"); // abort
						system("cls");
						PrintPan(vnPanMatrix, vnDolMatrix, cWhosTurn);
						gotoxy(0, 15);
						printf("                                     ");
						// �ٽ� ���� ��� ����
						// �ٽ����� �����
						if (Regame() == 1) { // ����� ����� ����
							IgnoreKeyboardInput();
							Recv(opponentSock, cNextKey);
							IgnoreKeyboardInput();
							if (!strcmp(cNextKey, "1")) {
								gotoxy(0, 16);
								printf("                                     ");
								// �ٽ��ϱ�
								cNextKey[0] = '1';
								cNextKey[1] = '\0';
								Send(opponentSock, cNextKey);
								// ���� �����
								InitializeToReGame(vnDolMatrix, &cWhosTurn, &cMyTurn, 1, &chance, &stCursor);
								PrintPan(vnPanMatrix, vnDolMatrix, cWhosTurn);
								cMenuMode = 0;
								cGameMode = 1;
								break;
							}
							else {
								gotoxy(0, 18);
								printf("                                     ");
								gotoxy(0, 19);
								printf(" ������ ������ �����ϼ̽��ϴ�   ");
								gotoxy(0, 20);
									printf("                                      ");
								_getch();
							}
						}
						else {
							// �簳���� ����
							Recv(opponentSock, cNextKey);
							Send(opponentSock, "0");
						}
						// �ٽ� ���� ��� ��
						if (cNetworkMode == '0')
							CloseServerSocket(&opponentSock, &listenSock, &cltAddr);
						else
							CloseClientSocket(&opponentSock, &servAddr);
						// ���� ����
						WSACleanup();
						gotoxy(0, 22); // ������ ����� �� (����� �� �߰��� ���� �Ž����� ����)
					}
				}
				else if (cWhosTurn != cMyTurn)
				{
					IgnoreKeyboardInput();
					Recv(opponentSock, cNextKey);
					IgnoreKeyboardInput(); // �Է� �ޱ� ���ķ� �� ����
					if (cNextKey[0] == 'c') // ���� �簳
					{
						cGameMode = 1;
						cMenuMode = 0;
					}
					if (cNextKey[0] == 'm')
					{
						if (stCursor.nHistory[1][0] != -1) {
							Murugi(&stCursor);
							system("cls");
							gotoxy(-1, 0);
							PrintPan(vnPanMatrix, vnDolMatrix, cWhosTurn);
							gotoxy(0, 18);
							printf("������ �����⸦ ����߽��ϴ�");
							gotoxy(stCursor.vnCursorCoords[0], stCursor.vnCursorCoords[1] - 1);
						}

						cGameMode = 1;
						cMenuMode = 0;
					}
					if (cNextKey[0] == 'a') // ���� ����
					{
						cGameMode = 0;
						cMenuMode = 1;
						gotoxy(0, 16);
						system("color 60");
						printf("                                 ");
						gotoxy(0, 16);
						printf(" �¸��߽��ϴ�\n  ������ ����Ͽ����ϴ�        ");
						gotoxy(0, 18);
						printf("                                   ");
						// �ٽ� ���� ��� ����
						// �ٽ����� �����
						if (Regame() == 1) { // �ٽ��ϱ�
							cNextKey[0] = '1';
							cNextKey[1] = '\0';
							Send(opponentSock, cNextKey);
							gotoxy(0, 18);
							for (int i = 0; i < 3; i++)
								printf("                          ");
							IgnoreKeyboardInput();
							Recv(opponentSock, cNextKey);
							IgnoreKeyboardInput();
							if (!strcmp(cNextKey, "1")) {
								// ���� �����
								InitializeToReGame(vnDolMatrix, &cWhosTurn, &cMyTurn, -1, &chance, &stCursor);
								gotoxy(0, 19);
								printf(" ������ ������ �����ϼ̽��ϴ�    ");
								Sleep(2);
								gotoxy(0, 19);
								printf("                                     ");
								PrintPan(vnPanMatrix, vnDolMatrix, cWhosTurn);
								cMenuMode = 0;
								cGameMode = 1;
								break;
							}
							else {
								gotoxy(0, 15);
								printf("                                 ");
								gotoxy(0, 19);
								printf(" ������ ������ �����ϼ̽��ϴ�   ");
								_getch();
							}

						}
						else {
							// �簳���� ����
							Send(opponentSock, "0");
						}
						// �ٽ� ���� ��� ��
						if (cNetworkMode == '0')
							CloseServerSocket(&opponentSock, &listenSock, &cltAddr);
						else
							CloseClientSocket(&opponentSock, &servAddr);
						// ���� ����
						WSACleanup();
					}
				}
			}
		}
		if (cNetworkMode == 0) continue;
		while (1) // �������
		{
			if (cMyTurn == cWhosTurn) // �� ������ ��
			{
				gotoxy(0, 16);
				printf(" �� �����Դϴ�. ���� �����ּ���      ");
				gotoxy(0, 18);
				printf("                               ");
				gotoxy(stCursor.vnCursorCoords[0], stCursor.vnCursorCoords[1]);
				system("color 60");
				nNextKey = _getch();
				cNextKey[0] = (char)nNextKey;
				cNextKey[1] = 0; // ���ڿ� ���·� ��Ʈ��ŷ�� �̷������ ������ NULL�� �־���.
				if (nNextKey < 224)
					Send(opponentSock, cNextKey);
			}
			else if (cMyTurn == -cWhosTurn) // ���� ������ ��
			{
				gotoxy(0, 16);
				printf("������ ������ ��ٸ��� ���Դϴ�");
				gotoxy(stCursor.vnCursorCoords[0], stCursor.vnCursorCoords[1]);
				system("color 80");
				IgnoreKeyboardInput();
				Recv(opponentSock, cNextKey);
				IgnoreKeyboardInput(); // �Է� �ޱ� ���ķ� �� ����
			}
			else
			{
				gotoxy(0, 22);
				printf("���� �����ΰ�? ���Ⱑ ���?\n");
				return 0;
			}
			if (cNextKey[0] == 27) // ESC
			{
				// �޴�
				if (cMyTurn == cWhosTurn)
					Send(opponentSock, cNextKey);
				cMenuMode = 1;
				cGameMode = 1;
				break;
			}

			if ((cNextKey[0] == 72) || (cNextKey[0] == 75) || (cNextKey[0] == 77) || (cNextKey[0] == 80))
			{
				stCursor.pvnCursorOnDolMat = MoveCursor(vnDolMatrix, stCursor.pvnCursorOnDolMat, stCursor.vnCursorCoords, cNextKey[0], cWhosTurn); // Ŀ���� ��ġ�� �ٲٰ� �ٲ� ��ġ�� ����
			}

			if (cNextKey[0] == 32)
			{
				if (*(stCursor.pvnCursorOnDolMat) == 0 || *(stCursor.pvnCursorOnDolMat) == 10)
				{
					PutDol(stCursor.pvnCursorOnDolMat, stCursor.vnCursorCoords, cWhosTurn);
					if (JudgeGameEnded(vnDolMatrix, stCursor.pvnCursorOnDolMat, cWhosTurn) != 0) // ������ �¸�
					{
						// 5�� ����
						system("color 60");
						gotoxy(stCursor.vnCursorCoords[0], stCursor.vnCursorCoords[1]);
						if (cWhosTurn == 1)
							printf("��");
						else
							printf("��");
						gotoxy(0, 16);
						if (cMyTurn == cWhosTurn)	// ���� �浹�̸�
							printf("�¸��ϼ̽��ϴ�                                                \n");
						else
							printf("�й��ϼ̽��ϴ�                                                \n");
						gotoxy(0, 18);
						printf("                                 ");
						
						// �ٽ� ���� ��� ����
						// �ٽ����� �����
						if (cMyTurn == cWhosTurn) { // �����̸�
							if (Regame() == 1) { // �ٽ��ϱ�
								cNextKey[0] = '1';
								cNextKey[1] = '\0';
								Send(opponentSock, cNextKey);
								gotoxy(0, 18);
								for (int i = 0; i < 3; i++)
									printf("                          \n");
								IgnoreKeyboardInput();
								Recv(opponentSock, cNextKey);
								IgnoreKeyboardInput();
								if (!strcmp(cNextKey, "1")) {
									// ���� �����
									InitializeToReGame(vnDolMatrix, &cWhosTurn, &cMyTurn, -1, &chance, &stCursor);
									gotoxy(0, 18);
									printf("                                      ");
									gotoxy(0, 19);
									printf(" ������ ������ �����ϼ̽��ϴ�    ");
									gotoxy(0, 20);
									printf("                                      ");
									Sleep(2);
									gotoxy(0, 19);
									printf("                                     ");
									PrintPan(vnPanMatrix, vnDolMatrix, cWhosTurn);
									continue;
								}
								else {
									gotoxy(0, 18);
									printf("                                      ");
									gotoxy(0, 19);
									printf(" ������ ������ �����ϼ̽��ϴ�   ");
									gotoxy(0, 20);
									printf("                                      ");
									_getch();
								}

							} else {
								// �簳���� ����
								Send(opponentSock, "0");
							}
						} else {
							if (Regame() == 1) {
								IgnoreKeyboardInput();
								Recv(opponentSock, cNextKey);
								IgnoreKeyboardInput();
								if (!strcmp(cNextKey, "1")) {
									//gotoxy(0, 19);
									//printf("                                     ");
									// �ٽ��ϱ�
									cNextKey[0] = '1';
									cNextKey[1] = '\0';
									Send(opponentSock, cNextKey);
									// ���� �����
									InitializeToReGame(vnDolMatrix, &cWhosTurn, &cMyTurn, 1, &chance, &stCursor);
									PrintPan(vnPanMatrix, vnDolMatrix, cWhosTurn);
									continue;
								} else {
									gotoxy(0, 18);
									printf("                                      ");
									gotoxy(0, 19);
									printf(" ������ ������ �����ϼ̽��ϴ�   ");
									gotoxy(0, 20);
									printf("                                      ");
									_getch();
								}
							} else {
								// �簳���� ����
								Recv(opponentSock, cNextKey);
								Send(opponentSock, "0");
							}
						}
						// �ٽ� ���� ��� ��

						cGameMode = 0;
						cMenuMode = 1;
						if (cNetworkMode == '0')
							CloseServerSocket(&opponentSock, &listenSock, &cltAddr);
						else
							CloseClientSocket(&opponentSock, &servAddr);
						// ���� ����
						WSACleanup();
						break;
					}
					else if (Judge33Situation(vnDolMatrix, stCursor.pvnCursorOnDolMat, cWhosTurn) == 1) // 3-3 ��Ȳ
					{
						*(stCursor.pvnCursorOnDolMat) = 10;
						gotoxy(0, 21);
						if (cWhosTurn == cMyTurn)
						{
							system("color 40");
							printf("�ݼ��� �ξ����ϴ�(3-3)");
						}
						else
						{
							printf("������ �ݼ��� �õ��߽��ϴ�(3-3)");
						}
						gotoxy(stCursor.vnCursorCoords[0], stCursor.vnCursorCoords[1]);
						if (cWhosTurn == 1)
						{
							IgnoreKeyboardInput();
							gotoxy(stCursor.vnCursorCoords[0], stCursor.vnCursorCoords[1]);
							Sleep(150);
							printf("��");

							gotoxy(stCursor.vnCursorCoords[0], stCursor.vnCursorCoords[1]);
							Sleep(150);
							printf("��");

							gotoxy(stCursor.vnCursorCoords[0], stCursor.vnCursorCoords[1]);
							Sleep(150);
							printf("��");
							IgnoreKeyboardInput();
						}
						else
						{
							IgnoreKeyboardInput();
							gotoxy(stCursor.vnCursorCoords[0], stCursor.vnCursorCoords[1]);
							Sleep(150);
							printf("��");

							gotoxy(stCursor.vnCursorCoords[0], stCursor.vnCursorCoords[1]);
							Sleep(150);
							printf("��");

							gotoxy(stCursor.vnCursorCoords[0], stCursor.vnCursorCoords[1]);
							Sleep(150);
							printf("��");
							IgnoreKeyboardInput();
						}
						printf("\a");
						gotoxy(0, 21);
						Sleep(500);
						if (cWhosTurn == cMyTurn)
							system("color 60");
						printf("                                   ");
					}
					else
					{
						cWhosTurn *= -1;

						//////�����⸦ ���� �����丮 ����

						// �� �迭���� ���� ��ġ�� ����
						stCursor.pHistory[1] = stCursor.pHistory[0];
						stCursor.pHistory[0] = stCursor.pvnCursorOnDolMat;

						// �׷��Ȼ� ��ǥ�� ����
						//{
						//	{x, y}, // ��� ���� ��
						//	{x, y}	// ���� ��밡 ���� ��
						//}
						stCursor.nHistory[1][0] = stCursor.nHistory[0][0];
						stCursor.nHistory[1][1] = stCursor.nHistory[0][1];
						stCursor.nHistory[0][0] = stCursor.vnCursorCoords[0];
						stCursor.nHistory[0][1] = stCursor.vnCursorCoords[1];
					}
				}
				else
				{
					printf("\a");
				}
			}
		}
	}
	return 0;
}
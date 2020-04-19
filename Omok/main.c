#pragma comment(lib, "ws2_32") // 소켓 함수 이용을 위해 추가
#include "include.h" // include할 것들 모아둠
//#define HOME

int main(int argc, char* argv[])
{
	int nNextKey;									// 자신의 키 입력을 받아 저장할 변수(_getch())
	char cNextKey[2];								// 키 입력을 문자열로 바꿔 네트워크를 이용하기 위한 변수
	char cMenuMode = 1, cGameMode = 0;				// 게임 모드 구분을 위해 생성(지금은 굳이 필요 없음)
	char cMyTurn;									// 내 차례가 언제인지 (1 : 흑돌일 때 내 차례 / -1 : 백돌일 때 내 차례)
	char cWhosTurn = 1;								// 첫 시도는 항상 흑돌
	int vnDolMatrix[15][15];						// 돌 정보를 담는 15 X 15 배열 생성
	int vnPanMatrix[15][15];						// 판 모양을 담는 15 X 15 배열 생성

	CURSOR stCursor;
	stCursor.pvnCursorOnDolMat = &vnDolMatrix[7][7];// 돌 배열에 연결된 커서 포인터에 돌 배열의 주소를 담음(vnDolMatrix[0][0]의 주소가 담김)
	stCursor.vnCursorCoords[0] = 2 * 7;
	stCursor.vnCursorCoords[1] = 7;			// 초기 좌표 지정
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

	cursor(0); // 커서 지우기
	system("mode con cols=35 lines=25 | title 몰래오목");
	system("color 60");
	COORD bufferSize = { 35, 1 }; // 가로, 세로
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize);
	//SayHi();
	while (1)
	{
		while (1)
		{
			// 초기 화면 생성
			if (10 * cGameMode + cMenuMode == 1)			// 모드 구분. 10진수의 특성을 이용하여 10의 자리는 게임중인지 여부, 1의 자리는 메뉴를 열었는지 여부를 나타냄
			{
				// 게임 초기화 시작
				system("cls");
				InitializeDolMatrix(vnDolMatrix, 15);			// 돌 배열 초기화
				InitializePanMatrix(vnPanMatrix, 15);			// 판 배열 초기화
				cNetworkMode = -1;
				stCursor.vnCursorCoords[0] = 2 * 7;
				stCursor.vnCursorCoords[1] = 7;
				stCursor.pvnCursorOnDolMat = &vnDolMatrix[7][7];
				cWhosTurn = 1;
				chance = 1;
				// 게임 초기화 끝

				// 메인 메뉴
				if ((nReturnFromIntro = RunIntro()) == 1) // 게임 시작
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
				// 게임 화면
				while (cNetworkMode != '0' && cNetworkMode != '1') // 서버인지 클라이언트인지 선택... 0 : server / 1 : client
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

					// 윈속 초기화
					WSADATA wsaData;
					if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
					{
						return 1;
					}

					if (strcmp(szServerIp, "")) // 클라이언트
					{
						opponentSock = RequestLinkToServer(szServerIp, &cltAddr);
						cNetworkMode = '1';
						cMyTurn = -1;
						system("color 80");
					}
					else // 서버
					{
						// 끄지마
						EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_GRAYED);
						DrawMenuBar(GetConsoleWindow());

						listenSock = LaunchServer(&servAddr);
						opponentSock = AcceptLinkFromClient(listenSock, &cltAddr);
						// 꺼도 됨
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
					if (nResult == 2) { // 무르기
						if (stCursor.nHistory[1][0] != -1) {
							Murugi(&stCursor);
							chance = 0;
						} else {
							system("cls");
							gotoxy(1, 7);
							printf("사용할 수 없습니다");
						}
						system("cls");
						gotoxy(-1, 0);
						PrintPan(vnPanMatrix, vnDolMatrix, cWhosTurn);
						gotoxy(stCursor.vnCursorCoords[0], stCursor.vnCursorCoords[1] - 1);
						cGameMode = 1;
						cMenuMode = 0;
						Send(opponentSock, "m"); // 무르기
					} else if (nResult == 1) { // 게임 재개
						gotoxy(-1, 0);
						PrintPan(vnPanMatrix, vnDolMatrix, cWhosTurn);
						gotoxy(stCursor.vnCursorCoords[0], stCursor.vnCursorCoords[1] - 1);
						cGameMode = 1;
						cMenuMode = 0;
						Send(opponentSock, "c"); // continue
					} else if (nResult == 0) { // 게임 종료
						cGameMode = 0;
						cMenuMode = 1;
						Send(opponentSock, "a"); // abort
						system("cls");
						PrintPan(vnPanMatrix, vnDolMatrix, cWhosTurn);
						gotoxy(0, 15);
						printf("                                     ");
						// 다시 시작 기능 시작
						// 다시할지 물어보기
						if (Regame() == 1) { // 기권한 사람은 패자
							IgnoreKeyboardInput();
							Recv(opponentSock, cNextKey);
							IgnoreKeyboardInput();
							if (!strcmp(cNextKey, "1")) {
								gotoxy(0, 16);
								printf("                                     ");
								// 다시하기
								cNextKey[0] = '1';
								cNextKey[1] = '\0';
								Send(opponentSock, cNextKey);
								// 게임 재시작
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
								printf(" 상대방이 재대결을 거절하셨습니다   ");
								gotoxy(0, 20);
									printf("                                      ");
								_getch();
							}
						}
						else {
							// 재개하지 않음
							Recv(opponentSock, cNextKey);
							Send(opponentSock, "0");
						}
						// 다시 시작 기능 끝
						if (cNetworkMode == '0')
							CloseServerSocket(&opponentSock, &listenSock, &cltAddr);
						else
							CloseClientSocket(&opponentSock, &servAddr);
						// 윈속 종료
						WSACleanup();
						gotoxy(0, 22); // 문구를 출력할 곳 (디버그 시 중간에 떠서 거슬렸음 ㅋㅋ)
					}
				}
				else if (cWhosTurn != cMyTurn)
				{
					IgnoreKeyboardInput();
					Recv(opponentSock, cNextKey);
					IgnoreKeyboardInput(); // 입력 받기 전후로 다 무시
					if (cNextKey[0] == 'c') // 게임 재개
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
							printf("상대방이 무르기를 사용했습니다");
							gotoxy(stCursor.vnCursorCoords[0], stCursor.vnCursorCoords[1] - 1);
						}

						cGameMode = 1;
						cMenuMode = 0;
					}
					if (cNextKey[0] == 'a') // 게임 종료
					{
						cGameMode = 0;
						cMenuMode = 1;
						gotoxy(0, 16);
						system("color 60");
						printf("                                 ");
						gotoxy(0, 16);
						printf(" 승리했습니다\n  상대방이 기권하였습니다        ");
						gotoxy(0, 18);
						printf("                                   ");
						// 다시 시작 기능 시작
						// 다시할지 물어보기
						if (Regame() == 1) { // 다시하기
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
								// 게임 재시작
								InitializeToReGame(vnDolMatrix, &cWhosTurn, &cMyTurn, -1, &chance, &stCursor);
								gotoxy(0, 19);
								printf(" 상대방이 재대결을 수락하셨습니다    ");
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
								printf(" 상대방이 재대결을 거절하셨습니다   ");
								_getch();
							}

						}
						else {
							// 재개하지 않음
							Send(opponentSock, "0");
						}
						// 다시 시작 기능 끝
						if (cNetworkMode == '0')
							CloseServerSocket(&opponentSock, &listenSock, &cltAddr);
						else
							CloseClientSocket(&opponentSock, &servAddr);
						// 윈속 종료
						WSACleanup();
					}
				}
			}
		}
		if (cNetworkMode == 0) continue;
		while (1) // 여기부터
		{
			if (cMyTurn == cWhosTurn) // 내 차례일 때
			{
				gotoxy(0, 16);
				printf(" 내 차례입니다. 돌을 놓아주세요      ");
				gotoxy(0, 18);
				printf("                               ");
				gotoxy(stCursor.vnCursorCoords[0], stCursor.vnCursorCoords[1]);
				system("color 60");
				nNextKey = _getch();
				cNextKey[0] = (char)nNextKey;
				cNextKey[1] = 0; // 문자열 형태로 네트워킹이 이루어지기 때문에 NULL을 넣었음.
				if (nNextKey < 224)
					Send(opponentSock, cNextKey);
			}
			else if (cMyTurn == -cWhosTurn) // 상대방 차례일 때
			{
				gotoxy(0, 16);
				printf("상대방의 착수를 기다리는 중입니다");
				gotoxy(stCursor.vnCursorCoords[0], stCursor.vnCursorCoords[1]);
				system("color 80");
				IgnoreKeyboardInput();
				Recv(opponentSock, cNextKey);
				IgnoreKeyboardInput(); // 입력 받기 전후로 다 무시
			}
			else
			{
				gotoxy(0, 22);
				printf("누구 차례인겨? 여기가 어디여?\n");
				return 0;
			}
			if (cNextKey[0] == 27) // ESC
			{
				// 메뉴
				if (cMyTurn == cWhosTurn)
					Send(opponentSock, cNextKey);
				cMenuMode = 1;
				cGameMode = 1;
				break;
			}

			if ((cNextKey[0] == 72) || (cNextKey[0] == 75) || (cNextKey[0] == 77) || (cNextKey[0] == 80))
			{
				stCursor.pvnCursorOnDolMat = MoveCursor(vnDolMatrix, stCursor.pvnCursorOnDolMat, stCursor.vnCursorCoords, cNextKey[0], cWhosTurn); // 커서의 위치를 바꾸고 바뀐 위치를 저장
			}

			if (cNextKey[0] == 32)
			{
				if (*(stCursor.pvnCursorOnDolMat) == 0 || *(stCursor.pvnCursorOnDolMat) == 10)
				{
					PutDol(stCursor.pvnCursorOnDolMat, stCursor.vnCursorCoords, cWhosTurn);
					if (JudgeGameEnded(vnDolMatrix, stCursor.pvnCursorOnDolMat, cWhosTurn) != 0) // 누군가 승리
					{
						// 5개 연속
						system("color 60");
						gotoxy(stCursor.vnCursorCoords[0], stCursor.vnCursorCoords[1]);
						if (cWhosTurn == 1)
							printf("●");
						else
							printf("○");
						gotoxy(0, 16);
						if (cMyTurn == cWhosTurn)	// 내가 흑돌이면
							printf("승리하셨습니다                                                \n");
						else
							printf("패배하셨습니다                                                \n");
						gotoxy(0, 18);
						printf("                                 ");
						
						// 다시 시작 기능 시작
						// 다시할지 물어보기
						if (cMyTurn == cWhosTurn) { // 승자이면
							if (Regame() == 1) { // 다시하기
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
									// 게임 재시작
									InitializeToReGame(vnDolMatrix, &cWhosTurn, &cMyTurn, -1, &chance, &stCursor);
									gotoxy(0, 18);
									printf("                                      ");
									gotoxy(0, 19);
									printf(" 상대방이 재대결을 수락하셨습니다    ");
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
									printf(" 상대방이 재대결을 거절하셨습니다   ");
									gotoxy(0, 20);
									printf("                                      ");
									_getch();
								}

							} else {
								// 재개하지 않음
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
									// 다시하기
									cNextKey[0] = '1';
									cNextKey[1] = '\0';
									Send(opponentSock, cNextKey);
									// 게임 재시작
									InitializeToReGame(vnDolMatrix, &cWhosTurn, &cMyTurn, 1, &chance, &stCursor);
									PrintPan(vnPanMatrix, vnDolMatrix, cWhosTurn);
									continue;
								} else {
									gotoxy(0, 18);
									printf("                                      ");
									gotoxy(0, 19);
									printf(" 상대방이 재대결을 거절하셨습니다   ");
									gotoxy(0, 20);
									printf("                                      ");
									_getch();
								}
							} else {
								// 재개하지 않음
								Recv(opponentSock, cNextKey);
								Send(opponentSock, "0");
							}
						}
						// 다시 시작 기능 끝

						cGameMode = 0;
						cMenuMode = 1;
						if (cNetworkMode == '0')
							CloseServerSocket(&opponentSock, &listenSock, &cltAddr);
						else
							CloseClientSocket(&opponentSock, &servAddr);
						// 윈속 종료
						WSACleanup();
						break;
					}
					else if (Judge33Situation(vnDolMatrix, stCursor.pvnCursorOnDolMat, cWhosTurn) == 1) // 3-3 상황
					{
						*(stCursor.pvnCursorOnDolMat) = 10;
						gotoxy(0, 21);
						if (cWhosTurn == cMyTurn)
						{
							system("color 40");
							printf("금수를 두었습니다(3-3)");
						}
						else
						{
							printf("상대방이 금수를 시도했습니다(3-3)");
						}
						gotoxy(stCursor.vnCursorCoords[0], stCursor.vnCursorCoords[1]);
						if (cWhosTurn == 1)
						{
							IgnoreKeyboardInput();
							gotoxy(stCursor.vnCursorCoords[0], stCursor.vnCursorCoords[1]);
							Sleep(150);
							printf("○");

							gotoxy(stCursor.vnCursorCoords[0], stCursor.vnCursorCoords[1]);
							Sleep(150);
							printf("⊙");

							gotoxy(stCursor.vnCursorCoords[0], stCursor.vnCursorCoords[1]);
							Sleep(150);
							printf("●");
							IgnoreKeyboardInput();
						}
						else
						{
							IgnoreKeyboardInput();
							gotoxy(stCursor.vnCursorCoords[0], stCursor.vnCursorCoords[1]);
							Sleep(150);
							printf("●");

							gotoxy(stCursor.vnCursorCoords[0], stCursor.vnCursorCoords[1]);
							Sleep(150);
							printf("◎");

							gotoxy(stCursor.vnCursorCoords[0], stCursor.vnCursorCoords[1]);
							Sleep(150);
							printf("○");
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

						//////무르기를 위한 히스토리 저장

						// 돌 배열에서 현재 위치를 저장
						stCursor.pHistory[1] = stCursor.pHistory[0];
						stCursor.pHistory[0] = stCursor.pvnCursorOnDolMat;

						// 그래픽상 좌표를 저장
						//{
						//	{x, y}, // 방금 놓은 돌
						//	{x, y}	// 전에 상대가 놓은 돌
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
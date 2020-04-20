#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "control.h"
#include "structs.h"
#include "init.h"

/*
게임 설명을 출력합니다
*/
void PrintDescription()
{
	printf("\n 흑돌부터 번갈아 가며 돌을 둡니다.\n");
	printf(" 먼저 5목을 만드는 사람이 승리.\n\n");
	printf("\n 키 설명\n");
	printf("  돌놓기 : Space\n  메뉴 : esc\n");
	printf("  채팅 빠져나오기 : \n      내용이 비어있을 때 Enter\n\n\n");

	printf(" 아무키나 입력시 되돌아갑니다.");
	_getch();
}

/*
환영 인트로를 출력합니다
*/
void SayHi()
{
	cursor(0);
	printf("\n\n\n\n\n\n");
	printf("             Hello");
	Sleep(1000);
	gotoxy(0, -1);
	printf("\n\n\n\n\n\n");
	printf("     reverse(Hello)");
	Sleep(1000);
	gotoxy(0, -1);
	printf("\n\n\n\n\n\n");
	printf("             olleH  ");
	Sleep(1000);
	gotoxy(0, -1);
	printf("\n\n\n\n\n\n");
	printf("M            olleH  ");
	Sleep(100);
	gotoxy(0, -1);
	printf("\n\n\n\n\n\n");
	printf("  M          olleH  ");
	Sleep(100);
	gotoxy(0, -1);
	printf("\n\n\n\n\n\n");
	printf("     M       olleH  ");
	Sleep(100);
	gotoxy(0, -1);
	printf("\n\n\n\n\n\n");
	printf("        M    olleH  ");
	Sleep(100);
	gotoxy(0, -1);
	printf("\n\n\n\n\n\n");
	printf("           M olleH  ");
	Sleep(100);
	gotoxy(0, -1);
	printf("\n\n\n\n\n\n");
	printf("           M-olleH  ");
	Sleep(100);
	gotoxy(0, -1);
	printf("\n\n\n\n\n\n");
	printf("           M-olleH  ");
	Sleep(1000);
}

/*
인트로를 출력합니다
*/
void DrawIntro()
{
    int x = 11, y = 7;//초기값
    int key;
    int end = 0;
    char* menus[3] = { "게임시작", "게임설명", "게임종료" };
    system("mode con cols=35 lines=25");
    COORD bufferSize = { 35, 26 }; // 가로, 세로
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize);
    COORD bufferSize2 = { 35, 25 }; // 가로, 세로
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize2);
    cursor(0);
    gotoxy(10, 5);
    printf("M-olleH 오 목\n");
    gotoxy(x, y);//+2
    //0xa3 ┌, 0xa4 ┐, 0xa6 └, 0xa5 ┘, 0xab ┼, 0xa8 ┬, 0xaa ┴, 0xa7 ├, 0xa9 ┤, 0xa1 ─(0x2d)
    printf("%c%c---------%c%c", 0xa6, 0xa3, 0xa6, 0xa4);
    gotoxy(x, y + 1);//+2
    printf("│         │");
    gotoxy(x, y + 2);//+2
    printf("%c%c---------%c%c", 0xa6, 0xa6, 0xa6, 0xa5);
    gotoxy(13, 8);
    printf("%s", menus[0]);
    gotoxy(13, 10);
    printf("%s", menus[1]);
    gotoxy(13, 12);
    printf("%s", menus[2]);
}

/*
시작 화면을 호출합니다
	@output		선택한 기능(메뉴) 번호 (7 : 게임시작 / 9 : 게임설명 / 11 : 게임종료)
*/
int RunIntro() {
    int x = 11, y = 7;//초기값
    int key;
    char* menus[3] = { "게임시작", "게임설명", "게임종료" };
    DrawIntro();
	IgnoreKeyboardInput();
    while (1) 
    {
        key = _getch(); //키입력
        if (key == 0xe0) {//방향키일경우
            key = _getch();
            if (key >= 72) { //상하좌우
                switch (key)
                {
                case 72://상
                    if (y > 8) {
                        // 지우기
                        gotoxy(x, y);
                        printf("           ");
                        gotoxy(x, y + 1);//+2
                        printf("  %s  ", menus[(y - 6) / 2]);
                        gotoxy(x, y + 2);//+2
                        printf("           ");
                        y = y - 2;
                        // 그리기
                        gotoxy(x, y);//+2
                        printf("┌---------┐");
                        gotoxy(x, y + 1);//+2
                        printf("│ %s│", menus[(y - 6) / 2]);
                        gotoxy(x, y + 2);//+2
                        printf("└---------┘ ");
                    }
                    break;
                case 80://하
                    if (y < 11) {
                        // 지우기
                        gotoxy(x, y);
                        printf("           ");
                        gotoxy(x, y + 1);//+2
                        printf("  %s  ", menus[(y - 6) / 2]);
                        gotoxy(x, y + 2);//+2
                        printf("           ");
                        y = y + 2;
                        // 그리기
                        gotoxy(x, y);//+2
                        printf("┌---------┐");
                        gotoxy(x, y + 1);//+2
                        printf("│ %s│", menus[(y - 6)/2]);
                        gotoxy(x, y + 2);//+2
                        printf("└---------┘ ");
                    }
                    break;
                }
            }
        }
        else if (key == 32 || key == '\r') {//스페이스나 엔터를 눌렀을때
            if (y == 7) {//게임시작
                system("cls"); //화면 클리어
                return 1;//컨트롤 실행
            }
            else if (y == 9) {//게임설명
                system("cls"); //화면 클리어
				PrintDescription();
                x = 11, y = 7; // 초기값으로
                DrawIntro();
            }
            else if (y == 11) {//게임종료
                gotoxy(1, 15);
                return 0;
            }
        }

    }
}

/*
게임 중 메뉴를 출력합니다
*/
void DrawMenu()
{
	int x = 11, y = 7;//초기값
	int key;
	char* menus[4] = { "계속하기", "게임설명", " 무르기 ", "기권하기" };
	system("mode con cols=35 lines=25");
	COORD bufferSize = { 35, 26 }; // 가로, 세로
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize);
	COORD bufferSize2 = { 35, 25 }; // 가로, 세로
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize2);
	cursor(0);
	gotoxy(10, 5);
	printf("M-olleH 오 목\n");
	gotoxy(x, y);//+2
	//0xa3 ┌, 0xa4 ┐, 0xa6 └, 0xa5 ┘, 0xab ┼, 0xa8 ┬, 0xaa ┴, 0xa7 ├, 0xa9 ┤, 0xa1 ─(0x2d)
	printf("%c%c---------%c%c", 0xa6, 0xa3, 0xa6, 0xa4);
	gotoxy(x, y + 1);//+2
	printf("│         │");
	gotoxy(x, y + 2);//+2
	printf("%c%c---------%c%c", 0xa6, 0xa6, 0xa6, 0xa5);
	gotoxy(13, 8);
	printf("%s", menus[0]);
	gotoxy(13, 10);
	printf("%s", menus[1]);
	gotoxy(13, 12);
	printf("%s", menus[2]);
	gotoxy(13, 14);
	printf("%s", menus[3]);
}

/*
게임 중 메뉴를 호출합니다
	@output		선택한 기능(메뉴) 번호 (7 : 게임시작 / 9 : 게임설명 / 11 : 무르기 / 13 : 기권)
*/
int RunMenu(int chance, struct cursor* stCursor) {
	int x = 11, y = 7;//초기값
	int key;
	char* menus[4] = { "계속하기", "게임설명", " 무르기 ", "기권하기" };
	cursor(0);
	DrawMenu();
	while (1)
	{
		key = _getch(); //키입력
		if (key == 0xe0) {//방향키일경우
			key = _getch();
			if (key >= 72) { //상하좌우
				switch (key)
				{
				case 72://상
					if (y > 8) {
						// 지우기
						gotoxy(x, y);
						printf("           ");
						gotoxy(x, y + 1);//+2
						printf("  %s  ", menus[(y - 6) / 2]);
						gotoxy(x, y + 2);//+2
						printf("           ");
						if ((chance == 0 && y == 13) || (stCursor->nHistory[1][0] == -1 && y == 13))
							y = y - 4;
						else
							y = y - 2;
						// 그리기
						gotoxy(x, y);//+2
						printf("┌---------┐");
						gotoxy(x, y + 1);//+2
						printf("│ %s│", menus[(y - 6) / 2]);
						gotoxy(x, y + 2);//+2
						printf("└---------┘ ");
					}
					break;
				case 80://하
					if (y < 13) {
						// 지우기
						gotoxy(x, y);
						printf("           ");
						gotoxy(x, y + 1);//+2
						printf("  %s  ", menus[(y - 6) / 2]);
						gotoxy(x, y + 2);//+2
						printf("           ");
						if (y == 9 && (chance == 0 || stCursor->nHistory[1][0] == -1))
							y = y + 4;
						else
							y = y + 2;
						// 그리기
						gotoxy(x, y);//+2
						printf("┌---------┐");
						gotoxy(x, y + 1);//+2
						printf("│ %s│", menus[(y - 6) / 2]);
						gotoxy(x, y + 2);//+2
						printf("└---------┘ ");
					}
					break;
				}
			}
		}
		else if (key == 32 || key == '\r') {//스페이스나 엔터를 눌렀을때
			if (y == 7) {//게임시작
				system("cls"); //화면 클리어
				return 1;//컨트롤 실행
			}
			else if (y == 9) {//게임설명
				system("cls"); //화면 클리어
				PrintDescription();
				x = 11, y = 7; // 초기값으로
				DrawMenu();
			}
			else if (y == 11) { // 무르기
				return 2;
			}
			else if (y == 13) { //기권
				gotoxy(1, 15);
				return 0;
			}
		}
	}
}

/*
재대결을 할 지 물어봅니다
	@output		선택한 응답 번호(1 : 다시 / 13 : 종료)
*/
int Regame()
{
	int x = 1;
	int key;
	gotoxy(0, 18);
	printf("   ┌---------┐\n ");
	printf("   │ 다시하기│   종료하기\n ");
	printf("   └---------┘ ");
	while (1) {
		if ((key = _getch()) == 224) {
			switch (_getch()) {
			case 75: // 왼쪽
				x = 1;
				gotoxy(0, 18);
				printf("   ┌---------┐              \n ");
				printf("   │ 다시하기│   종료하기    \n ");
				printf("   └---------┘              ");
				break;

			case 77: // 오른쪽
				x = 13;
				gotoxy(0, 18);
				printf("               ┌---------┐\n ");
				printf("     다시하기  │ 종료하기│\n ");
				printf("               └---------┘    ");
				break;

			}
		}
		else if (key == 32 || key == '\r') { // 스페이스 or 엔터
			gotoxy(0, 18);
			printf("                            \n ");
			printf("                            \n ");
			printf("                               ");
			return x;
		}
	}
}
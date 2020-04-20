#include <stdio.h>
#include <windows.h>
#include "init.h"
#include "structs.h"
#include "control.h"

/*
커서 보이기 & 숨기기
	@prarm		n		보일지 숨길지 (0 : 숨기기 / 1 : 보이기)
*/
void cursor(int n)
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = n;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

/*
판을 출력합니다
@param	vnPanMatrix[][]		판 모양이 담긴 배열(가로 세로 사이즈를 맞추지 않은 배열)
@param	vnDolMatrix[][]		돌 정보가 담긴 배열
@param	cWhosTurn			현재 차례
@return	없음
*/
void PrintPan(int vnPanMatrix[15][15], int vnDolMatrix[15][15], char cWhosTurn)
{
	//0x01 ┌, 0x02 ┐, 0x03 └, 0x04 ┘, 0x10 ┼, 0x16 ┬, 0x15 ┴, 0x19 ├, 0x17 ┤, 0x06 ─
	//0xa3 ┌, 0xa4 ┐, 0xa6 └, 0xa5 ┘, 0xab ┼, 0xa8 ┬, 0xaa ┴, 0xa7 ├, 0xa9 ┤, 0xa1 ─(0x2d)
	gotoxy(-1, 0);
	for (int i = 0; i < 15; i++)				// 판
	{
		printf(" ");
		for (int j = 0; j < 15; j++)
		{
			switch (vnDolMatrix[i][j])
			{
			case 1:
				printf("●");
				break;
			case 11:
				printf("⊙");
				break;
			case -1:
				printf("○");
				break;
			case 9:
				printf("◎");
				break;
			case 10:
				cWhosTurn == 1 ? printf("●") : printf("○");
				break;
			case 0:
				printf("%c%c", 0xa6 , vnPanMatrix[i][j]);	// 커서가 없는 칸은 기존의 판 모양을 출력
				(j < 14) ? printf("%c", 0x2d) : printf(""); // 각 행의 마지막 열이 아니라면 가로바(─)를 추가로 출력
				break;
			}
		}
		printf("\n");
	}
}

/*
 nSize X nSize 사이즈의 돌 위치 배열을 0으로 초기화 합니다(커서의 초기 위치에 10 추가)
 @param		pvMatrix		선언한 돌 위치 배열의 주소(포인터)
 @param		nSize			오목판 사이즈
 @output	없음
 */
void InitializeDolMatrix(int* pvMatrix, int nSize)
{
	for (int i = 0; i < nSize * nSize; i++)
	{
		if (i == 15 * 7 + 7)
		{
			*pvMatrix = 10;
		}
		else
		{
			*pvMatrix = 0;
		}
		pvMatrix++;
	}
}

/*
 nSize X nSize 사이즈의 오목판 배열을 초기화 합니다
 @param		pvMatrix		선언한 오목판 배열의 주소(포인터)
 @param		nSize			오목판 사이즈
 @output	없음
 */
void InitializePanMatrix(int* pvnMatrix, int nSize)
{
	//0x01 ┌, 0x02 ┐, 0x03 └, 0x04 ┘, 0x10 ┼, 0x16 ┬, 0x15 ┴, 0x19 ├, 0x17 ┤, 0x06 ─
	//0xa3 ┌, 0xa4 ┐, 0xa6 └, 0xa5 ┘, 0xab ┼, 0xa8 ┬, 0xaa ┴, 0xa7 ├, 0xa9 ┤, 0xa1 ─
	for (int i = 0; i < nSize; i++)
	{
		(i == 0) ? (*pvnMatrix = 0xa3) : ((i == nSize - 1) ? (*pvnMatrix = 0xa6) : (*pvnMatrix = 0xa7));
		pvnMatrix++;
		for (int j = 0; j < nSize - 2; j++)
		{
			(i == 0) ? (*pvnMatrix = 0xa8) : ((i == nSize - 1) ? (*pvnMatrix = 0xaa) : (*pvnMatrix = 0xab));
			pvnMatrix++;
		}
		(i == 0) ? (*pvnMatrix = 0xa4) : ((i == nSize - 1) ? (*pvnMatrix = 0xa5) : (*pvnMatrix = 0xa9));
		pvnMatrix++;
	}
}
/*
게임 재시작을 위해 변수들을 초기화 합니다
@param	vnDolMatrix[][]		돌 정보가 담긴 배열
@param	cWhosTurn			현재 차례
@prarm	cMyTurn				내 차례 정보가 담긴 변수의 주소
@prarm	cNewMyTurn			새 게임에서 이용할 돌(1 : 흑 / -1 : 백)
@prarm	chance				무르기 기회가 담긴 변수의 주소
@prarm	stCursor			커서 구조체의 주소
@output 없음
*/
void InitializeToReGame(int vnDolMatrix[15][15], char* cWhosTurn, char* cMyTurn, char cNewMyTurn, int* chance, struct cursor* stCursor)
{
	system("cls");
	InitializeDolMatrix(vnDolMatrix, 15);			// 돌 배열 초기화
	stCursor->vnCursorCoords[0] = 2 * 7;			// 초기 x좌표 변경(중앙으로)
	stCursor->vnCursorCoords[1] = 7;				// 초기 y좌표 변경(중앙으로)
	stCursor->pvnCursorOnDolMat = &vnDolMatrix[7][7]; // 초기 돌 배열의 커서 포인터의 위치를 중앙으로 변경
	stCursor->pHistory[0] = NULL;
	stCursor->pHistory[1] = NULL;
	stCursor->nHistory[0][0] = -1;
	stCursor->nHistory[0][1] = -1;
	stCursor->nHistory[1][0] = -1;
	stCursor->nHistory[1][1] = -1;
	*cWhosTurn = 1;									// 처음 시작은 흑돌
	*cMyTurn = cNewMyTurn;							// 내 돌이 무슨 색인지 저장
	*chance = 1;									// 무르기 기회 초기화(1번)
}
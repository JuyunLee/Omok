//0x01 ┌, 0x02 ┐, 0x03 └, 0x04 ┘, 0x10 ┼, 0x16 ┬, 0x15 ┴, 0x19 ├, 0x17 ┤, 0x06 ─
//0xa3 ┌, 0xa4 ┐, 0xa6 └, 0xa5 ┘, 0xab ┼, 0xa8 ┬, 0xaa ┴, 0xa7 ├, 0xa9 ┤, 0xa1 ─(0x2d)

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>	// 커맨드창에서 커서의 위치로 이동하기 위해 필요
#include "control.h"
#include "structs.h"

/*
x,y 좌표에 해당하는 곳으로 이동합니다
@param      x       x좌표
@param      y       y좌표
@output     없음
*/
void gotoxy(int x, int y) //특정 위치에 출력을 하고 싶을 때 사용하는 함수
{
	COORD Pos;

	Pos.X = x + 1;
	Pos.Y = y + 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos); //를 통해 gotoxy의 위치 지정
}

/*
커서의 위치를 움직입니다
@param		pvnDolMatrix			돌 배열
@param		pvnCursorOnDolMat		돌 배열과 연결된 커서 포인터
@param		vnCursorCoords			커서의 콘솔창 상 좌표
@param		nDirection				방향 구분 번호(↑, ←, →, ↓)
@param		cWhosTurn				누구 차례인지(1 : 흑돌 / -1 : 백돌)
@output		pvnCursorOnDolMat		이동 후 커서의 주소값
*/
int* MoveCursor(int* pvnDolMatrix, int* pvnCursorOnDolMat, int vnCursorCoords[], int nDirection, char cWhosTurn)
{
	int nCursorCoord;
	*pvnCursorOnDolMat = *pvnCursorOnDolMat - 10; // 기존에 있던 자리에서 커서 표시(10) 제거
	int x = vnCursorCoords[0] / 2;
	int y = vnCursorCoords[1];
	// 콘솔창 커서 제거 시작
	gotoxy(vnCursorCoords[0], vnCursorCoords[1]); // 콘솔창 커서 위치 조정
	switch (*pvnCursorOnDolMat)
	{
	case -1:	// 커서를 움직인 후 백돌만 존재
		printf("○");
		break;

	case 0:		// 아무 것도 없음 -> 판 그리기
		//0xa3 ┌, 0xa4 ┐, 0xa6 └, 0xa5 ┘, 0xab ┼, 0xa8 ┬, 0xaa ┴, 0xa7 ├, 0xa9 ┤, 0xa1 ─(0x2d)
		nCursorCoord = y * 100 + x; // 죄표를 숫자 하나로 나타냄(2자리씩, yyxx)
		if (nCursorCoord < 100) // 제일 윗줄
		{
			(nCursorCoord == 0) ? printf("%c%c%c",0xa6 , 0xa3, 0x2d) : ((nCursorCoord == 14) ? printf("%c%c ", 0xa6, 0xa4) : printf("%c%c%c", 0xa6, 0xa8, 0x2d));
		}
		else if (nCursorCoord >= 1400) // 제일 아랫줄
		{
			(nCursorCoord == 1400) ? printf("%c%c%c", 0xa6, 0xa6, 0x2d) : ((nCursorCoord == 1414) ? printf("%c%c ", 0xa6, 0xa5) : printf("%c%c%c", 0xa6, 0xaa, 0x2d));
		}
		else // 중간 줄
		{
			(nCursorCoord % 100 == 0) ? printf("%c%c%c", 0xa6, 0xa7, 0x2d) : ((nCursorCoord % 100 == 14) ? printf("%c%c ", 0xa6, 0xa9) : printf("%c%c%c", 0xa6, 0xab, 0x2d));
		}
		break;

	case 1:	// 커서를 움직인 후 흑돌만 존재
		printf("●");
		break;
	}
	// 콘솔창 커서 제거 끝
	gotoxy(vnCursorCoords[0], vnCursorCoords[1]);
	// 돌 배열 커서 및 콘솔창 커서 이동
	switch (nDirection)
	{
	case 72:
		if (y > 0) // 주소 비교 : y좌표가 0보다 클 때
		{
			// 첫 셀로부터의 거리가 14 이하
			//   -> 첫 번째 줄이다 
			//    -> 거리가 14보다 크면 첫 번째 줄이 아니다
			//     -> 위로 이동해도 된다
			pvnCursorOnDolMat -= 15; // 커서 옮기기
			vnCursorCoords[1] -= 1; // 실행창에 보이는 외부 커서의 위치 변경
		}
		break;
	case 75:
		if (x > 0) // 주소 비교 : x 좌표(콘솔창 좌표의 x를 2로 나눔)가 0보다 클 때
		{
			// 가장 왼쪽 줄은 15의 배수 
			//   -> 15로 나눈 나머지가 0
			//    -> 현재 커서가 왼쪽 끝이다
			//     -> 15로 나눈 나머지가 0이 아니면(0보다 크면) 왼쪽 끝이 아니다
			//      -> 왼쪽으로 이동해도 된다
			pvnCursorOnDolMat--; // 커서 옮기기
			vnCursorCoords[0] -= 2;	// 실행창에 보이는 외부 커서의 위치 변경
		}
		break;
	case 77:
		if (x < 14) // 주소 비교 :  x 좌표(콘솔창 좌표의 x를 2로 나눔)가 14보다 작을 때
		{
			// 가장 오른쪽 줄의 값에 1을 더하면 15의 배수 
			//   -> 1을 더해서 15로 나눈 나머지가 0 
			//    -> 현재 커서가 오른쪽 끝이다 
			//     -> 1을 더해서 15로 나눈 나머지가 0이 아니면(0보다 크면) 오른쪽 끝이 아니다 
			//      -> 오른쪽으로 이동해도 된다
			pvnCursorOnDolMat++; // 커서 옮기기
			vnCursorCoords[0] += 2; // 실행창에 보이는 외부 커서의 위치 변경
		}
		break;
	case 80:
		if (y < 14) // 주소 비교 : y좌표가 14보다 작을 때
		{
			pvnCursorOnDolMat += 15; // 커서 옮기기
			vnCursorCoords[1] += 1; // 실행창에 보이는 외부 커서의 위치 변경
		}
		break;
	}
	*pvnCursorOnDolMat += 10; // 새 자리에 표시
	gotoxy(vnCursorCoords[0], vnCursorCoords[1]); // 새 자리로 이동
	switch (*pvnCursorOnDolMat)
	{
	case 9:	// 백돌과 커서가 있을 때
		printf("◎");
		break;

	case 10: // 커서만 있을 때
		cWhosTurn == 1 ? printf("●") : printf("○");
		break;

	case 11: // 흑돌과 커서가 있을 때
		printf("⊙");
		break;
	}
	gotoxy(vnCursorCoords[0], vnCursorCoords[1]); // 출력하면 커서가 이동 -> 되돌려줌

	return pvnCursorOnDolMat;
}

/*
커서가 위치한 좌표에 돌을 놓습니다
@param      pvnCursorOnDolMat	돌 배열과 연결된 커서 포인터
@param      vnCursorCoords[]	커서의 콘솔창 상 좌표
@param      cWhosTurn			누구 차례인지(1 : 흑돌 / -1 : 백돌)
@output     없음
*/
void PutDol(int* pvnCursorOnDolMat, int* pvnCursorCoords, int cWhosTurn)
{
	*pvnCursorOnDolMat += cWhosTurn; // 돌판에서 커서가 가리키고 있는 곳의 수에 백돌은 -1, 흑돌은 1을 더함
	gotoxy(pvnCursorCoords[0], pvnCursorCoords[1]); // 커서의 위치로 이동
	if (cWhosTurn == 1)	// 흑돌 차례
	{
		printf("⊙"); // 바둑돌과 커서 출력
	}
	else				// 백돌 차례
	{
		printf("◎"); // 바둑돌과 커서 출력
	}
}

/*
키보드 입력을 무시합니다
*/
void IgnoreKeyboardInput()
{
	while (_kbhit()) // 키보드 입력이 들어오는 동안
	{
		_getch();	// 받아서 버림
	}
}

/*
무르기를 실시합니다
@param		stCursor	커서 구조체의 주소
*/
void Murugi(struct cursor* stCursor)
{
	// 현재 위치 커서 표시 제거
	*(stCursor->pvnCursorOnDolMat) -= 10;

	for (int i = 0; i < 2; i++) {
		// 돌배열
		*(stCursor->pHistory[i]) = 0;
	}
	stCursor->pvnCursorOnDolMat = stCursor->pHistory[1];
	*(stCursor->pvnCursorOnDolMat) += 10;
	stCursor->vnCursorCoords[0] = stCursor->nHistory[1][0];
	stCursor->vnCursorCoords[1] = stCursor->nHistory[1][1];
	stCursor->pHistory[0] = NULL;
	stCursor->pHistory[1] = NULL;
	stCursor->nHistory[0][0] = -1;
	stCursor->nHistory[0][1] = -1;
	stCursor->nHistory[1][0] = -1;
	stCursor->nHistory[1][1] = -1;
}
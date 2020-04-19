#pragma once

/*
콘솔창 커서를 x,y 좌표에 해당하는 곳으로 이동합니다
@param      x       x좌표
@param      y       y좌표
@output     없음
*/
void gotoxy(int x, int y);

/*
커서의 위치를 움직입니다
@param		pvnCursorOnDolMat		돌 배열과 연결된 커서 포인터
@param		vnCursorCoords			커서의 콘솔창 상 좌표
@param		nDirection				방향 구분 번호(↑, ←, →, ↓)
@param		cWhosTurn				누구 차례인지(1 : 흑돌 / -1 : 백돌)
@output		pvnCursorOnDolMat		이동 후 커서의 주소값
*/
int* MoveCursor(int* pvnDolMatrix, int* pvnCursorOnDolMat, int vnCursorCoords[], int nDirection, char cWhosTurn);

/*
커서가 위치한 좌표에 돌을 놓습니다
@param      pvnCursorOnDolMat	돌 배열과 연결된 커서 포인터
@param      vnCursorCoords[]	커서의 콘솔창 상 좌표
@param      cWhosTurn			누구 차례인지(1 : 흑돌 / -1 : 백돌)
@output     없음
*/
void PutDol(int* pvnCursorOnDolMat, int* pvnCursorCoords, int cWhosTurn);

/*
키보드 입력을 무시합니다
*/
void IgnoreKeyboardInput();

/*
무르기를 실시합니다
*/
void Murugi(struct cursor* stCursor);
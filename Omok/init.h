#pragma once

void cursor(int n); // 커서 보이기 & 숨기기

/*
판을 출력합니다
@param	vnPanMatrix[][]		판 모양이 담긴 배열(가로 세로 사이즈를 맞추지 않은 배열)
@param	vnDolMatrix[][]		돌 정보가 담긴 배열
@param	cWhosTurn			현재 차례
@return	없음
*/
void PrintPan(int vnPanMatrix[15][15], int vnDolMatrix[15][15], char cWhosTurn);

/*
 nSize X nSize 사이즈의 돌 위치 배열을 0으로 초기화 합니다(커서의 초기 위치에 10 추가)
 @param		pvMatrix		선언한 돌 위치 배열의 주소(포인터)
 @param		nSize			오목판 사이즈
 @output	없음
 */
void InitializeDolMatrix(int* pvMatrix, int nSize);

/*
 nSize X nSize 사이즈의 오목판 배열을 초기화 합니다
 @param		pvMatrix		선언한 오목판 배열의 주소(포인터)
 @param		nSize			오목판 사이즈
 @output	없음
 */
void InitializePanMatrix(int* pvnMatrix, int nSize);

void InitializeToReGame(int vnDolMatrix[15][15], char* cWhosTurn, char* cMyTurn, char cNewMyTurn, int* chance, struct cursor* stCursor);
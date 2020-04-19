#pragma once
/*
돌이 연속으로 5개 놓여서 게임이 끝났는지 판단합니다
@param      pvnDolMatrix		돌 배열 주소
@param      pvnCursorOnDolMat   돌 배열과 연결된 커서 포인터
@param      cWhosTurn           차례
@output     char                게임 종료 여부(1 : 흑돌 승리 / 0 : 진행 / -1 : 백돌 승리 ---> cWhosTurn을 그대로 출력하면 같은 결과가 나옴)
*/
char JudgeGameEnded(int* pvnDolMatrix, int* pvnCursorOnDolMat, char cWhosTurn);

/*
돌을 놓으면 3상황이 되는지 판단합니다
@param      vnDolMatrix[][]     돌 배열
@param      pvnCursorOnDolMat   돌 배열과 연결된 커서 포인터
@param      cWhosTurn           차례
@param      cDirection          방향('L'eft / 'R'ight / 'U'p / 'D'own)
@output     char                3 상황 여부(1 : 맞음 / 0 : 아님)
*/
char Judge3Situation(int vnDolMatrix[15][15], int* pvnCursorOnDolMat, char cWhosTurn, char cDirection);

/*
돌을 놓으면 3-3상황이 되는지 판단합니다
@param      vnDolMatrix         돌 배열
@param      pvnCursorOnDolMat   돌 배열과 연결된 커서 포인터
@param      cWhosTurn           차례
@output     char                3-3 상황 여부(1 : 맞음 / 0 : 아님)
*/
char Judge33Situation(int vnDolMatrix[15][15], int* pvnCursorOnDolMat, char cWhosTurn);
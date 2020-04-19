#pragma once
typedef struct cursor { // 커서 정보를 담는 구조체
	int* pvnCursorOnDolMat;
	int vnCursorCoords[2];
	int* pHistory[2];								// pvnCursorOnDolMat 저장
	int nHistory[2][2];								// vnCursorCoords 저장
} CURSOR;

typedef struct message {
	char msg[100];
	int roomnum;
} MESSAGE;
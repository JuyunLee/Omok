#pragma once
typedef struct cursor { // 커서 정보를 담는 구조체
	int* pvnCursorOnDolMat;
	int vnCursorCoords[2];
	int* pHistory[2];								// pvnCursorOnDolMat 저장
	int nHistory[2][2];								// vnCursorCoords 저장
} CURSOR;

typedef struct message { // 로비에 보낼 메시지 구조체 (형변환을 이용해서 주고받으면 됨 : (char*)&MESSAGE -> sendto)
	char msg[100];
	int roomnum;
} MESSAGE;
#pragma once
typedef struct cursor { // Ŀ�� ������ ��� ����ü
	int* pvnCursorOnDolMat;
	int vnCursorCoords[2];
	int* pHistory[2];								// pvnCursorOnDolMat ����
	int nHistory[2][2];								// vnCursorCoords ����
} CURSOR;

typedef struct message { // �κ� ���� �޽��� ����ü (����ȯ�� �̿��ؼ� �ְ������ �� : (char*)&MESSAGE -> sendto)
	char msg[100];
	int roomnum;
} MESSAGE;
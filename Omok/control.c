//0x01 ��, 0x02 ��, 0x03 ��, 0x04 ��, 0x10 ��, 0x16 ��, 0x15 ��, 0x19 ��, 0x17 ��, 0x06 ��
//0xa3 ��, 0xa4 ��, 0xa6 ��, 0xa5 ��, 0xab ��, 0xa8 ��, 0xaa ��, 0xa7 ��, 0xa9 ��, 0xa1 ��(0x2d)

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>	// Ŀ�ǵ�â���� Ŀ���� ��ġ�� �̵��ϱ� ���� �ʿ�
#include "control.h"
#include "structs.h"

/*
x,y ��ǥ�� �ش��ϴ� ������ �̵��մϴ�
@param      x       x��ǥ
@param      y       y��ǥ
@output     ����
*/
void gotoxy(int x, int y) //Ư�� ��ġ�� ����� �ϰ� ���� �� ����ϴ� �Լ�
{
	COORD Pos;

	Pos.X = x + 1;
	Pos.Y = y + 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos); //�� ���� gotoxy�� ��ġ ����
}

/*
Ŀ���� ��ġ�� �����Դϴ�
@param		pvnDolMatrix			�� �迭
@param		pvnCursorOnDolMat		�� �迭�� ����� Ŀ�� ������
@param		vnCursorCoords			Ŀ���� �ܼ�â �� ��ǥ
@param		nDirection				���� ���� ��ȣ(��, ��, ��, ��)
@param		cWhosTurn				���� ��������(1 : �浹 / -1 : �鵹)
@output		pvnCursorOnDolMat		�̵� �� Ŀ���� �ּҰ�
*/
int* MoveCursor(int* pvnDolMatrix, int* pvnCursorOnDolMat, int vnCursorCoords[], int nDirection, char cWhosTurn)
{
	int nCursorCoord;
	*pvnCursorOnDolMat = *pvnCursorOnDolMat - 10; // ������ �ִ� �ڸ����� Ŀ�� ǥ��(10) ����
	int x = vnCursorCoords[0] / 2;
	int y = vnCursorCoords[1];
	// �ܼ�â Ŀ�� ���� ����
	gotoxy(vnCursorCoords[0], vnCursorCoords[1]); // �ܼ�â Ŀ�� ��ġ ����
	switch (*pvnCursorOnDolMat)
	{
	case -1:	// Ŀ���� ������ �� �鵹�� ����
		printf("��");
		break;

	case 0:		// �ƹ� �͵� ���� -> �� �׸���
		//0xa3 ��, 0xa4 ��, 0xa6 ��, 0xa5 ��, 0xab ��, 0xa8 ��, 0xaa ��, 0xa7 ��, 0xa9 ��, 0xa1 ��(0x2d)
		nCursorCoord = y * 100 + x; // ��ǥ�� ���� �ϳ��� ��Ÿ��(2�ڸ���, yyxx)
		if (nCursorCoord < 100) // ���� ����
		{
			(nCursorCoord == 0) ? printf("%c%c%c",0xa6 , 0xa3, 0x2d) : ((nCursorCoord == 14) ? printf("%c%c ", 0xa6, 0xa4) : printf("%c%c%c", 0xa6, 0xa8, 0x2d));
		}
		else if (nCursorCoord >= 1400) // ���� �Ʒ���
		{
			(nCursorCoord == 1400) ? printf("%c%c%c", 0xa6, 0xa6, 0x2d) : ((nCursorCoord == 1414) ? printf("%c%c ", 0xa6, 0xa5) : printf("%c%c%c", 0xa6, 0xaa, 0x2d));
		}
		else // �߰� ��
		{
			(nCursorCoord % 100 == 0) ? printf("%c%c%c", 0xa6, 0xa7, 0x2d) : ((nCursorCoord % 100 == 14) ? printf("%c%c ", 0xa6, 0xa9) : printf("%c%c%c", 0xa6, 0xab, 0x2d));
		}
		break;

	case 1:	// Ŀ���� ������ �� �浹�� ����
		printf("��");
		break;
	}
	// �ܼ�â Ŀ�� ���� ��
	gotoxy(vnCursorCoords[0], vnCursorCoords[1]);
	// �� �迭 Ŀ�� �� �ܼ�â Ŀ�� �̵�
	switch (nDirection)
	{
	case 72:
		if (y > 0) // �ּ� �� : y��ǥ�� 0���� Ŭ ��
		{
			// ù ���κ����� �Ÿ��� 14 ����
			//   -> ù ��° ���̴� 
			//    -> �Ÿ��� 14���� ũ�� ù ��° ���� �ƴϴ�
			//     -> ���� �̵��ص� �ȴ�
			pvnCursorOnDolMat -= 15; // Ŀ�� �ű��
			vnCursorCoords[1] -= 1; // ����â�� ���̴� �ܺ� Ŀ���� ��ġ ����
		}
		break;
	case 75:
		if (x > 0) // �ּ� �� : x ��ǥ(�ܼ�â ��ǥ�� x�� 2�� ����)�� 0���� Ŭ ��
		{
			// ���� ���� ���� 15�� ��� 
			//   -> 15�� ���� �������� 0
			//    -> ���� Ŀ���� ���� ���̴�
			//     -> 15�� ���� �������� 0�� �ƴϸ�(0���� ũ��) ���� ���� �ƴϴ�
			//      -> �������� �̵��ص� �ȴ�
			pvnCursorOnDolMat--; // Ŀ�� �ű��
			vnCursorCoords[0] -= 2;	// ����â�� ���̴� �ܺ� Ŀ���� ��ġ ����
		}
		break;
	case 77:
		if (x < 14) // �ּ� �� :  x ��ǥ(�ܼ�â ��ǥ�� x�� 2�� ����)�� 14���� ���� ��
		{
			// ���� ������ ���� ���� 1�� ���ϸ� 15�� ��� 
			//   -> 1�� ���ؼ� 15�� ���� �������� 0 
			//    -> ���� Ŀ���� ������ ���̴� 
			//     -> 1�� ���ؼ� 15�� ���� �������� 0�� �ƴϸ�(0���� ũ��) ������ ���� �ƴϴ� 
			//      -> ���������� �̵��ص� �ȴ�
			pvnCursorOnDolMat++; // Ŀ�� �ű��
			vnCursorCoords[0] += 2; // ����â�� ���̴� �ܺ� Ŀ���� ��ġ ����
		}
		break;
	case 80:
		if (y < 14) // �ּ� �� : y��ǥ�� 14���� ���� ��
		{
			pvnCursorOnDolMat += 15; // Ŀ�� �ű��
			vnCursorCoords[1] += 1; // ����â�� ���̴� �ܺ� Ŀ���� ��ġ ����
		}
		break;
	}
	*pvnCursorOnDolMat += 10; // �� �ڸ��� ǥ��
	gotoxy(vnCursorCoords[0], vnCursorCoords[1]); // �� �ڸ��� �̵�
	switch (*pvnCursorOnDolMat)
	{
	case 9:	// �鵹�� Ŀ���� ���� ��
		printf("��");
		break;

	case 10: // Ŀ���� ���� ��
		cWhosTurn == 1 ? printf("��") : printf("��");
		break;

	case 11: // �浹�� Ŀ���� ���� ��
		printf("��");
		break;
	}
	gotoxy(vnCursorCoords[0], vnCursorCoords[1]); // ����ϸ� Ŀ���� �̵� -> �ǵ�����

	return pvnCursorOnDolMat;
}

/*
Ŀ���� ��ġ�� ��ǥ�� ���� �����ϴ�
@param      pvnCursorOnDolMat	�� �迭�� ����� Ŀ�� ������
@param      vnCursorCoords[]	Ŀ���� �ܼ�â �� ��ǥ
@param      cWhosTurn			���� ��������(1 : �浹 / -1 : �鵹)
@output     ����
*/
void PutDol(int* pvnCursorOnDolMat, int* pvnCursorCoords, int cWhosTurn)
{
	*pvnCursorOnDolMat += cWhosTurn; // ���ǿ��� Ŀ���� ����Ű�� �ִ� ���� ���� �鵹�� -1, �浹�� 1�� ����
	gotoxy(pvnCursorCoords[0], pvnCursorCoords[1]); // Ŀ���� ��ġ�� �̵�
	if (cWhosTurn == 1)	// �浹 ����
	{
		printf("��"); // �ٵϵ��� Ŀ�� ���
	}
	else				// �鵹 ����
	{
		printf("��"); // �ٵϵ��� Ŀ�� ���
	}
}

/*
Ű���� �Է��� �����մϴ�
*/
void IgnoreKeyboardInput()
{
	while (_kbhit()) // Ű���� �Է��� ������ ����
	{
		_getch();	// �޾Ƽ� ����
	}
}

/*
�����⸦ �ǽ��մϴ�
@param		stCursor	Ŀ�� ����ü�� �ּ�
*/
void Murugi(struct cursor* stCursor)
{
	// ���� ��ġ Ŀ�� ǥ�� ����
	*(stCursor->pvnCursorOnDolMat) -= 10;

	for (int i = 0; i < 2; i++) {
		// ���迭
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
#include <stdio.h>
#include <windows.h>
#include "init.h"
#include "structs.h"
#include "control.h"

/*
Ŀ�� ���̱� & �����
	@prarm		n		������ ������ (0 : ����� / 1 : ���̱�)
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
���� ����մϴ�
@param	vnPanMatrix[][]		�� ����� ��� �迭(���� ���� ����� ������ ���� �迭)
@param	vnDolMatrix[][]		�� ������ ��� �迭
@param	cWhosTurn			���� ����
@return	����
*/
void PrintPan(int vnPanMatrix[15][15], int vnDolMatrix[15][15], char cWhosTurn)
{
	//0x01 ��, 0x02 ��, 0x03 ��, 0x04 ��, 0x10 ��, 0x16 ��, 0x15 ��, 0x19 ��, 0x17 ��, 0x06 ��
	//0xa3 ��, 0xa4 ��, 0xa6 ��, 0xa5 ��, 0xab ��, 0xa8 ��, 0xaa ��, 0xa7 ��, 0xa9 ��, 0xa1 ��(0x2d)
	gotoxy(-1, 0);
	for (int i = 0; i < 15; i++)				// ��
	{
		printf(" ");
		for (int j = 0; j < 15; j++)
		{
			switch (vnDolMatrix[i][j])
			{
			case 1:
				printf("��");
				break;
			case 11:
				printf("��");
				break;
			case -1:
				printf("��");
				break;
			case 9:
				printf("��");
				break;
			case 10:
				cWhosTurn == 1 ? printf("��") : printf("��");
				break;
			case 0:
				printf("%c%c", 0xa6 , vnPanMatrix[i][j]);	// Ŀ���� ���� ĭ�� ������ �� ����� ���
				(j < 14) ? printf("%c", 0x2d) : printf(""); // �� ���� ������ ���� �ƴ϶�� ���ι�(��)�� �߰��� ���
				break;
			}
		}
		printf("\n");
	}
}

/*
 nSize X nSize �������� �� ��ġ �迭�� 0���� �ʱ�ȭ �մϴ�(Ŀ���� �ʱ� ��ġ�� 10 �߰�)
 @param		pvMatrix		������ �� ��ġ �迭�� �ּ�(������)
 @param		nSize			������ ������
 @output	����
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
 nSize X nSize �������� ������ �迭�� �ʱ�ȭ �մϴ�
 @param		pvMatrix		������ ������ �迭�� �ּ�(������)
 @param		nSize			������ ������
 @output	����
 */
void InitializePanMatrix(int* pvnMatrix, int nSize)
{
	//0x01 ��, 0x02 ��, 0x03 ��, 0x04 ��, 0x10 ��, 0x16 ��, 0x15 ��, 0x19 ��, 0x17 ��, 0x06 ��
	//0xa3 ��, 0xa4 ��, 0xa6 ��, 0xa5 ��, 0xab ��, 0xa8 ��, 0xaa ��, 0xa7 ��, 0xa9 ��, 0xa1 ��
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
���� ������� ���� �������� �ʱ�ȭ �մϴ�
@param	vnDolMatrix[][]		�� ������ ��� �迭
@param	cWhosTurn			���� ����
@prarm	cMyTurn				�� ���� ������ ��� ������ �ּ�
@prarm	cNewMyTurn			�� ���ӿ��� �̿��� ��(1 : �� / -1 : ��)
@prarm	chance				������ ��ȸ�� ��� ������ �ּ�
@prarm	stCursor			Ŀ�� ����ü�� �ּ�
@output ����
*/
void InitializeToReGame(int vnDolMatrix[15][15], char* cWhosTurn, char* cMyTurn, char cNewMyTurn, int* chance, struct cursor* stCursor)
{
	system("cls");
	InitializeDolMatrix(vnDolMatrix, 15);			// �� �迭 �ʱ�ȭ
	stCursor->vnCursorCoords[0] = 2 * 7;			// �ʱ� x��ǥ ����(�߾�����)
	stCursor->vnCursorCoords[1] = 7;				// �ʱ� y��ǥ ����(�߾�����)
	stCursor->pvnCursorOnDolMat = &vnDolMatrix[7][7]; // �ʱ� �� �迭�� Ŀ�� �������� ��ġ�� �߾����� ����
	stCursor->pHistory[0] = NULL;
	stCursor->pHistory[1] = NULL;
	stCursor->nHistory[0][0] = -1;
	stCursor->nHistory[0][1] = -1;
	stCursor->nHistory[1][0] = -1;
	stCursor->nHistory[1][1] = -1;
	*cWhosTurn = 1;									// ó�� ������ �浹
	*cMyTurn = cNewMyTurn;							// �� ���� ���� ������ ����
	*chance = 1;									// ������ ��ȸ �ʱ�ȭ(1��)
}
#pragma once

void cursor(int n); // Ŀ�� ���̱� & �����

/*
���� ����մϴ�
@param	vnPanMatrix[][]		�� ����� ��� �迭(���� ���� ����� ������ ���� �迭)
@param	vnDolMatrix[][]		�� ������ ��� �迭
@param	cWhosTurn			���� ����
@return	����
*/
void PrintPan(int vnPanMatrix[15][15], int vnDolMatrix[15][15], char cWhosTurn);

/*
 nSize X nSize �������� �� ��ġ �迭�� 0���� �ʱ�ȭ �մϴ�(Ŀ���� �ʱ� ��ġ�� 10 �߰�)
 @param		pvMatrix		������ �� ��ġ �迭�� �ּ�(������)
 @param		nSize			������ ������
 @output	����
 */
void InitializeDolMatrix(int* pvMatrix, int nSize);

/*
 nSize X nSize �������� ������ �迭�� �ʱ�ȭ �մϴ�
 @param		pvMatrix		������ ������ �迭�� �ּ�(������)
 @param		nSize			������ ������
 @output	����
 */
void InitializePanMatrix(int* pvnMatrix, int nSize);

void InitializeToReGame(int vnDolMatrix[15][15], char* cWhosTurn, char* cMyTurn, char cNewMyTurn, int* chance, struct cursor* stCursor);
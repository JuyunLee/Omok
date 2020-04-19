#pragma once

/*
�ܼ�â Ŀ���� x,y ��ǥ�� �ش��ϴ� ������ �̵��մϴ�
@param      x       x��ǥ
@param      y       y��ǥ
@output     ����
*/
void gotoxy(int x, int y);

/*
Ŀ���� ��ġ�� �����Դϴ�
@param		pvnCursorOnDolMat		�� �迭�� ����� Ŀ�� ������
@param		vnCursorCoords			Ŀ���� �ܼ�â �� ��ǥ
@param		nDirection				���� ���� ��ȣ(��, ��, ��, ��)
@param		cWhosTurn				���� ��������(1 : �浹 / -1 : �鵹)
@output		pvnCursorOnDolMat		�̵� �� Ŀ���� �ּҰ�
*/
int* MoveCursor(int* pvnDolMatrix, int* pvnCursorOnDolMat, int vnCursorCoords[], int nDirection, char cWhosTurn);

/*
Ŀ���� ��ġ�� ��ǥ�� ���� �����ϴ�
@param      pvnCursorOnDolMat	�� �迭�� ����� Ŀ�� ������
@param      vnCursorCoords[]	Ŀ���� �ܼ�â �� ��ǥ
@param      cWhosTurn			���� ��������(1 : �浹 / -1 : �鵹)
@output     ����
*/
void PutDol(int* pvnCursorOnDolMat, int* pvnCursorCoords, int cWhosTurn);

/*
Ű���� �Է��� �����մϴ�
*/
void IgnoreKeyboardInput();

/*
�����⸦ �ǽ��մϴ�
*/
void Murugi(struct cursor* stCursor);
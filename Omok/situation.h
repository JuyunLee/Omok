#pragma once
/*
���� �������� 5�� ������ ������ �������� �Ǵ��մϴ�
@param      pvnDolMatrix		�� �迭 �ּ�
@param      pvnCursorOnDolMat   �� �迭�� ����� Ŀ�� ������
@param      cWhosTurn           ����
@output     char                ���� ���� ����(1 : �浹 �¸� / 0 : ���� / -1 : �鵹 �¸� ---> cWhosTurn�� �״�� ����ϸ� ���� ����� ����)
*/
char JudgeGameEnded(int* pvnDolMatrix, int* pvnCursorOnDolMat, char cWhosTurn);

/*
���� ������ 3��Ȳ�� �Ǵ��� �Ǵ��մϴ�
@param      vnDolMatrix[][]     �� �迭
@param      pvnCursorOnDolMat   �� �迭�� ����� Ŀ�� ������
@param      cWhosTurn           ����
@param      cDirection          ����('L'eft / 'R'ight / 'U'p / 'D'own)
@output     char                3 ��Ȳ ����(1 : ���� / 0 : �ƴ�)
*/
char Judge3Situation(int vnDolMatrix[15][15], int* pvnCursorOnDolMat, char cWhosTurn, char cDirection);

/*
���� ������ 3-3��Ȳ�� �Ǵ��� �Ǵ��մϴ�
@param      vnDolMatrix         �� �迭
@param      pvnCursorOnDolMat   �� �迭�� ����� Ŀ�� ������
@param      cWhosTurn           ����
@output     char                3-3 ��Ȳ ����(1 : ���� / 0 : �ƴ�)
*/
char Judge33Situation(int vnDolMatrix[15][15], int* pvnCursorOnDolMat, char cWhosTurn);
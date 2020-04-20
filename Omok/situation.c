#include <stdio.h>

/*
���� �������� 5�� ������ ������ �������� �Ǵ��մϴ�
@param      pvnDolMatrix        �� �迭 �ּ�
@param      pvnCursorOnDolMat   �� �迭�� ����� Ŀ�� ������
@param      cWhosTurn           ����
@output     char                ���� ���� ����(1 : �浹 �¸� / 0 : ���� / -1 : �鵹 �¸� ---> cWhosTurn�� �״�� ����ϸ� ���� ����� ����)
*/
char JudgeGameEnded(int* pvnDolMatrix, int* pvnCursorOnDolMat, char cWhosTurn)
{
	// Ŀ�� ��ġ ���ϱ�
	int xc = (pvnCursorOnDolMat - pvnDolMatrix) % 15; // ��� ���� ���� x��ǥ; �׳� Ŀ�� ��ǥ�� ���ϴ� �κ��̴� �����ϼŵ� �˴ϴ� ((0,0)���κ��� n��°�� ��, 15�� ���� �������� x��ǥ)
	int yc = (pvnCursorOnDolMat - pvnDolMatrix) / 15; // ��� ���� ���� y��ǥ; �׳� Ŀ�� ��ǥ�� ���ϴ� �κ��̴� �����ϼŵ� �˴ϴ� ((0,0)���κ��� n��°�� ��, 15�� ���� ���� y��ǥ)
	int nInARow = 0; // �������� �� �� �ִ��� ������ ����
	int* pAddressNow; // �� ȸ ���� �˻��� ��ġ(�ּ�)�� ������ ������

	// �Ǵ� ����
	// �� ����
	for (int i = 0; i < 15; i++)
	{
		pAddressNow = (pvnDolMatrix + i) + (15 * yc); // x = 0 ~ 14, y = ����
					//		x						y
		if (*pAddressNow != 10 + cWhosTurn && *pAddressNow != cWhosTurn)
		{
			nInARow = 0;
		}
		else if ((nInARow += *pAddressNow) == cWhosTurn * 5 + 10)
		{
			if (i < 14 && *(pAddressNow + 1) == cWhosTurn)
			{
				nInARow = 0;
			}
			else
			{
				return cWhosTurn;
			}
		}
	}
	nInARow = 0;

	// �� ����
	for (int i = 0; i < 15; i++)
	{
		pAddressNow = (pvnDolMatrix + xc) + (15 * i); // x : ����, y = 0 ~ 14
					//				x						y
		if (*pAddressNow != 10 + cWhosTurn && *pAddressNow != cWhosTurn)
		{
			nInARow = 0;
		}
		else if ((nInARow += *pAddressNow) == cWhosTurn * 5 + 10)
		{
			if (i < 14 && *(pAddressNow + 15) == cWhosTurn)
			{
				nInARow = 0;
			}
			else
			{
				return 1;
			}
		}
	}
	nInARow = 0;

	// �� ����(xc : Ŀ���� x��, yc : Ŀ���� y��)
	// ������ : y - yc = x - xc
	// x����(y = 0) = xc - yc
	// y����(x = 0) = yc - xc
	// ��Ȳ�� �´� ������ �����ؼ� �ּҰ��� �Ź� 16�� ���ϸ� �� �� ������ �� �� ĭ �� �� -> ��� �������� �밢��
	int* pAddressStart;
	if ((xc - yc) >= 0) // x���� ��� �밢���� ��
	{
		pAddressStart = pvnDolMatrix + (xc - yc); // x����
	}
	else // y���� ��� �밢���� ��
	{
		pAddressStart = pvnDolMatrix + 15 * (yc - xc); // y����
	}
	for (int i = 0; i < (15 - abs(xc - yc)); i++) // ó������ ������ �����ϴ� ĭ�� ���� == 15 - (x������ ����) == 15 - abs(xc - yc)
	{
		pAddressNow = pAddressStart + 16 * i;
		if (*pAddressNow != 10 + cWhosTurn && *pAddressNow != cWhosTurn)
		{
			nInARow = 0;
		}
		else if ((nInARow += *pAddressNow) == cWhosTurn * 5 + 10)
		{
			if (i < (14 - abs(xc - yc)) && *(pAddressNow + 16) == cWhosTurn)
			{
				nInARow = 0;
			}
			else
			{
				return 1;
			}
		}
	}
	nInARow = 0;

	// �� ����(xc : Ŀ���� x��, yc : Ŀ���� y��)
	// ������ : x + y = xc + yc
	// x����(y = 0) = xc + yc
	// y����(x = 0) = xc + yc
	// �Ź� 14�� ���ϸ� �� �� ������ �� �� ĭ �� �� -> �·� �������� �밢��
	// x, y���� ��� �迭 ��ǥ ���� �ۿ� �ִ� ���, x�� �ִ񰪿��� x + y�� 14���� �ʰ��� ��ŭ�� y�� �����ָ� ���� ������ ��ǥ ����
	if ((xc + yc) <= 14)
	{
		pAddressStart = pvnDolMatrix + (xc + yc); // x����
	}
	else
	{
		pAddressStart = (pvnDolMatrix + 14) + 15 * (xc + yc - 14); // ������ �����Ŀ��� ���� �� ���� x��ǥ
	}
	for (int i = 0; i < 15 - abs(xc + yc - 14); i++)
	{
		pAddressNow = pAddressStart + 14 * i;
		if (*pAddressNow != 10 + cWhosTurn && *pAddressNow != cWhosTurn)
		{
			nInARow = 0;
		}
		else if ((nInARow += *pAddressNow) == cWhosTurn * 5 + 10)
		{
			if (i < (14 - abs(xc + yc - 14)) && *(pAddressNow + 14) == cWhosTurn)
			{
				nInARow = 0;
			}
			else
			{
				return 1;
			}
		}
	}

	return 0;
}
/*
���� ������ 3��Ȳ�� �Ǵ��� �Ǵ��մϴ�
@param      pvnDolMatrix        �� �迭 �ּ�
@param      pvnCursorOnDolMat   �� �迭�� ����� Ŀ�� ������
@param      cWhosTurn           ����
@param      cDirection          ����('L'eft / 'R'ight / 'U'p / 'D'own)
@output     char                3 ��Ȳ ����(1 : ���� / 0 : �ƴ�)
*/
char Judge3Situation(int vnDolMatrix[15][15], int* pvnCursorOnDolMat, char cWhosTurn, char cDirection)
{
	// char������ �� ������ �׳� �޸� �Ƴ�����...������
	char cX = (pvnCursorOnDolMat - vnDolMatrix) % 15;	// ���� ���� ������ x��ǥ ���
	char cY = (pvnCursorOnDolMat - vnDolMatrix) / 15;	// ���� ���� ������ y��ǥ ���

	char cXtoUse = cX, cYtoUse = cY;					// �˻縦 ������ ���� �ʱ�ȭ 720 - 144 = 576
	char cJumped = 0;									// ���� Ƚ��
	char cJumpedSum = 0;
	char cNumOfDols = 0;								// ���Ե� ���� ����
	char cSkipFlag = 0;									// �� ȸ�� �˻� ������ �� ���θ� ��� ����
	char cFlag = 0;

	switch (cDirection)
	{
	case 6:
		// R-1 �������� ã��(���� ��)
		while (1)
		{
			if (cXtoUse == 0)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] != 0) // � ���̴��� ������
				{
					return 0;
				}
				else
				{
					cJumped++;
					cJumpedSum++;
					break;
				}
			}
			else if (vnDolMatrix[cYtoUse][cXtoUse - 1] == cWhosTurn || vnDolMatrix[cYtoUse][cXtoUse - 1] == cWhosTurn + 10)
			{
				cXtoUse--;
				cJumped = 0;
			}
			else if (vnDolMatrix[cYtoUse][cXtoUse - 1] == 0 || vnDolMatrix[cYtoUse][cXtoUse - 1] == 10)
			{
				cXtoUse--;
				cJumped++;
				cJumpedSum++;
				if (cJumpedSum == 2)
				{
					break;
				}
			}
			else
			{
				if (cJumped == 0)
				{
					cSkipFlag = 1;
					break;
				}
				else
				{
					break;
				}
			}
		}
		cXtoUse += cJumped;
		cJumped = 0;
		cJumpedSum = 0;
		// R-2 �˻�
		if (cSkipFlag != 1)
		{
			for (int i = 0; i < 5; i++)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn || vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn + 10) // �� ��
				{
					cNumOfDols++;
					cJumped = 0;
				}
				else if (vnDolMatrix[cYtoUse][cXtoUse] == 0 || vnDolMatrix[cYtoUse][cXtoUse] == 10)
				{
					cJumped++;
					cJumpedSum++;
				}
				else
				{
					cNumOfDols = 0;
					break;
				}
				cXtoUse++;
			}
			if (cNumOfDols == 3)
			{
				if ((cJumpedSum == 2 && cJumped == 0)
					|| ((cJumpedSum == 2 && cJumped == 1) && (vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn || vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn + 10)))
				{
					return 0;
				}
				else
				{
					cFlag = 1;
				}
			}
		}
		cSkipFlag = 0;
		cNumOfDols = 0;
		cJumped = 0;
		cJumpedSum = 0;
		cXtoUse = cX;
		cYtoUse = cY;

		// L-1 �������� ã��(������ ��)
		while (1)
		{
			if (cXtoUse == 14)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] != 0) // � ���̴��� ������
				{
					return 0;
				}
				else
				{
					cJumped++;
					cJumpedSum++;
					break;
				}
			}
			else if (vnDolMatrix[cYtoUse][cXtoUse + 1] == cWhosTurn || vnDolMatrix[cYtoUse][cXtoUse + 1] == cWhosTurn + 10)
			{
				cXtoUse++;
				cJumped = 0;
			}
			else if (vnDolMatrix[cYtoUse][cXtoUse + 1] == 0 || vnDolMatrix[cYtoUse][cXtoUse + 1] == 10)
			{
				cXtoUse++;
				cJumped++;
				cJumpedSum++;
				if (cJumpedSum == 2)
				{
					break;
				}
			}
			else
			{
				if (cJumped == 0)
				{
					cSkipFlag = 1;
					break;
				}
				else
				{
					break;
				}
			}
		}
		cXtoUse -= cJumped;
		cJumped = 0;
		cJumpedSum = 0;

		// L-2 �˻�
		if (cSkipFlag != 1)
		{
			for (int i = 0; i < 5; i++)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn || vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn + 10) // �� ��
				{
					cNumOfDols++;
					cJumped = 0;
				}
				else if (vnDolMatrix[cYtoUse][cXtoUse] == 0 || vnDolMatrix[cYtoUse][cXtoUse] == 10)
				{
					cJumped++;
					cJumpedSum++;
				}
				else
				{
					cNumOfDols = 0;
					break;
				}
				cXtoUse--;
			}
			if (cNumOfDols == 3)
			{
				if ((cJumpedSum == 2 && cJumped == 0) || ((cJumpedSum == 2 && cJumped == 1) && (vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn || vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn + 10)))
				{
					return 0;
				}
				else
				{
					cFlag = 1;
				}
			}
		}
		cSkipFlag = 0;
		if (cFlag == 1)
			return 1;
		break;
	case 2:
		// U-1 �������� ã��(���� ��)
		while (1)
		{
			if (cXtoUse == 0)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] != 0) // � ���̴��� ������
				{
					return 0;
				}
				else
				{
					cJumped++;
					cJumpedSum++;
					break;
				}
			}
			else if (vnDolMatrix[cYtoUse - 1][cXtoUse] == cWhosTurn || vnDolMatrix[cYtoUse - 1][cXtoUse] == cWhosTurn + 10)
			{
				cYtoUse--;
				cJumped = 0;
			}
			else if (vnDolMatrix[cYtoUse - 1][cXtoUse] == 0 || vnDolMatrix[cYtoUse - 1][cXtoUse] == 10)
			{
				cYtoUse--;
				cJumped++;
				cJumpedSum++;
				if (cJumpedSum == 2)
				{
					break;
				}
			}
			else
			{
				if (cJumped == 0)
				{
					cSkipFlag = 1;
					break;
				}
				else
				{
					break;
				}
			}
		}
		cYtoUse += cJumped;
		cJumped = 0;
		cJumpedSum = 0;
		// U-2 �˻�
		if (cSkipFlag != 1)
		{
			for (int i = 0; i < 5; i++)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn || vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn + 10) // �� ��
				{
					cNumOfDols++;
					cJumped = 0;
				}
				else if (vnDolMatrix[cYtoUse][cXtoUse] == 0 || vnDolMatrix[cYtoUse][cXtoUse] == 10)
				{
					cJumped++;
					cJumpedSum++;
				}
				else
				{
					cNumOfDols = 0;
					break;
				}
				cYtoUse++;
			}
			if (cNumOfDols == 3)
			{
				if ((cJumpedSum == 2 && cJumped == 0) || ((cJumpedSum == 2 && cJumped == 1) && (vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn || vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn + 10)))
				{
					return 0;
				}
				else
				{
					cFlag = 1;
				}
			}
		}
		cSkipFlag = 0;
		cNumOfDols = 0;
		cJumped = 0;
		cJumpedSum = 0;
		cXtoUse = cX;
		cYtoUse = cY;

		// D-1 �������� ã��(�Ʒ��� ��)
		while (1)
		{
			if (cYtoUse == 14)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] != 0) // � ���̴��� ������
				{
					return 0;
				}
				else
				{
					cJumped++;
					cJumpedSum++;
					break;
				}
			}
			else if (vnDolMatrix[cYtoUse + 1][cXtoUse] == cWhosTurn || vnDolMatrix[cYtoUse + 1][cXtoUse] == cWhosTurn + 10)
			{
				cYtoUse++;
				cJumped = 0;
			}
			else if (vnDolMatrix[cYtoUse + 1][cXtoUse] == 0 || vnDolMatrix[cYtoUse + 1][cXtoUse] == 10)
			{
				cYtoUse++;
				cJumped++;
				cJumpedSum++;
				if (cJumpedSum == 2)
				{
					break;
				}
			}
			else
			{
				if (cJumped == 0)
				{
					cSkipFlag = 1;
					break;
				}
				else
				{
					break;
				}
			}
		}
		cXtoUse -= cJumped;
		cJumped = 0;
		cJumpedSum = 0;

		// D-2 �˻�
		if (cSkipFlag != 1)
		{
			for (int i = 0; i < 5; i++)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn || vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn + 10) // �� ��
				{
					cNumOfDols++;
					cJumped = 0;
				}
				else if (vnDolMatrix[cYtoUse][cXtoUse] == 0 || vnDolMatrix[cYtoUse][cXtoUse] == 10)
				{
					cJumped++;
					cJumpedSum++;
				}
				else
				{
					cNumOfDols = 0;
					break;
				}
				cYtoUse--;
			}
			if (cNumOfDols == 3)
			{
				if ((cJumpedSum == 2 && cJumped == 0) || ((cJumpedSum == 2 && cJumped == 1) && (vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn || vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn + 10)))
				{
					return 0;
				}
				else
				{
					cFlag = 1;
				}
			}
		}
		cSkipFlag = 0;
		if (cFlag == 1)
			return 1;
		break;
	case 3:
		// RD-1 �������� ã��(�������� ��)
		while (1)
		{
			if (cXtoUse == 0 || cYtoUse == 0)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] != 0) // � ���̴��� ������
				{
					return 0;
				}
				else
				{
					cJumped++;
					cJumpedSum++;
					break;
				}
			}
			else if (vnDolMatrix[cYtoUse - 1][cXtoUse - 1] == cWhosTurn || vnDolMatrix[cYtoUse - 1][cXtoUse - 1] == cWhosTurn + 10)
			{
				cXtoUse--;
				cYtoUse--;
				cJumped = 0;
			}
			else if (vnDolMatrix[cYtoUse - 1][cXtoUse - 1] == 0 || vnDolMatrix[cYtoUse - 1][cXtoUse - 1] == 10)
			{
				cXtoUse--;
				cYtoUse--;
				cJumped++;
				cJumpedSum++;
				if (cJumpedSum == 2)
				{
					break;
				}
			}
			else
			{
				if (cJumped == 0)
				{
					cSkipFlag = 1;
					break;
				}
				else
				{
					break;
				}
			}
		}
		cXtoUse += cJumped;
		cYtoUse += cJumped;
		cJumped = 0;
		cJumpedSum = 0;
		// RD-2 �˻�
		if (cSkipFlag != 1)
		{
			for (int i = 0; i < 5; i++)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn || vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn + 10) // �� ��
				{
					cNumOfDols++;
					cJumped = 0;
				}
				else if (vnDolMatrix[cYtoUse][cXtoUse] == 0 || vnDolMatrix[cYtoUse][cXtoUse] == 10)
				{
					cJumped++;
					cJumpedSum++;
				}
				else
				{
					cNumOfDols = 0;
					break;

				}
				cXtoUse++;
				cYtoUse++;
			}
			if (cNumOfDols == 3)
			{
				if ((cJumpedSum == 2 && cJumped == 0) || ((cJumpedSum == 2 && cJumped == 1) && (vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn || vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn + 10)))
				{
					return 0;
				}
				else
				{
					cFlag = 1;
				}
			}
		}
		cSkipFlag = 0;
		cNumOfDols = 0;
		cJumped = 0;
		cJumpedSum = 0;
		cXtoUse = cX;
		cYtoUse = cY;

		// LU-1 �������� ã��(�����ʾƷ��� ��)
		while (1)
		{
			if (cYtoUse == 14 || cXtoUse == 14)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] != 0) // � ���̴��� ������
				{
					return 0;
				}
				else
				{
					cJumped++;
					cJumpedSum++;
					break;
				}
			}
			else if (vnDolMatrix[cYtoUse + 1][cXtoUse + 1] == cWhosTurn || vnDolMatrix[cYtoUse + 1][cXtoUse + 1] == cWhosTurn + 10)
			{
				cXtoUse++;
				cYtoUse++;
				cJumped = 0;
			}
			else if (vnDolMatrix[cYtoUse + 1][cXtoUse + 1] == 0 || vnDolMatrix[cYtoUse + 1][cXtoUse + 1] == 10)
			{
				cXtoUse++;
				cYtoUse++;
				cJumped++;
				cJumpedSum++;
				if (cJumpedSum == 2)
				{
					break;
				}
			}
			else
			{
				if (cJumped == 0)
				{
					cSkipFlag = 1;
					break;
				}
				else
				{
					break;
				}
			}
		}
		cXtoUse -= cJumped;
		cYtoUse -= cJumped;
		cJumped = 0;
		cJumpedSum = 0;

		// LU-2 �˻�
		if (cSkipFlag != 1)
		{
			for (int i = 0; i < 5; i++)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn || vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn + 10) // �� ��
				{
					cNumOfDols++;
					cJumped = 0;
				}
				else if (vnDolMatrix[cYtoUse][cXtoUse] == 0 || vnDolMatrix[cYtoUse][cXtoUse] == 10)
				{
					cJumped++;
					cJumpedSum++;
				}
				else
				{
					cNumOfDols = 0;
					break;
				}
				cXtoUse--;
				cYtoUse--;
			}
			if (cNumOfDols == 3)
			{
				if ((cJumpedSum == 2 && cJumped == 0) || ((cJumpedSum == 2 && cJumped == 1) && (vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn || vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn + 10)))
				{
					return 0;
				}
				else
				{
					cFlag = 1;
				}
			}
		}
		cSkipFlag = 0;
		if (cFlag == 1)
			return 1;
		break;
	case 1:
		// LD-1 �������� ã��(���������� ��)
		while (1)
		{
			if (cXtoUse == 0 || cYtoUse == 0)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] != 0) // � ���̴��� ������
				{
					return 0;
				}
				else
				{
					cJumped++;
					cJumpedSum++;
					break;
				}
			}
			else if (vnDolMatrix[cYtoUse - 1][cXtoUse + 1] == cWhosTurn || vnDolMatrix[cYtoUse - 1][cXtoUse + 1] == cWhosTurn + 10)
			{
				cXtoUse++;
				cYtoUse--;
				cJumped = 0;
			}
			else if (vnDolMatrix[cYtoUse - 1][cXtoUse + 1] == 0 || vnDolMatrix[cYtoUse - 1][cXtoUse + 1] == 10)
			{
				cXtoUse++;
				cYtoUse--;
				cJumped++;
				cJumpedSum++;
				if (cJumpedSum == 2)
				{
					break;
				}
			}
			else
			{
				if (cJumped == 0)
				{
					cSkipFlag = 1;
					break;
				}
				else
				{
					break;
				}
			}
		}
		cXtoUse -= cJumped;
		cYtoUse += cJumped;
		cJumped = 0;
		cJumpedSum = 0;
		// LD-2 �˻�
		if (cSkipFlag != 1)
		{
			for (int i = 0; i < 5; i++)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn || vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn + 10) // �� ��
				{
					cNumOfDols++;
					cJumped = 0;
				}
				else if (vnDolMatrix[cYtoUse][cXtoUse] == 0 || vnDolMatrix[cYtoUse][cXtoUse] == 10)
				{
					cJumped++;
					cJumpedSum++;
				}
				else
				{
					cNumOfDols = 0;
					break;

				}
				cXtoUse--;
				cYtoUse++;
			}
			if (cNumOfDols == 3)
			{
				if ((cJumpedSum == 2 && cJumped == 0) || ((cJumpedSum == 2 && cJumped == 1) && (vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn || vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn + 10)))
				{
					return 0;
				}
				else
				{
					cFlag = 1;
				}
			}
		}
		cSkipFlag = 0;
		cNumOfDols = 0;
		cJumped = 0;
		cJumpedSum = 0;
		cXtoUse = cX;
		cYtoUse = cY;

		// RU-1 �������� ã��(���ʾƷ��� ��)
		while (1)
		{
			if (cYtoUse == 14 || cXtoUse == 14)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] != 0) // � ���̴��� ������
				{
					return 0;
				}
				else
				{
					cJumped++;
					cJumpedSum++;
					break;
				}
			}
			else if (vnDolMatrix[cYtoUse + 1][cXtoUse - 1] == cWhosTurn || vnDolMatrix[cYtoUse + 1][cXtoUse - 1] == cWhosTurn + 10)
			{
				cXtoUse--;
				cYtoUse++;
				cJumped = 0;
			}
			else if (vnDolMatrix[cYtoUse + 1][cXtoUse - 1] == 0 || vnDolMatrix[cYtoUse + 1][cXtoUse - 1] == 10)
			{
				cXtoUse--;
				cYtoUse++;
				cJumped++;
				cJumpedSum++;
				if (cJumpedSum == 2)
				{
					break;
				}
			}
			else
			{
				if (cJumped == 0)
				{
					cSkipFlag = 1;
					break;
				}
				else
				{
					break;
				}
			}
		}
		cXtoUse += cJumped;
		cYtoUse -= cJumped;
		cJumped = 0;
		cJumpedSum = 0;

		// RU-2 �˻�
		if (cSkipFlag != 1)
		{
			for (int i = 0; i < 5; i++)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn || vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn + 10) // �� ��
				{
					cNumOfDols++;
					cJumped = 0;
				}
				else if (vnDolMatrix[cYtoUse][cXtoUse] == 0 || vnDolMatrix[cYtoUse][cXtoUse] == 10)
				{
					cJumped++;
					cJumpedSum++;
				}
				else
				{
					cNumOfDols = 0;
					break;
				}
				cXtoUse++;
				cYtoUse--;
			}
			if (cNumOfDols == 3)
			{
				if ((cJumpedSum == 2 && cJumped == 0) || ((cJumpedSum == 2 && cJumped == 1) && (vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn || vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn + 10)))
				{
					return 0;
				}
				else
				{
					cFlag = 1;
				}
			}
		}
		cSkipFlag = 0;
		if (cFlag == 1)
			return 1;
		break;
	}
}

/*
���� ������ 3-3��Ȳ�� �Ǵ��� �Ǵ��մϴ�
@param      vnDolMatrix[][]        �� �迭 �ּ�
@param      pvnCursorOnDolMat   �� �迭�� ����� Ŀ�� ������
@param      cWhosTurn           ����
@output     char                3-3 ��Ȳ ����(1 : ���� / 0 : �ƴ�)
*/
char Judge33Situation(int vnDolMatrix[15][15], int* pvnCursorOnDolMat, char cWhosTurn)
{
	char cNumOf3 = 0;
	// �� ����
	if (Judge3Situation(vnDolMatrix, pvnCursorOnDolMat, cWhosTurn, 6) == 1)
		cNumOf3++;
	// �� ����
	if (Judge3Situation(vnDolMatrix, pvnCursorOnDolMat, cWhosTurn, 2) == 1)
		cNumOf3++;
	// �� ����
	if (Judge3Situation(vnDolMatrix, pvnCursorOnDolMat, cWhosTurn, 1) == 1)
		cNumOf3++;
	// �� ����
	if (Judge3Situation(vnDolMatrix, pvnCursorOnDolMat, cWhosTurn, 3) == 1)
		cNumOf3++;

	if (cNumOf3 >= 2)
		return 1;

	else
		return 0;
}
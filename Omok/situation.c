#include <stdio.h>

/*
돌이 연속으로 5개 놓여서 게임이 끝났는지 판단합니다
@param      pvnDolMatrix        돌 배열 주소
@param      pvnCursorOnDolMat   돌 배열과 연결된 커서 포인터
@param      cWhosTurn           차례
@output     char                게임 종료 여부(1 : 흑돌 승리 / 0 : 진행 / -1 : 백돌 승리 ---> cWhosTurn을 그대로 출력하면 같은 결과가 나옴)
*/
char JudgeGameEnded(int* pvnDolMatrix, int* pvnCursorOnDolMat, char cWhosTurn)
{
	// 커서 위치 구하기
	int xc = (pvnCursorOnDolMat - pvnDolMatrix) % 15; // 방금 놓은 돌의 x좌표; 그냥 커서 좌표값 구하는 부분이니 무시하셔도 됩니다 ((0,0)으로부터 n번째일 때, 15로 나눈 나머지는 x좌표)
	int yc = (pvnCursorOnDolMat - pvnDolMatrix) / 15; // 방금 놓은 돌의 y좌표; 그냥 커서 좌표값 구하는 부분이니 무시하셔도 됩니다 ((0,0)으로부터 n번째일 때, 15로 나눈 몫은 y좌표)
	int nInARow = 0; // 연속으로 몇 개 있는지 저장할 변수
	int* pAddressNow; // 매 회 마다 검사할 위치(주소)를 저장할 포인터

	// 판단 시작
	// → 방향
	for (int i = 0; i < 15; i++)
	{
		pAddressNow = (pvnDolMatrix + i) + (15 * yc); // x = 0 ~ 14, y = 고정
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

	// ↓ 방향
	for (int i = 0; i < 15; i++)
	{
		pAddressNow = (pvnDolMatrix + xc) + (15 * i); // x : 고정, y = 0 ~ 14
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

	// ↘ 방향(xc : 커서의 x값, yc : 커서의 y값)
	// 방정식 : y - yc = x - xc
	// x절편(y = 0) = xc - yc
	// y절편(x = 0) = yc - xc
	// 상황에 맞는 절편에서 시작해서 주소값에 매번 16씩 더하면 한 줄 내려간 후 한 칸 더 감 -> 우로 내려가는 대각선
	int* pAddressStart;
	if ((xc - yc) >= 0) // x절편에 닿는 대각선일 때
	{
		pAddressStart = pvnDolMatrix + (xc - yc); // x절편
	}
	else // y절편에 닿는 대각선일 때
	{
		pAddressStart = pvnDolMatrix + 15 * (yc - xc); // y절편
	}
	for (int i = 0; i < (15 - abs(xc - yc)); i++) // 처음부터 끝까지 존재하는 칸의 개수 == 15 - (x절편의 절댓값) == 15 - abs(xc - yc)
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

	// ↙ 방향(xc : 커서의 x값, yc : 커서의 y값)
	// 방정식 : x + y = xc + yc
	// x절편(y = 0) = xc + yc
	// y절편(x = 0) = xc + yc
	// 매번 14씩 더하면 한 줄 내려간 후 한 칸 덜 감 -> 좌로 내려가는 대각선
	// x, y절편 모두 배열 좌표 범위 밖에 있는 경우, x의 최댓값에서 x + y가 14보다 초과한 만큼을 y에 더해주면 가장 윗줄의 좌표 나옴
	if ((xc + yc) <= 14)
	{
		pAddressStart = pvnDolMatrix + (xc + yc); // x절편
	}
	else
	{
		pAddressStart = (pvnDolMatrix + 14) + 15 * (xc + yc - 14); // 직선의 방정식에서 가장 윗 줄의 x좌표
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
돌을 놓으면 3상황이 되는지 판단합니다
@param      pvnDolMatrix        돌 배열 주소
@param      pvnCursorOnDolMat   돌 배열과 연결된 커서 포인터
@param      cWhosTurn           차례
@param      cDirection          방향('L'eft / 'R'ight / 'U'p / 'D'own)
@output     char                3 상황 여부(1 : 맞음 / 0 : 아님)
*/
char Judge3Situation(int vnDolMatrix[15][15], int* pvnCursorOnDolMat, char cWhosTurn, char cDirection)
{
	// char형으로 한 이유는 그냥 메모리 아끼려고...ㅋㅋㅋ
	char cX = (pvnCursorOnDolMat - vnDolMatrix) % 15;	// 돌을 놓은 지점의 x좌표 계산
	char cY = (pvnCursorOnDolMat - vnDolMatrix) / 15;	// 돌을 놓은 지점의 y좌표 계산

	char cXtoUse = cX, cYtoUse = cY;					// 검사를 시작할 시점 초기화 720 - 144 = 576
	char cJumped = 0;									// 점프 횟수
	char cJumpedSum = 0;
	char cNumOfDols = 0;								// 포함된 돌의 개수
	char cSkipFlag = 0;									// 당 회차 검사 생략할 지 여부를 담는 변수
	char cFlag = 0;

	switch (cDirection)
	{
	case 6:
		// R-1 시작지점 찾기(왼쪽 끝)
		while (1)
		{
			if (cXtoUse == 0)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] != 0) // 어떤 돌이던지 있으면
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
		// R-2 검사
		if (cSkipFlag != 1)
		{
			for (int i = 0; i < 5; i++)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn || vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn + 10) // 내 돌
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

		// L-1 시작지점 찾기(오른쪽 끝)
		while (1)
		{
			if (cXtoUse == 14)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] != 0) // 어떤 돌이던지 있으면
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

		// L-2 검사
		if (cSkipFlag != 1)
		{
			for (int i = 0; i < 5; i++)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn || vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn + 10) // 내 돌
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
		// U-1 시작지점 찾기(위쪽 끝)
		while (1)
		{
			if (cXtoUse == 0)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] != 0) // 어떤 돌이던지 있으면
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
		// U-2 검사
		if (cSkipFlag != 1)
		{
			for (int i = 0; i < 5; i++)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn || vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn + 10) // 내 돌
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

		// D-1 시작지점 찾기(아래쪽 끝)
		while (1)
		{
			if (cYtoUse == 14)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] != 0) // 어떤 돌이던지 있으면
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

		// D-2 검사
		if (cSkipFlag != 1)
		{
			for (int i = 0; i < 5; i++)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn || vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn + 10) // 내 돌
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
		// RD-1 시작지점 찾기(왼쪽위쪽 끝)
		while (1)
		{
			if (cXtoUse == 0 || cYtoUse == 0)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] != 0) // 어떤 돌이던지 있으면
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
		// RD-2 검사
		if (cSkipFlag != 1)
		{
			for (int i = 0; i < 5; i++)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn || vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn + 10) // 내 돌
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

		// LU-1 시작지점 찾기(오른쪽아래쪽 끝)
		while (1)
		{
			if (cYtoUse == 14 || cXtoUse == 14)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] != 0) // 어떤 돌이던지 있으면
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

		// LU-2 검사
		if (cSkipFlag != 1)
		{
			for (int i = 0; i < 5; i++)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn || vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn + 10) // 내 돌
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
		// LD-1 시작지점 찾기(오른쪽위쪽 끝)
		while (1)
		{
			if (cXtoUse == 0 || cYtoUse == 0)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] != 0) // 어떤 돌이던지 있으면
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
		// LD-2 검사
		if (cSkipFlag != 1)
		{
			for (int i = 0; i < 5; i++)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn || vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn + 10) // 내 돌
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

		// RU-1 시작지점 찾기(왼쪽아래쪽 끝)
		while (1)
		{
			if (cYtoUse == 14 || cXtoUse == 14)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] != 0) // 어떤 돌이던지 있으면
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

		// RU-2 검사
		if (cSkipFlag != 1)
		{
			for (int i = 0; i < 5; i++)
			{
				if (vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn || vnDolMatrix[cYtoUse][cXtoUse] == cWhosTurn + 10) // 내 돌
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
돌을 놓으면 3-3상황이 되는지 판단합니다
@param      vnDolMatrix[][]        돌 배열 주소
@param      pvnCursorOnDolMat   돌 배열과 연결된 커서 포인터
@param      cWhosTurn           차례
@output     char                3-3 상황 여부(1 : 맞음 / 0 : 아님)
*/
char Judge33Situation(int vnDolMatrix[15][15], int* pvnCursorOnDolMat, char cWhosTurn)
{
	char cNumOf3 = 0;
	// → 방향
	if (Judge3Situation(vnDolMatrix, pvnCursorOnDolMat, cWhosTurn, 6) == 1)
		cNumOf3++;
	// ↓ 방향
	if (Judge3Situation(vnDolMatrix, pvnCursorOnDolMat, cWhosTurn, 2) == 1)
		cNumOf3++;
	// ↘ 방향
	if (Judge3Situation(vnDolMatrix, pvnCursorOnDolMat, cWhosTurn, 1) == 1)
		cNumOf3++;
	// ↙ 방향
	if (Judge3Situation(vnDolMatrix, pvnCursorOnDolMat, cWhosTurn, 3) == 1)
		cNumOf3++;

	if (cNumOf3 >= 2)
		return 1;

	else
		return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include "data.h"

maxroomnum = 1;
roomcount = 0;

ROOM* GetRoom()
{
	ROOM* tmp;
	tmp = (ROOM*)malloc(sizeof(ROOM));
	tmp->before = NULL;
	tmp->after = NULL;
	tmp->roomnum = maxroomnum++;
	roomcount++;

	return tmp;
}

int GetNthRoom(ROOM** head, int iteration)
{
	if (iteration == 0)
		return (*head)->roomnum;
	if ((*head)->after != NULL)
		GetNthRoom(&(*head)->after, --iteration);
	else
		return -1; // ������
}

void AddRoom(ROOM** head, SOCKADDR_IN ownerAddr, char* roomname)
{
	if (*head == NULL)
	{
		*head = GetRoom();
		(*head)->ownerAddr = ownerAddr;
		strcpy((*head)->roomname, roomname);
		return;
	}
	AddRoom(&(*head)->after, ownerAddr, roomname);
	(*head)->after->before = *head;
}

void DeleteRoom(ROOM** head) {
	ROOM* tmp = NULL;
	tmp = *head;

	if (*head == NULL)
	{
		printf("����� ����(room)\n");
		return;
	}

	if ((*head)->after == NULL) // ������
	{
		*head = (*head)->before;
		if (*head != NULL)
			(*head)->after = NULL;
	}
	else // ó�� & �߰�
	{
		*head = (*head)->after;
		(*head)->before = (*head)->before->before;
	}

	roomcount--;
	free(tmp);
}

//"192.168.0.30" ����
void GetOwnerAddr(ROOM** head, char* buf, int roomnum) {
	
	if (*head == NULL) {
		strcpy(buf, "ALREADY"); // �̹� �������� ��
		return;
	}
	if((*head)->roomnum == roomnum){
		strcpy(buf, inet_ntoa((*head)->ownerAddr.sin_addr));
		DeleteRoom(&(*head)); // ������������������������������������������������ ���䰡������
		return;
	} else if ((*head)->after != NULL){
		GetOwnerAddr(&(*head)->after, buf, roomnum);
	}
}

ROOM* SearchRoom(ROOM** head, int roomnum)
{
	if (*head == NULL)
		return NULL;
	if ((*head)->roomnum == roomnum)
		return *head;
	if ((*head)->after != NULL)
		SearchRoom(&(*head)->after, roomnum);
	else
		return NULL;
}

WORK* GetWork()
{
	WORK* tmp;
	tmp = (WORK*)malloc(sizeof(WORK));
	tmp->link = NULL;

	return tmp;
}

void AddWork(WORK** front, WORK** rear, int roomnum, char* msg, SOCKADDR_IN addr)
{
	WORK* tmp;
	tmp = GetWork();
	if (*rear == NULL)
	{
		*rear = tmp;
		(*rear)->roomnum = roomnum;
		strcpy((*rear)->msg, msg);
		(*rear)->addr = addr;
		*front = *rear;			
		//front�� ���� �������� ������Ʈ 
		//--> ��Ŀ �����忡�� front != NULL�� �� �������� ���� �ϹǷ� �ʹ��� ��ħ ������ �����ϱ� ����
		//--> ��Ŀ ������� ���� ������ ������ �Ӱ迵�� ������ �������� �����Ǿ����Ƿ� ���� mutex�� semaphore�� �� �ʿ䰡 ������
	}
	else
	{
		(*rear)->roomnum = roomnum;
		strcpy((*rear)->msg, msg);
		(*rear)->addr = addr;
		(*rear)->link = tmp;
		*rear = (*rear)->link;
	}
}

void DeleteWork(WORK** front, WORK** rear)
{
	if (*front == NULL)
	{
		printf("����� �����ϴ�.\n");
		return;
	}
	if (*rear == *front)
	{
		*rear = NULL;
	}
	WORK* tmp;
	tmp = *front;
	*front = (*front)->link;
	free(tmp);
}
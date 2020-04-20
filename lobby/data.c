#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include "data.h"

maxroomnum = 1;
roomcount = 0;

/*
방 노드(Double Linked List)를 생성
	@param		없음
	@output		새 노드의 주소
*/
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

/*
N번째 방을 가져옵니다
@param		head			ROOM Double Linked List의 head
@param		iteration		재귀 횟수(--> iteration번째)
*/
int GetNthRoom(ROOM** head, int iteration)
{
	if (iteration == 0)
		return (*head)->roomnum;
	if ((*head)->after != NULL)
		GetNthRoom(&(*head)->after, --iteration);
	else
		return -1;
}

/*
방을 추가합니다
@param		head		ROOM Double Linked List의 head의 주소
@param		ownerAddr	방 주인의 주소정보
@param		roomname	방 이름
*/
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

/*
방을 제거합니다
@param		head		제거할 방을 가리키는 head의 주소
*/
void DeleteRoom(ROOM** head) {
	ROOM* tmp = NULL;
	tmp = *head;

	if (*head == NULL)
	{
		printf("지울거 없음(room)\n");
		return;
	}

	if ((*head)->after == NULL) // 마지막
	{
		*head = (*head)->before;
		if (*head != NULL)
			(*head)->after = NULL;
	}
	else // 처음 & 중간
	{
		*head = (*head)->after;
		(*head)->before = (*head)->before->before;
	}

	roomcount--;
	free(tmp);
}

/*
방 주인의 ip주소를 가져와서 저장합니다
@param		head		ROOM Double Linked List의 head의 주소
@param		buf			방 주인의 ip 주소를 담을 배열(문자열 저장)
@param		roomnum		가져올 방의 번호
*/
void GetOwnerAddr(ROOM** head, char* buf, int roomnum) {
	
	if (*head == NULL) {
		strcpy(buf, "ALREADY"); // 이미 게임중인 방
		return;
	}
	if((*head)->roomnum == roomnum){
		strcpy(buf, inet_ntoa((*head)->ownerAddr.sin_addr));
		DeleteRoom(&(*head));
		return;
	} else if ((*head)->after != NULL){
		GetOwnerAddr(&(*head)->after, buf, roomnum);
	}
}

/*
방 번호에 해당하는 방의 주소를 찾습니다
@param		head		ROOM Double Linked List의 head의 주소
@param		roomnum		찾을 방 번호
@output		찾은 방 노드의 주소
*/
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

/*
Work(Queue)의 노드를 생성합니다
@output		생성한 노드의 주소
*/
WORK* GetWork()
{
	WORK* tmp;
	tmp = (WORK*)malloc(sizeof(WORK));
	tmp->link = NULL;

	return tmp;
}

/*
Work를 추가합니다
@param		front		Work Queue의 front
@param		rear		Work Queue의 rear
@param		roomnum		접근할 방의 번호
@param		msg			받은 메시지
@param		addr		메시지를 보낸 사람의 주소 정보
*/
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
		//front를 가장 마지막에 업데이트 
		//--> 워커 스레드에서 front != NULL일 때 가져가서 일을 하므로 초반의 겹침 문제를 방지하기 위함
		//--> 워커 스레드와 메인 스레드 사이의 임계영역 문제가 논리적으로 배제되었으므로 굳이 mutex나 semaphore를 쓸 필요가 없어짐
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

/*
Work를 삭제합니다
@param		front		Work Queue의 front
@param		rear		Work Queue의 rear
*/
void DeleteWork(WORK** front, WORK** rear)
{
	if (*front == NULL)
	{
		printf("지울거 없습니다.\n");
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
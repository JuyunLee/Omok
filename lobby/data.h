#pragma once
typedef struct doublelinkedlist_room {		// �� ��� ����ü
	int roomnum;
	char roomname[21];
	SOCKADDR_IN ownerAddr;
	struct doublelinkedlist_room* before, *after;
} ROOM;

typedef struct queue_work {		// ��Ŀ �����尡 �ؾ� �� ���� ����ü
	int roomnum;
	char msg[100];
	SOCKADDR_IN addr; // ���
	struct queue_work* link;
} WORK;

typedef struct message {	// Ŭ���̾�Ʈ�� �ְ���� �޽��� ����ü
	char msg[100];
	int roomnum;
} MESSAGE;

extern WORK* workfront;
extern WORK* workrear;
extern int maxroomnum;
extern int roomcount;

/*
�� ���(Double Linked List)�� ����
	@param		����
	@output		�� ����� �ּ�
*/
ROOM* GetRoom();

/*
N��° ���� �����ɴϴ�(���� ��Ī �� �̿�)
@param		head			ROOM Double Linked List�� head�� �ּ�
@param		iteration		��� Ƚ��(--> iteration��°)
*/
int GetNthRoom(ROOM** head, int iteration);

/*
���� �߰��մϴ�
@param		head		ROOM Double Linked List�� head�� �ּ�
@param		ownerAddr	�� ������ �ּ�����
@param		roomname	�� �̸�
*/
void AddRoom(ROOM** head, SOCKADDR_IN ownerAddr, char* roomname);

/*
���� �����մϴ�
@param		head		������ ���� ����Ű�� head�� �ּ�
*/
void DeleteRoom(ROOM** head);

/*
�� ������ ip�ּҸ� �����ͼ� �����մϴ�
@param		head		ROOM Double Linked List�� head�� �ּ�
@param		buf			�� ������ ip �ּҸ� ���� �迭(���ڿ� ����)
@param		roomnum		�� ������ �� ��ȣ
*/
void GetOwnerAddr(ROOM** head, char* buf, int roomnum);

/*
�� ��ȣ�� �ش��ϴ� ���� �ּҸ� ã���ϴ�
@param		head		ROOM Double Linked List�� head�� �ּ�
@param		roomnum		ã�� �� ��ȣ
@output		ã�� �� ����� �ּ�
*/
ROOM* SearchRoom(ROOM** head, int roomnum);

/*
Work(Queue)�� ��带 �����մϴ�
@output		������ ����� �ּ�
*/
WORK* GetWork();

/*
Work�� �߰��մϴ�
@param		front		Work Queue�� front
@param		rear		Work Queue�� rear
@param		roomnum		������ ���� ��ȣ
@param		msg			���� �޽���
@param		addr		�޽����� ���� ����� �ּ� ����
*/
void AddWork(WORK** front, WORK** rear, int roomnum, char* msg, SOCKADDR_IN addr);

/*
Work�� �����մϴ�
@param		front		Work Queue�� front
@param		rear		Work Queue�� rear
*/
void DeleteWork(WORK** front, WORK** rear);
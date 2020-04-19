#pragma once
typedef struct doublelinkedlist_room {
	int roomnum;
	char roomname[21];
	SOCKADDR_IN ownerAddr;
	struct doublelinkedlist_room* before, *after;
} ROOM;

typedef struct queue_work {
	int roomnum;
	char msg[100]; // 
	SOCKADDR_IN addr; // ´ë»ó
	struct queue_work* link;
} WORK;

typedef struct message {
	char msg[100];
	int roomnum;
} MESSAGE;

//#ifndef DONE
extern WORK* workfront;
extern WORK* workrear;
extern int maxroomnum;
extern int roomcount;
//#define DONE
//#endif

ROOM* GetRoom();

int GetNthRoom(ROOM** head, int iteration);

void AddRoom(ROOM** head, SOCKADDR_IN ownerAddr, char* roomname);

void DeleteRoom(ROOM** head);

void GetOwnerAddr(ROOM** head, char* buf, int roomnum);

ROOM* SearchRoom(ROOM** head, int roomnum);

WORK* GetWork();

void AddWork(WORK** front, WORK** rear, int roomnum, char* msg, SOCKADDR_IN addr);

void DeleteWork(WORK** front, WORK** rear);
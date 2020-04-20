#pragma once
typedef struct doublelinkedlist_room {		// 방 목록 구조체
	int roomnum;
	char roomname[21];
	SOCKADDR_IN ownerAddr;
	struct doublelinkedlist_room* before, *after;
} ROOM;

typedef struct queue_work {		// 워커 스레드가 해야 할 일의 구조체
	int roomnum;
	char msg[100];
	SOCKADDR_IN addr; // 대상
	struct queue_work* link;
} WORK;

typedef struct message {	// 클라이언트와 주고받을 메시지 구조체
	char msg[100];
	int roomnum;
} MESSAGE;

extern WORK* workfront;
extern WORK* workrear;
extern int maxroomnum;
extern int roomcount;

/*
방 노드(Double Linked List)를 생성
	@param		없음
	@output		새 노드의 주소
*/
ROOM* GetRoom();

/*
N번째 방을 가져옵니다(랜덤 매칭 시 이용)
@param		head			ROOM Double Linked List의 head의 주소
@param		iteration		재귀 횟수(--> iteration번째)
*/
int GetNthRoom(ROOM** head, int iteration);

/*
방을 추가합니다
@param		head		ROOM Double Linked List의 head의 주소
@param		ownerAddr	방 주인의 주소정보
@param		roomname	방 이름
*/
void AddRoom(ROOM** head, SOCKADDR_IN ownerAddr, char* roomname);

/*
방을 제거합니다
@param		head		제거할 방을 가리키는 head의 주소
*/
void DeleteRoom(ROOM** head);

/*
방 주인의 ip주소를 가져와서 저장합니다
@param		head		ROOM Double Linked List의 head의 주소
@param		buf			방 주인의 ip 주소를 담을 배열(문자열 저장)
@param		roomnum		방 주인의 방 번호
*/
void GetOwnerAddr(ROOM** head, char* buf, int roomnum);

/*
방 번호에 해당하는 방의 주소를 찾습니다
@param		head		ROOM Double Linked List의 head의 주소
@param		roomnum		찾을 방 번호
@output		찾은 방 노드의 주소
*/
ROOM* SearchRoom(ROOM** head, int roomnum);

/*
Work(Queue)의 노드를 생성합니다
@output		생성한 노드의 주소
*/
WORK* GetWork();

/*
Work를 추가합니다
@param		front		Work Queue의 front
@param		rear		Work Queue의 rear
@param		roomnum		접근할 방의 번호
@param		msg			받은 메시지
@param		addr		메시지를 보낸 사람의 주소 정보
*/
void AddWork(WORK** front, WORK** rear, int roomnum, char* msg, SOCKADDR_IN addr);

/*
Work를 삭제합니다
@param		front		Work Queue의 front
@param		rear		Work Queue의 rear
*/
void DeleteWork(WORK** front, WORK** rear);
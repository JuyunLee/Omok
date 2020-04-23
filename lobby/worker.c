#include "worker.h"
#define DEBUG

/*
워커 스레드의 메인 함수
*/
unsigned WINAPI DoWork(void* arg) { // thread main

	char* header;
	char* roomname = { 0 };
	ROOM* head = NULL;
	ROOM* tmp = NULL;
	SOCKET sock = *((SOCKET*)arg);
	MESSAGE buf;
	char* direction;
	while (1) {
		if (workfront != NULL) {
#ifdef DEBUG
			printf("working : %s\n", workfront->msg);
#endif
			header = strtok(workfront->msg, "~");
			if (!strcmp(header, "m")) { // 방 만들기
				roomname = strtok(NULL, "\0");
				AddRoom(&head, workfront->addr, roomname);
			} 
			else if (!strcmp(header, "e")) { // 입장
				GetOwnerAddr(&head, buf.msg, workfront->roomnum);
				if (!strcmp(buf.msg, "ALREADY"))
					if (head != NULL)
						buf.roomnum = head->roomnum;
					else
						buf.roomnum = NULL;
				sendto(sock, (char*)& buf, sizeof(buf), NULL, (SOCKADDR*) & (workfront->addr), sizeof(workfront->addr));
			}
			else if (!strcmp(header, "mv")) { // 방향키 상하(좌우는 클라이언트측에서 처리)
				direction = strtok(NULL, "\0");
				tmp = SearchRoom(&head, workfront->roomnum);
				if (!strcmp(direction, "up")) {
					if (tmp != NULL && tmp->before != NULL) {
						buf.roomnum = tmp->before->roomnum;
						tmp = tmp->before;
					}
				} 
				else if (!strcmp(direction, "down")) {
					if (tmp != NULL && tmp->after != NULL) {
						buf.roomnum = tmp->after->roomnum;
						tmp = tmp->after;
					}
				}

				if (tmp == NULL) {
					if (head != NULL)
						buf.roomnum = head->roomnum;
					tmp = head;
					memset(buf.msg, 0, sizeof(buf.msg));
					strcpy(buf.msg, "!@#$NULL$#@!^!@#$NULL$#@!^!@#$NULL$#@!");
				} 
				else {
					buf.roomnum = tmp->roomnum;
					memset(buf.msg, 0, sizeof(buf.msg));
					if (tmp->before == NULL) // 처음
						strcpy(buf.msg, "!@#$NULL$#@!^");
					else {
						strcpy(buf.msg, tmp->before->roomname);
						strcat(buf.msg, "^");
					}

					if (tmp == NULL) // 중간
						strcat(buf.msg, "!@#$NULL$#@!^");
					else {
						strcat(buf.msg, tmp->roomname);
						strcat(buf.msg, "^");
					}

					if (tmp->after == NULL) // 끝
						strcat(buf.msg, "!@#$NULL$#@!");
					else {
						strcat(buf.msg, tmp->after->roomname);
					}
				}
				sendto(sock, (char*)&buf, sizeof(buf), NULL, (SOCKADDR*)&(workfront->addr), sizeof(workfront->addr));
			}
			else if (!strcmp(header, "i")) { // 로비 입장 시 방 목록 제공
				memset(buf.msg, 0, sizeof(buf.msg));
				if (head == NULL) {
					strcpy(buf.msg, "!@#$NULL$#@!^!@#$NULL$#@!^!@#$NULL$#@!~");
					strcat(buf.msg, inet_ntoa(workfront->addr.sin_addr));
					buf.roomnum = 0;
					sendto(sock, (char*)& buf, sizeof(buf), NULL, (SOCKADDR*) & (workfront->addr), sizeof(workfront->addr));
				}
				else {
					if (head->before == NULL) // 처음
						strcpy(buf.msg, "!@#$NULL$#@!^");
					else {
						strcpy(buf.msg, head->before->roomname);
						strcat(buf.msg, "^");
					}

					if (head == NULL) // 중간
						strcat(buf.msg, "!@#$NULL$#@!^");
					else {
						strcat(buf.msg, head->roomname);
						strcat(buf.msg, "^");
					}

					if (head->after == NULL) // 끝
						strcat(buf.msg, "!@#$NULL$#@!");
					else {
						strcat(buf.msg, head->after->roomname);
					}
					buf.roomnum = head->roomnum;
					strcat(buf.msg, "~");
					strcat(buf.msg, inet_ntoa(workfront->addr.sin_addr));
					sendto(sock, (char*)& buf, sizeof(buf), NULL, (SOCKADDR*) & (workfront->addr), sizeof(workfront->addr));
				}
			}
			else if (!strcmp(header, "rd")) {
				if (roomcount == 0) {
					memset(buf.msg, 0, sizeof(buf.msg));
					strcpy(buf.msg, "ALREADY");
					buf.roomnum = NULL;
					sendto(sock, (char*)& buf, sizeof(buf), NULL, (SOCKADDR*)& (workfront->addr), sizeof(workfront->addr));
				}
				else {
					srand((unsigned)time(NULL));
					buf.roomnum = GetNthRoom(&head, rand() % roomcount);
					GetOwnerAddr(&head, buf.msg, buf.roomnum);
					if (!strcmp(buf.msg, "ALREADY"))
						if (head != NULL)
							buf.roomnum = head->roomnum;
						else
							buf.roomnum = NULL;
					sendto(sock, (char*)& buf, sizeof(buf), NULL, (SOCKADDR*) & (workfront->addr), sizeof(workfront->addr));
				}
			}
			else if (!strcmp(header, "gr")) { // 방 목록 달라고 하면
				tmp = SearchRoom(&head, workfront->roomnum);
				if (tmp == NULL) {
					if (head != NULL) {
						buf.roomnum = head->roomnum;
						if (head->before == NULL) // 처음
							strcpy(buf.msg, "!@#$NULL$#@!^");
						else {
							strcpy(buf.msg, head->before->roomname);
							strcat(buf.msg, "^");
						}

						if (head == NULL) // 중간
							strcat(buf.msg, "!@#$NULL$#@!^");
						else {
							strcat(buf.msg, head->roomname);
							strcat(buf.msg, "^");
						}

						if (head->after == NULL) // 끝
							strcat(buf.msg, "!@#$NULL$#@!");
						else {
							strcat(buf.msg, head->after->roomname);
						}
					}
					else {
						memset(buf.msg, 0, sizeof(buf.msg));
						strcpy(buf.msg, "!@#$NULL$#@!^!@#$NULL$#@!^!@#$NULL$#@!");
					}
				}
				else {
					buf.roomnum = tmp->roomnum;
					memset(buf.msg, 0, sizeof(buf.msg));
					if (tmp->before == NULL) // 처음
						strcpy(buf.msg, "!@#$NULL$#@!^");
					else {
						strcpy(buf.msg, tmp->before->roomname);
						strcat(buf.msg, "^");
					}

					if (tmp == NULL) // 중간
						strcat(buf.msg, "!@#$NULL$#@!^");
					else {
						strcat(buf.msg, tmp->roomname);
						strcat(buf.msg, "^");
					}

					if (tmp->after == NULL) // 끝
						strcat(buf.msg, "!@#$NULL$#@!");
					else {
						strcat(buf.msg, tmp->after->roomname);
					}
				}
				sendto(sock, (char*)&buf, sizeof(buf), NULL, (SOCKADDR*)&(workfront->addr), sizeof(workfront->addr));
			}
			memset(buf.msg, 0, sizeof(buf.msg));
			memset(&header, 0, sizeof(header));
			DeleteWork(&workfront, &workrear);
		}
	}
	_endthreadex(0);
}


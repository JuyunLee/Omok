#pragma once

/*
���� ������ ����մϴ�
*/
void PrintDescription();

/*
ȯ�� ��Ʈ�θ� ����մϴ�
*/
void SayHi();

/*
��Ʈ�θ� ����մϴ�
*/
void DrawIntro();

/*
���� ȭ���� ȣ���մϴ�
*/
int RunIntro();

/*
���� �� �޴��� ����մϴ�
*/
void DrawMenu();

/*
���� �� �޴��� ȣ���մϴ�
*/
int RunMenu(int chance, struct cursor* stCursor);

/*
�� ���� ȭ���� ����մϴ�
*/
void DrawSelectDolMenu();

/*
�� ���� â�� ȣ���մϴ�
*/
char SelectDol();

/*
������ �� �� ����ϴ�
*/
int Regame();
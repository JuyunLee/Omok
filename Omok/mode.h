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
	@output		������ ���(�޴�) ��ȣ (7 : ���ӽ��� / 9 : ���Ӽ��� / 11 : ��������)
*/
int RunIntro();

/*
���� �� �޴��� ����մϴ�
*/
void DrawMenu();

/*
���� �� �޴��� ȣ���մϴ�
	@output		������ ���(�޴�) ��ȣ (7 : ���ӽ��� / 9 : ���Ӽ��� / 11 : ������ / 13 : ���)
*/
int RunMenu(int chance, struct cursor* stCursor);

/*
������ �� �� ����ϴ�
	@output		������ ���� ��ȣ(1 : �ٽ� / 13 : ����)
*/
int Regame();
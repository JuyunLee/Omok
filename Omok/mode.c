#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "control.h"
#include "structs.h"
#include "init.h"

/*
���� ������ ����մϴ�
*/
void PrintDescription()
{
	printf("\n �浹���� ������ ���� ���� �Ӵϴ�.\n");
	printf(" ���� 5���� ����� ����� �¸�.\n\n");
	printf("\n Ű ����\n");
	printf("  ������ : Space\n  �޴� : esc\n");
	printf("  ä�� ���������� : \n      ������ ������� �� Enter\n\n\n");

	printf(" �ƹ�Ű�� �Է½� �ǵ��ư��ϴ�.");
	_getch();
}

/*
ȯ�� ��Ʈ�θ� ����մϴ�
*/
void SayHi()
{
	cursor(0);
	printf("\n\n\n\n\n\n");
	printf("             Hello");
	Sleep(1000);
	gotoxy(0, -1);
	printf("\n\n\n\n\n\n");
	printf("     reverse(Hello)");
	Sleep(1000);
	gotoxy(0, -1);
	printf("\n\n\n\n\n\n");
	printf("             olleH  ");
	Sleep(1000);
	gotoxy(0, -1);
	printf("\n\n\n\n\n\n");
	printf("M            olleH  ");
	Sleep(100);
	gotoxy(0, -1);
	printf("\n\n\n\n\n\n");
	printf("  M          olleH  ");
	Sleep(100);
	gotoxy(0, -1);
	printf("\n\n\n\n\n\n");
	printf("     M       olleH  ");
	Sleep(100);
	gotoxy(0, -1);
	printf("\n\n\n\n\n\n");
	printf("        M    olleH  ");
	Sleep(100);
	gotoxy(0, -1);
	printf("\n\n\n\n\n\n");
	printf("           M olleH  ");
	Sleep(100);
	gotoxy(0, -1);
	printf("\n\n\n\n\n\n");
	printf("           M-olleH  ");
	Sleep(100);
	gotoxy(0, -1);
	printf("\n\n\n\n\n\n");
	printf("           M-olleH  ");
	Sleep(1000);
}

/*
��Ʈ�θ� ����մϴ�
*/
void DrawIntro()
{
    int x = 11, y = 7;//�ʱⰪ
    int key;
    int end = 0;
    char* menus[3] = { "���ӽ���", "���Ӽ���", "��������" };
    system("mode con cols=35 lines=25");
    COORD bufferSize = { 35, 26 }; // ����, ����
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize);
    COORD bufferSize2 = { 35, 25 }; // ����, ����
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize2);
    cursor(0);
    gotoxy(10, 5);
    printf("M-olleH �� ��\n");
    gotoxy(x, y);//+2
    //0xa3 ��, 0xa4 ��, 0xa6 ��, 0xa5 ��, 0xab ��, 0xa8 ��, 0xaa ��, 0xa7 ��, 0xa9 ��, 0xa1 ��(0x2d)
    printf("%c%c---------%c%c", 0xa6, 0xa3, 0xa6, 0xa4);
    gotoxy(x, y + 1);//+2
    printf("��         ��");
    gotoxy(x, y + 2);//+2
    printf("%c%c---------%c%c", 0xa6, 0xa6, 0xa6, 0xa5);
    gotoxy(13, 8);
    printf("%s", menus[0]);
    gotoxy(13, 10);
    printf("%s", menus[1]);
    gotoxy(13, 12);
    printf("%s", menus[2]);
}

/*
���� ȭ���� ȣ���մϴ�
	@output		������ ���(�޴�) ��ȣ (7 : ���ӽ��� / 9 : ���Ӽ��� / 11 : ��������)
*/
int RunIntro() {
    int x = 11, y = 7;//�ʱⰪ
    int key;
    char* menus[3] = { "���ӽ���", "���Ӽ���", "��������" };
    DrawIntro();
	IgnoreKeyboardInput();
    while (1) 
    {
        key = _getch(); //Ű�Է�
        if (key == 0xe0) {//����Ű�ϰ��
            key = _getch();
            if (key >= 72) { //�����¿�
                switch (key)
                {
                case 72://��
                    if (y > 8) {
                        // �����
                        gotoxy(x, y);
                        printf("           ");
                        gotoxy(x, y + 1);//+2
                        printf("  %s  ", menus[(y - 6) / 2]);
                        gotoxy(x, y + 2);//+2
                        printf("           ");
                        y = y - 2;
                        // �׸���
                        gotoxy(x, y);//+2
                        printf("��---------��");
                        gotoxy(x, y + 1);//+2
                        printf("�� %s��", menus[(y - 6) / 2]);
                        gotoxy(x, y + 2);//+2
                        printf("��---------�� ");
                    }
                    break;
                case 80://��
                    if (y < 11) {
                        // �����
                        gotoxy(x, y);
                        printf("           ");
                        gotoxy(x, y + 1);//+2
                        printf("  %s  ", menus[(y - 6) / 2]);
                        gotoxy(x, y + 2);//+2
                        printf("           ");
                        y = y + 2;
                        // �׸���
                        gotoxy(x, y);//+2
                        printf("��---------��");
                        gotoxy(x, y + 1);//+2
                        printf("�� %s��", menus[(y - 6)/2]);
                        gotoxy(x, y + 2);//+2
                        printf("��---------�� ");
                    }
                    break;
                }
            }
        }
        else if (key == 32 || key == '\r') {//�����̽��� ���͸� ��������
            if (y == 7) {//���ӽ���
                system("cls"); //ȭ�� Ŭ����
                return 1;//��Ʈ�� ����
            }
            else if (y == 9) {//���Ӽ���
                system("cls"); //ȭ�� Ŭ����
				PrintDescription();
                x = 11, y = 7; // �ʱⰪ����
                DrawIntro();
            }
            else if (y == 11) {//��������
                gotoxy(1, 15);
                return 0;
            }
        }

    }
}

/*
���� �� �޴��� ����մϴ�
*/
void DrawMenu()
{
	int x = 11, y = 7;//�ʱⰪ
	int key;
	char* menus[4] = { "����ϱ�", "���Ӽ���", " ������ ", "����ϱ�" };
	system("mode con cols=35 lines=25");
	COORD bufferSize = { 35, 26 }; // ����, ����
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize);
	COORD bufferSize2 = { 35, 25 }; // ����, ����
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize2);
	cursor(0);
	gotoxy(10, 5);
	printf("M-olleH �� ��\n");
	gotoxy(x, y);//+2
	//0xa3 ��, 0xa4 ��, 0xa6 ��, 0xa5 ��, 0xab ��, 0xa8 ��, 0xaa ��, 0xa7 ��, 0xa9 ��, 0xa1 ��(0x2d)
	printf("%c%c---------%c%c", 0xa6, 0xa3, 0xa6, 0xa4);
	gotoxy(x, y + 1);//+2
	printf("��         ��");
	gotoxy(x, y + 2);//+2
	printf("%c%c---------%c%c", 0xa6, 0xa6, 0xa6, 0xa5);
	gotoxy(13, 8);
	printf("%s", menus[0]);
	gotoxy(13, 10);
	printf("%s", menus[1]);
	gotoxy(13, 12);
	printf("%s", menus[2]);
	gotoxy(13, 14);
	printf("%s", menus[3]);
}

/*
���� �� �޴��� ȣ���մϴ�
	@output		������ ���(�޴�) ��ȣ (7 : ���ӽ��� / 9 : ���Ӽ��� / 11 : ������ / 13 : ���)
*/
int RunMenu(int chance, struct cursor* stCursor) {
	int x = 11, y = 7;//�ʱⰪ
	int key;
	char* menus[4] = { "����ϱ�", "���Ӽ���", " ������ ", "����ϱ�" };
	cursor(0);
	DrawMenu();
	while (1)
	{
		key = _getch(); //Ű�Է�
		if (key == 0xe0) {//����Ű�ϰ��
			key = _getch();
			if (key >= 72) { //�����¿�
				switch (key)
				{
				case 72://��
					if (y > 8) {
						// �����
						gotoxy(x, y);
						printf("           ");
						gotoxy(x, y + 1);//+2
						printf("  %s  ", menus[(y - 6) / 2]);
						gotoxy(x, y + 2);//+2
						printf("           ");
						if ((chance == 0 && y == 13) || (stCursor->nHistory[1][0] == -1 && y == 13))
							y = y - 4;
						else
							y = y - 2;
						// �׸���
						gotoxy(x, y);//+2
						printf("��---------��");
						gotoxy(x, y + 1);//+2
						printf("�� %s��", menus[(y - 6) / 2]);
						gotoxy(x, y + 2);//+2
						printf("��---------�� ");
					}
					break;
				case 80://��
					if (y < 13) {
						// �����
						gotoxy(x, y);
						printf("           ");
						gotoxy(x, y + 1);//+2
						printf("  %s  ", menus[(y - 6) / 2]);
						gotoxy(x, y + 2);//+2
						printf("           ");
						if (y == 9 && (chance == 0 || stCursor->nHistory[1][0] == -1))
							y = y + 4;
						else
							y = y + 2;
						// �׸���
						gotoxy(x, y);//+2
						printf("��---------��");
						gotoxy(x, y + 1);//+2
						printf("�� %s��", menus[(y - 6) / 2]);
						gotoxy(x, y + 2);//+2
						printf("��---------�� ");
					}
					break;
				}
			}
		}
		else if (key == 32 || key == '\r') {//�����̽��� ���͸� ��������
			if (y == 7) {//���ӽ���
				system("cls"); //ȭ�� Ŭ����
				return 1;//��Ʈ�� ����
			}
			else if (y == 9) {//���Ӽ���
				system("cls"); //ȭ�� Ŭ����
				PrintDescription();
				x = 11, y = 7; // �ʱⰪ����
				DrawMenu();
			}
			else if (y == 11) { // ������
				return 2;
			}
			else if (y == 13) { //���
				gotoxy(1, 15);
				return 0;
			}
		}
	}
}

/*
������ �� �� ����ϴ�
	@output		������ ���� ��ȣ(1 : �ٽ� / 13 : ����)
*/
int Regame()
{
	int x = 1;
	int key;
	gotoxy(0, 18);
	printf("   ��---------��\n ");
	printf("   �� �ٽ��ϱ⦢   �����ϱ�\n ");
	printf("   ��---------�� ");
	while (1) {
		if ((key = _getch()) == 224) {
			switch (_getch()) {
			case 75: // ����
				x = 1;
				gotoxy(0, 18);
				printf("   ��---------��              \n ");
				printf("   �� �ٽ��ϱ⦢   �����ϱ�    \n ");
				printf("   ��---------��              ");
				break;

			case 77: // ������
				x = 13;
				gotoxy(0, 18);
				printf("               ��---------��\n ");
				printf("     �ٽ��ϱ�  �� �����ϱ⦢\n ");
				printf("               ��---------��    ");
				break;

			}
		}
		else if (key == 32 || key == '\r') { // �����̽� or ����
			gotoxy(0, 18);
			printf("                            \n ");
			printf("                            \n ");
			printf("                               ");
			return x;
		}
	}
}
#pragma once

/*
게임 설명을 출력합니다
*/
void PrintDescription();

/*
환영 인트로를 출력합니다
*/
void SayHi();

/*
인트로를 출력합니다
*/
void DrawIntro();

/*
시작 화면을 호출합니다
*/
int RunIntro();

/*
게임 중 메뉴를 출력합니다
*/
void DrawMenu();

/*
게임 중 메뉴를 호출합니다
*/
int RunMenu(int chance, struct cursor* stCursor);

/*
돌 선택 화면을 출력합니다
*/
void DrawSelectDolMenu();

/*
돌 선택 창을 호출합니다
*/
char SelectDol();

/*
재대결을 할 지 물어봅니다
*/
int Regame();
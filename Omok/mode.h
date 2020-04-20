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
	@output		선택한 기능(메뉴) 번호 (7 : 게임시작 / 9 : 게임설명 / 11 : 게임종료)
*/
int RunIntro();

/*
게임 중 메뉴를 출력합니다
*/
void DrawMenu();

/*
게임 중 메뉴를 호출합니다
	@output		선택한 기능(메뉴) 번호 (7 : 게임시작 / 9 : 게임설명 / 11 : 무르기 / 13 : 기권)
*/
int RunMenu(int chance, struct cursor* stCursor);

/*
재대결을 할 지 물어봅니다
	@output		선택한 응답 번호(1 : 다시 / 13 : 종료)
*/
int Regame();
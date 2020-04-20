#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <Windows.h>
#include <time.h>
#include "data.h"

/*
워커 스레드의 메인 함수
*/
unsigned WINAPI DoWork(void* arg); // thread main
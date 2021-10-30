#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define SIZE_BUFFER 100500
#define RESULT_FILE L"../x64/Debug/Result.csv"
#include <Windows.h>
#include <string.h>
#include <stdio.h>

struct User;
typedef struct User u;

struct User
{
	LPWSTR familia, imya, otchestvo;
	DWORD vozr;
};

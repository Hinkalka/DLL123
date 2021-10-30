#include "Header.h"

#define PATH L"DLL.dll"
#define DATA_PATH L"../Programma/Users1.csv"
#define SEARCH L"Иван"

typedef u* (_cdecl* ParseStr)(LPWSTR, LPDWORD);
typedef u* (_cdecl* ReadAndWrite)(LPWSTR, u*, DWORD);
typedef DWORD(_cdecl* SredniVozrast)(u*);


int WINAPI WinMain(HINSTANCE hlnstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    HINSTANCE OD;
    if (!(OD = LoadLibrary(PATH))) return 1;
    SredniVozrast SV;
    ParseStr Read;
    ReadAndWrite Write;
    Read = (ParseStr)GetProcAddress(OD, "ParseStr");
    Write = (ReadAndWrite)GetProcAddress(OD, "ReadAndWrite");
    SV = (SredniVozrast)GetProcAddress(OD, "SredniVozrast");

    DWORD n = 0;
    u* users = Read(DATA_PATH, &n);
    u* finded = Write(SEARCH, users, n);
    SV(finded);

    FreeLibrary(OD);
    return 0;
}

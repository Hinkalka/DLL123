#include "../Programma/Header.h"

BOOL WINAPI DllMain(HINSTANCE hlnstDll, DWORD dwReason, LPVOID IpReserved)
{
	BOOL bAllWentWell = TRUE;
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	if (bAllWentWell)
		return TRUE;
	else
		return FALSE;
}
LPWSTR RCSVF(LPWSTR path)
{
	HANDLE hFile = CreateFile(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD d = 0;
	DWORD rb = 100500;
	LPWSTR str = calloc(rb, sizeof(WCHAR));
	if (ReadFile(hFile, str, rb * sizeof(WCHAR), &d, NULL))
	{
		str[d / 2] = '\0';
		if (d == 0)
		{
			MessageBox(NULL, L"Ошибка", L"Файл пустой или его нет", MB_ICONERROR);
		}
	}
	else
	{
		MessageBox(NULL, L"Ошибка", L"Ошибка при чтении данных файла", MB_ICONERROR);
	}
	if (str != NULL)
	{
		CloseHandle(hFile);
		return str;
	}
	CloseHandle(hFile);
	return NULL;
}
DWORD LpwstrToDword(LPWSTR str)//перевод из строки в число
{
	DWORD dw = 0;
	for (size_t i = 0; i < wcslen(str); i++)//по всем символам строки
	{
		dw += (str[i] - '0');//получаем цифру
		dw *= 10;//увеличиваем разряд числа на 1
	}
	return dw / 10;//делим на 10, т.к. лишний раз увеличили разряд
}
__declspec(dllexport) DWORD SredniVozrast(u* users)
{
	DWORD summ = 0, avAge = 0;
	int n = 0;
	while (users[n].vozr != NULL)
	{
		summ += users[n].vozr;
		n++;
	}
	avAge = summ / n;
	HANDLE hFile = CreateFile(RESULT_FILE, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD d = 0;
	TCHAR bugg[100];
	swprintf(bugg, 100, TEXT("Средний возраст пользователей = %d\n"), avAge);
	SetFilePointer(hFile, 0, NULL, FILE_END);
	WriteFile(hFile, bugg, wcslen(bugg) * sizeof(TCHAR), d, NULL);
	CloseHandle(hFile);
	return avAge;
}
__declspec(dllexport) u* ParseStr(LPWSTR path, LPDWORD count)
{
	LPWSTR str = RCSVF(path);
	TCHAR* del = L"\n";
	TCHAR* del1 = L";";
	int n = 0, k = 0;
	for (int i = 0; i < wcslen(str); i++)
	{
		if (str[i] == del[0])
			n++;
	}
	*count = n;
	u* data = calloc(n, sizeof(u));
	if (n != 0)
	{
		LPWSTR istr = calloc(n + 1, sizeof(WCHAR));
		LPWSTR t = wcstok(str, del, istr);
		while (t != NULL && k < n)
		{
			LPWSTR strT = calloc(wcslen(t) + 1, sizeof(WCHAR));
			wcscpy(strT, t);
			LPWSTR jstr = calloc(wcslen(t) + 1, sizeof(WCHAR));
			LPWSTR tokenJ = wcstok(strT, del1, jstr);
			data[k].familia = calloc(wcslen(tokenJ) + 1, sizeof(WCHAR));
			wcscpy(data[k].familia, tokenJ);
			tokenJ = wcstok(NULL, del1, jstr);
			data[k].imya = calloc(wcslen(tokenJ) + 1, sizeof(WCHAR));
			wcscpy(data[k].imya, tokenJ);
			tokenJ = wcstok(NULL, del1, jstr);
			data[k].otchestvo = calloc(wcslen(tokenJ) + 1, sizeof(WCHAR));
			wcscpy(data[k].otchestvo, tokenJ);
			tokenJ = wcstok(NULL, L"\r", jstr);
			data[k].vozr = tokenJ != NULL ? LpwstrToDword(tokenJ) : 0;
			t = (LPWSTR)wcstok(NULL, del, istr);
			free(strT);
			free(jstr);
			k++;
		}
		free(t);
		free(istr);
		return data;
	}
	return NULL;
}
__declspec(dllexport) u* ReadAndWrite(LPWSTR search, u* datausers, DWORD k)
{
	int n = 0;
	u* s = calloc(k, sizeof(u));
	for (int i = 0; i < k; i++)
	{
		if (wcsstr(datausers[i].familia, search))
		{
			s[n].familia = calloc(wcslen(datausers[i].familia) + 1, sizeof(WCHAR));
			wcscpy(s[n].familia, datausers[i].familia);
			s[n].imya = calloc(wcslen(datausers[i].imya) + 1, sizeof(WCHAR));
			wcscpy(s[n].imya, datausers[i].imya);
			s[n].otchestvo = calloc(wcslen(datausers[i].otchestvo) + 1, sizeof(WCHAR));
			wcscpy(s[n].otchestvo, datausers[i].otchestvo);
			s[n].vozr = datausers[i].vozr;
			n++;
		}
	}
	HANDLE hFile = CreateFile(RESULT_FILE, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD d = 0;
	TCHAR b[SIZE_BUFFER];
	LPWSTR srt = calloc(100500, sizeof(WCHAR));
	swprintf(b, SIZE_BUFFER, TEXT("%s;%s;%s;%d\r\n"), s[0].familia, s[0].imya, s[0].otchestvo, s[0].vozr);
	wcscpy(srt, b);
	for (int i = 1; i < n; i++)
	{
		swprintf(b, SIZE_BUFFER, TEXT("%s;%s;%s;%d\r\n"), s[i].familia, s[i].imya, s[i].otchestvo, s[i].vozr);
		wcscat(srt, b);
	}
	if (srt != NULL)
		WriteFile(hFile, srt, wcslen(srt) * sizeof(TCHAR), d, NULL);
	CloseHandle(hFile);
	return s;
}


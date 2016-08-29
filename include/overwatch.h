#ifndef _MEMZ_OVERWATCH_H
#define _MEMZ_OVERWATCH_H

#define _WIN32_WINNT 0x0501

#include <Windows.h>

DWORD WINAPI overwatch_thread(LPVOID parameter);
LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif // _MEMZ_OVERWATCH_H
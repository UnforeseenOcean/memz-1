#ifndef _MEMZ_WINDOW_H
#define _MEMZ_WINDOW_H

#define _WIN32_WINNT 0x0501

#include <Windows.h>

typedef struct
{
    HWND hwnd;
    HDC hdc;
    RECT rect;
} window_t;

#endif // _MEMZ_WINDOW_H
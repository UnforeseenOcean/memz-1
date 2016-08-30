#ifndef _MEMZ_WINDOW_H
#define _MEMZ_WINDOW_H

#include <memz/def.h>
#include <Windows.h>

typedef struct
{
    HWND hwnd;
    HDC hdc;
    RECT rect;
} window_t;

#endif // _MEMZ_WINDOW_H
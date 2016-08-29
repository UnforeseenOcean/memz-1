#ifndef _MEMZ_UTIL_H
#define _MEMZ_UTIL_H

#define _WIN32_WINNT 0x0501

#include <Windows.h>

int random();
void reverse_string_w(LPWSTR str);
LRESULT CALLBACK message_box_hook(int nCode, WPARAM wParam, LPARAM lParam);

#endif // _MEMZ_UTIL_H
#ifndef _MEMZ_UTIL_H
#define _MEMZ_UTIL_H

#include <memz/def.h>
#include <Windows.h>

int random();
void reverse_string_w(LPWSTR str);
LRESULT CALLBACK message_box_hook(int nCode, WPARAM wParam, LPARAM lParam);

#endif // _MEMZ_UTIL_H
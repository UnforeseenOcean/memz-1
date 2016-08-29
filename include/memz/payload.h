#ifndef _MEMZ_PAYLOAD_H
#define _MEMZ_PAYLOAD_H

typedef struct
{
    void* function;
    int start_delay;
    const int cicles;
    int delay;
    int min_delay;
    const int accel;
} payload_t;

void payload_host(void* pl);

#endif // _MEMZ_PAYLOAD_H
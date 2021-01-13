#include "screen.h"
#include "../cpu/ports.h"
#include "../libc/mem.h"
#include "../libc/printf.h"

static int screen_x;
static int screen_y;
static uint8_t *video_mem = (uint8_t *) VIDEO_ADDRESS;

/** wrapper implementation of _putchar for printf library */
void _putchar(char character) {
	char _p[2] = {character, 0};
	kprint(&_p[0]);
}

/* Declaration of private functions */
int get_cursor_offset();
void set_cursor_offset(int offset);
int print_char(char c, int col, int row, char attr);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);

/**********************************************************
 * Public Kernel API functions                            *
 **********************************************************/

void enter(void) {
	set_screen_pos(0, screen_y+1);
}

void backspace(void) {
	if (screen_x == 0)
		set_screen_pos(TERM_MAX_COLS-1, screen_y-1);
	else
		set_screen_pos(screen_x-1, screen_y);
	*(uint8_t *)(video_mem + ((TERM_MAX_COLS*screen_y + screen_x) << 1)) = ' ';
}

void scroll_up(void) {
	int32_t x, y, old_line, new_line;

	/* move each row in the video memory up by 1 */
	for (y = 0; y < TERM_MAX_ROWS-1; y++) {
		for (x = 0; x < TERM_MAX_COLS; x++) {
			old_line = TERM_MAX_COLS*(y+1) + x;
			new_line = TERM_MAX_COLS*y + x;
			*(uint8_t *)(video_mem + (new_line << 1)) = *(uint8_t *)(video_mem + (old_line << 1));
		}
	}

	/* clear bottom line */
	for (x = 0; x < TERM_MAX_COLS; x++) {
		new_line = TERM_MAX_COLS*(TERM_MAX_ROWS-1) + x;
		*(uint8_t *)(video_mem + (new_line << 1)) = ' ';
	}
	set_screen_pos(0, screen_y);
}

void set_screen_pos(int x, int y) {
	if (x < TERM_MAX_COLS)
		screen_x = x;
	else {
		enter();
		set_cursor_pos();
		return;
	}

	if (y < TERM_MAX_ROWS)
		screen_y = y;
	else {
		scroll_up();
		screen_y = TERM_MAX_ROWS - 1;
	}
	set_cursor_pos();
}

void set_cursor_pos() {
	set_cursor_offset(get_offset(screen_x, screen_y));
}

/**
 * Print a message on the specified location
 * If col, row, are negative, we will use the current offset
 */
void kprint_at(char *message, int col, int row) {
	/* Set cursor if col/row are negative */
	int offset;
	if (col >= 0 && row >= 0)
		offset = get_offset(col, row);
	else {
		offset = get_cursor_offset();
		row = get_offset_row(offset);
		col = get_offset_col(offset);
	}

	/* Loop through message and print it */
	int i = 0;
	while (message[i] != 0) {
		offset = print_char(message[i++], col, row, WHITE_ON_BLACK);
		/* Compute row/col for next iteration */
		row = get_offset_row(offset);
		col = get_offset_col(offset);
	}
}

void kprint(char *message) {
	kprint_at(message, -1, -1);
}

void kprint_backspace() {
	int offset = get_cursor_offset()-2;
	int row = get_offset_row(offset);
	int col = get_offset_col(offset);
	print_char(0x08, col, row, WHITE_ON_BLACK);
}


/**********************************************************
 * Private kernel functions                               *
 **********************************************************/


/**
 * Innermost print function for our kernel, directly accesses the video memory 
 *
 * If 'col' and 'row' are negative, we will print at current cursor location
 * If 'attr' is zero it will use 'white on black' as default
 * Returns the offset of the next character
 * Sets the video cursor to the returned offset
 */
int print_char(char c, int col, int row, char attr) {
	uint8_t *vidmem = (uint8_t*) VIDEO_ADDRESS;
	if (!attr) attr = WHITE_ON_BLACK;

	/* Error control: print a red 'E' if the coords aren't right */
	if (col >= TERM_MAX_COLS || row >= TERM_MAX_ROWS) {
		vidmem[2*(TERM_MAX_COLS)*(TERM_MAX_ROWS)-2] = 'E';
		vidmem[2*(TERM_MAX_COLS)*(TERM_MAX_ROWS)-1] = RED_ON_WHITE;
		return get_offset(col, row);
	}

	int offset;
	if (col >= 0 && row >= 0) offset = get_offset(col, row);
	else offset = get_cursor_offset();

	if (c == '\n') {
		row = get_offset_row(offset);
		offset = get_offset(0, row+1);
	} else if (c == 0x08) { /* Backspace */
		vidmem[offset] = ' ';
		vidmem[offset+1] = attr;
	} else {
		vidmem[offset] = c;
		vidmem[offset+1] = attr;
		offset += 2;
	}

	/* Check if the offset is over screen size and scroll */
	if (offset >= TERM_MAX_ROWS * TERM_MAX_COLS * 2) {
		int i;
		for (i = 1; i < TERM_MAX_ROWS; i++)
			memcpy((void *)(get_offset(0, i - 1) + VIDEO_ADDRESS),
			       (const void *)(get_offset(0, i) + VIDEO_ADDRESS), TERM_MAX_COLS * 2);

		/* Blank last line */
		char *last_line = (char *) (get_offset(0, TERM_MAX_ROWS - 1) + (uint8_t *) VIDEO_ADDRESS);
		for (i = 0; i < TERM_MAX_COLS * 2; i++) last_line[i] = 0;

		offset -= 2 * TERM_MAX_COLS;
	}

	set_cursor_offset(offset);
	return offset;
}

int get_cursor_offset() {
	/* Use the VGA ports to get the current cursor position
	 * 1. Ask for high byte of the cursor offset (data 14)
	 * 2. Ask for low byte (data 15)
	 */
	port_byte_out(REG_SCREEN_CTRL, 14);
	int offset = port_byte_in(REG_SCREEN_DATA) << 8; /* High byte: << 8 */
	port_byte_out(REG_SCREEN_CTRL, 15);
	offset += port_byte_in(REG_SCREEN_DATA);
	return offset * 2; /* Position * size of character cell */
}

void set_cursor_offset(int offset) {
	/* Similar to get_cursor_offset, but instead of reading we write data */
	offset /= 2;
	port_byte_out(REG_SCREEN_CTRL, 14);
	port_byte_out(REG_SCREEN_DATA, (uint8_t)(offset >> 8));
	port_byte_out(REG_SCREEN_CTRL, 15);
	port_byte_out(REG_SCREEN_DATA, (uint8_t)(offset & 0xff));
}

void clear() {
	int screen_size = TERM_MAX_COLS * TERM_MAX_ROWS;
	int i;
	uint8_t *screen = (uint8_t*) VIDEO_ADDRESS;

	for (i = 0; i < screen_size; i++) {
		screen[i*2] = ' ';
		screen[i*2+1] = WHITE_ON_BLACK;
	}
	set_cursor_offset(get_offset(0, 0));
}


int get_offset(int col, int row) { return 2 * (row * TERM_MAX_COLS + col); }
int get_offset_row(int offset) { return offset / (2 * TERM_MAX_COLS); }
int get_offset_col(int offset) { return (offset - (get_offset_row(offset)*2*TERM_MAX_COLS))/2; }
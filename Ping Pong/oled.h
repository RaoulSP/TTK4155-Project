#include <avr/io.h>

#ifndef OLED_H_
#define OLED_H_

int timer_intr;

//---TIMER SETUP FOR 60Hz refresh---


void oled_init();

void oled_print_string(char * string, uint8_t column, uint8_t line, uint8_t font_size, int invert);
void oled_print_char(char character, uint8_t font_size, int invert);

void oled_goto_line(int line);
void oled_goto_column(int column);
void oled_clear_line(int line);
void oled_draw_pixel(int x,int y);
int oled_read_pixel(int x, int y);
void oled_clear_pixel(int x, int y);
void oled_invert_pixel(int x, int y);

void oled_write_command(char command);
void oled_write_data(char data);
char oled_read_data();

void oled_clear_screen();
void oled_refresh();

void oled_draw_line(int x1, int y1, int x2, int y2);
void oled_invert_rectangle(int x1, int y1, int x2, int y2);

#endif /* OLED_H_ */
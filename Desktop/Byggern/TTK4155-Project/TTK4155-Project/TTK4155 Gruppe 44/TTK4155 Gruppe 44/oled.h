#include <avr/io.h>

#ifndef OLED_H_
#define OLED_H_

//oled write
//oled init
//oled configure?

void oled_init();

void oled_print_string(char * string, uint8_t column, uint8_t line, uint8_t fontSize);
void oled_print_char(char character, uint8_t column, uint8_t line, uint8_t fontSize);

void oled_reset();
void oled_home();
void oled_goto_line(int line);
void oled_goto_column(int column);
void oled_clear_line(int line);
void oled_pos(int row,int column);
void oled_print(int row, int column, char character, int font); //char* ex

void oled_write_command(char command);
void oled_write_data(char data);

void oled_clear_sram();

#endif /* OLED_H_ */
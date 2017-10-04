#include "oled.h"
#include "fonts.h"

void oled_init(){
	oled_write_command(0xae); //display off
	oled_write_command(0xa1); //segment remap
	oled_write_command(0xda); //common pads hardware: alternative
	oled_write_command(0x12);
	oled_write_command(0xc8); //common output scan direction:com63~com0
	oled_write_command(0xa8); //multiplex ration mode:63
	oled_write_command(0x3f);
	oled_write_command(0xd5); //display divide ratio/osc. freq. mode
	oled_write_command(0x80);
	oled_write_command(0x81); //contrast control
	oled_write_command(0x50);
	oled_write_command(0xd9); //set pre-charge period
	oled_write_command(0x21);
	oled_write_command(0x20); //Set Memory Addressing Mode
	oled_write_command(0x02);
	oled_write_command(0xdb); //VCOM deselect level mode
	oled_write_command(0x30);
	oled_write_command(0xad); //master configuration
	oled_write_command(0x00);
	oled_write_command(0xa4); //out follows RAM content
	oled_write_command(0xa6); //set normal display
	oled_write_command(0xaf); // display on
	oled_clear_sram();
}
void oled_print_string(char * string, uint8_t column, uint8_t line, uint8_t fontSize){
	int length = strlen(string);
	for (int i = 0; i < length; i++){
		oled_print_char(string[i], column, line, fontSize);
		column = column + fontSize;
		if (column > (127 - fontSize)){ //if no space for another char
			column = 0;
			line++;
		}
	}
}

void oled_print_char(char character, uint8_t column, uint8_t line, uint8_t fontSize){ //TODO: gjør at kun print_string brukes av oss, print_char skal ikke ta inn column og line
	oled_goto_column(column); //remove? Only in print string
	oled_goto_line(line); //remove?
	if (fontSize == 8){
		for (int i = 0; i < 8; i++){
			oled_write_data(pgm_read_byte(&font8[character-' '][i]));
		}
	}
	else if (fontSize == 5){
		for (int i = 0; i < 5; i++){
			oled_write_data(pgm_read_byte(&font5[character-' '][i]));
		}
	}
	else if (fontSize == 4){
		for (int i = 0; i < 4; i++){
			oled_write_data(pgm_read_byte(&font4[character-' '][i]));
		}
	}
}

void oled_write_command(char command){
	volatile char *oled_command = (char *) 0x1000;
	printf("%d \n",command);
	*oled_command = command;
}

void oled_write_data(char data){
	volatile char *oled_data = (char *) 0x1200;
	*oled_data = data;
}

void oled_clear_sram(){
	for (uint8_t line = 0; line < 8; line++){
		oled_clear_line(line);
	}
}

//void oled_reset();
//void oled_home();

void oled_goto_line(int line){
	oled_write_command(0xB0 + line);
}
void oled_goto_column(int column){ //0 - 127
	uint8_t lower = column & 0b00001111;
	uint8_t upper = column >> 4;
	oled_write_command(lower);
	oled_write_command(0b00010000 | upper);
}

void oled_clear_line(int line){
	oled_goto_line(line);
	oled_goto_column(0);
	for (uint8_t i = 0; i < 128; i++){
		oled_write_data(0x00);
	}
}

void oled_pos(int row,int column);
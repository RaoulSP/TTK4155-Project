#include "oled.h"
#include "fonts.h"
#include "../lib/interrupts.h"
#include <avr/io.h>

int doublebuffering = 1;
int oled_back_buffer_index = 0;

volatile uint8_t *oled_command = 0x1000;
volatile uint8_t *oled_back_buffer = 0x1800;
volatile uint8_t *oled_front_buffer = 0x1200;

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
	oled_clear_screen();	  //Sets all buffer data to 0
	
	//Set up timer, enable timer/counter compare match interrupt for 60 FPS
	TCCR2 = (1 << CS22) | (1 << CS21) | (1 << CS20) | (1 << WGM21);  //clock source to be used by the Timer/Counter clkI/O/1024 -  CTC mode
	TIMSK = (1 << OCIE2);					
	int OCRA_num = (long)F_CPU/(refresh_rate*1024);
	OCR2 = OCRA_num; //Sets the value for the compare match to 80
}

void oled_print_string(char * string, uint8_t column, uint8_t line, uint8_t font_size, int invert){
	oled_goto_column(column);
	oled_goto_line(line);
	
	int length = strlen(string);
	for (int i = 0; i < length; i++){
		oled_print_char(string[i], font_size, invert);
		column = column + font_size;
		if (column > (127 - font_size)){ //if no space for another char
			column = 0;
			line++;
			oled_goto_column(column);
			oled_goto_line(line);
		}
	}
}

void oled_print_char(char character, uint8_t font_size, int invert){
	int font_address;
	if (font_size == 8){
		font_address = &font8[character-' '];
	}
	else if (font_size == 5){
		font_address = &font5[character-' '];
	}
	else if (font_size == 4){
		font_address = &font4[character-' '];
	}
	if(!invert){
		for (int i = 0; i < font_size; i++){
			oled_write_data(pgm_read_byte(font_address + i));
		}	
	}
	else{
		for (int i = 0; i < font_size; i++){
			oled_write_data(~pgm_read_byte(font_address + i));
		}
	}
}

void oled_clear_screen(){
	for (uint8_t line = 0; line < 8; line++){
		oled_clear_line(line);
	}
}

void oled_clear_line(int line){
	oled_goto_line(line);
	oled_goto_column(0);
	for (uint8_t i = 0; i < 128; i++){
		oled_write_data(0x00);
	}
}

void oled_write_command(char command){
	*oled_command = command;
}

void oled_write_data(char data){
	if(doublebuffering){
		if (oled_back_buffer_index > 1023){
			return -1;
		}
		oled_back_buffer[oled_back_buffer_index] = data;
		oled_back_buffer_index++;	
	}
	else{
		*oled_front_buffer = data;
	}	
}

char oled_read_data(){
	if(doublebuffering){
		if (oled_back_buffer_index > 1023){
			return 0x00;
		}
		return oled_back_buffer[oled_back_buffer_index];
	}
	else{
		return 0x00; //Alternatively NULL
	}
}

void oled_goto_line(int line){
	if(doublebuffering){
		oled_back_buffer_index = oled_back_buffer_index % 128 + line*128;	
	}
	else{
		oled_write_command(0xB0 + line);
	}
}

void oled_goto_column(int column){ 
	if(doublebuffering){
		oled_back_buffer_index = oled_back_buffer_index - oled_back_buffer_index % 128 + column;
	}
	else{
		uint8_t lower = 0 & 0b00001111;
		uint8_t upper = 0 >> 4;
		oled_write_command(lower);
		oled_write_command(0b00010000 | upper);
	}
}

void oled_refresh(){
	uint8_t lower = 0 & 0b00001111; // set column to 0
	uint8_t upper = 0 >> 4;
	oled_write_command(lower);
	oled_write_command(0b00010000 | upper);
	
	if(doublebuffering){
		for(int page = 0; page < 8; page++){
			oled_write_command(0xB0 + page);
			for(int column = 0; column < 128; column++){
				*oled_front_buffer = oled_back_buffer[column + page*128];
			}
		}	
	}
}

void oled_draw_pixel(int x, int y){
	oled_goto_column(x);
	oled_goto_line(y/8);
	oled_write_data((1 << (y % 8)) | oled_read_data());
}

int oled_read_pixel(int x, int y){
	oled_goto_column(x);
	oled_goto_line(y/8);
	return !!((1 << (y % 8)) & oled_read_data());
}

void oled_clear_pixel(int x, int y){
	oled_goto_column(x);
	oled_goto_line(y/8);
	oled_write_data(~(1 << (y % 8)) & oled_read_data());
}

void oled_invert_pixel(int x, int y){
	oled_goto_column(x);
	oled_goto_line(y/8);
	oled_write_data((1 << (y % 8)) ^ oled_read_data()); //exclusive or
}

void oled_draw_line(int x1, int y1, int x2, int y2){
	if ((y2-y1)<=(x2-x1)){
		float m = (float)(y2-y1)/(float)(x2-x1);
		for (int x = 0; x < (x2-x1); x++){
			int y = (int)(m*x+0.5);
			oled_draw_pixel(x1+x,y1+y);
		}
	}
	else{
		float m = (float)(x2-x1)/(float)(y2-y1);
		for (int y = 0; y < (y2-y1); y++){
			int x = (int)(m*y+0.5);
			oled_draw_pixel(x1+x,y1+y);
		}
	}
	
	//TODO: make it draw correctly in every direction
}

void oled_invert_rectangle(int x1, int y1, int x2, int y2){
	if(y1 > y2){
		int temp = y1;
		y1 = y2;
		y2 = temp;
	}
	
	if(x1 > x2){
		int temp = x1;
		x1 = y2;
		x2 = temp;
	}
	int t = 255 << y1%8;		//Top line to be filled
	int b = 255 >> 8 - y2%8;	//Bottom line to be filled
	if(y1/8 != y2/8){			//Over several lines
		//Filling top
		oled_goto_line(y1/8);
		for(int x = x1; x < x2; x++){
			oled_goto_column(x);
			oled_write_data(t ^ oled_read_data());
		}
		//Filling lines inbetween
		for (int l = y1/8 + 1; l < y2/8; l++){
			oled_goto_line(l);
			for (int x = x1; x < x2; x++){
				oled_goto_line(x);
				oled_write_data(255 ^ oled_read_data());
			}
		}
		//Filling bottom
		oled_goto_line(y2/8);
		for(int x = x1; x < x2; x++){
			oled_goto_column(x);
			oled_write_data(b ^ oled_read_data());
		}
	}
	else{
		int m = t & b;
		oled_goto_line(y1/8);
		for(int x = x1; x < x2; x++){
			oled_goto_column(x);
			oled_write_data(m ^ oled_read_data());
		}
	}
}

ISR(TIMER2_COMP_vect)
{
	if(state == in_menu){
		flags.oled_refresh_timeout = 1;	
	}
	else if(state == in_game){
		flags.game_time_passed = 1;
		flags.game_interrupt == 1;
	}
	
}
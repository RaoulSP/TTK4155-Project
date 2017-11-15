
#include <util/delay.h>

#include "joy.h"
#include "adc.h"
#include "settings.h"

int x;
int y;
int z = 0;

int xmin = 0;
int xmean = 127;
int xmax = 255;

int ymin = 0;
int ymean = 127;
int ymax = 255;

void joy_init(){
	clear_bit(DDRB,PB2);	//Set joystick button pin to input
	set_bit(PORTB,PB2);		//Set the internal pull-up resistor needed for the button
	//joy_calibrate();
}

void joy_calibrate(){
	xmean = adc_read('x');
	ymean = adc_read('y');
	xmax = 0;
	xmin = 255;
	ymax = 0;
	ymin = 255;
	
	printf("Calibrating...\r\n");
	oled_print_string("Please calibrate joystick!",0,0,8,0);
	oled_refresh();
	
	while (z == 0){
		x = adc_read('x');
		y = adc_read('y');
		z = !test_bit(PINB, PB2);
		
		if (x > xmax){
			xmax = x;
			printf("%d \r\n", xmax);
		}
		if (y > ymax){
			ymax = y;
			printf("%d \r\n", ymax);
		}
		if (x < xmin){
			xmin = x;
			printf("%d \r\n", xmin);
		}
		if (y < ymin){
			ymin = y;
			printf("%d \r\n", ymin);
		}
	}
	printf("Calibrated.\r\n");
	oled_clear_screen();
	oled_print_string("Calibrated!",0,0,8,0);
	oled_refresh();
}

Position joy_get_position(){
		x = adc_read('x');
		y = adc_read('y');
		z = !test_bit(PINB, PB2);
		
		if (x > xmean){
			x = ((x - xmean) * 100) / (xmax - xmean);
		}
		else{
			x = ((x - xmean) * 100) / (xmean - xmin);
		}
		if (y > ymean){
			y = ((y - ymean) * 100) / (ymax - ymean);
		}
		else{
			y = ((y - ymean)  * 100) / (ymean - ymin);
		}
		
		Position pos;
		pos.x = x;
		pos.y = y;
		pos.z = z;
		
		if (x < 4 && x > -4)
		{
			pos.x = 0;
		}
		if (y < 4 && y > -4)
		{
			pos.y = 0;
		}
		return pos;
}

Position_polar joy_get_position_polar(){
	Position pos_cart = joy_get_position();
	Position_polar pos_pol;
	pos_pol.angle = atan2(pos_cart.y,pos_cart.x) * (180.0/ M_PI);
	pos_pol.amplitude = sqrt((uint16_t)(pos_cart.y*pos_cart.y) + (uint16_t)(pos_cart.x*pos_cart.x));
	return pos_pol;
}

Direction joy_get_direction(){ 
	Position_polar pos_pol  = joy_get_position_polar();
	
	if(pos_pol.amplitude > 80){
		if (pos_pol.angle > 135){
			return LEFT;
		}
		else if(pos_pol.angle > 45){
			return UP;
		}
		else if(pos_pol.angle > -45){
			return RIGHT;
		}
		else if(pos_pol.angle > -135){
			return DOWN;
		}
		else{
			return LEFT;
		}
	}
	return NEUTRAL;
} 

void joy_print(){
	/*
	if(all){
		printf("x = ");
		printf("%d  ", adc_read('x'));
		
		printf("y = ");
		printf("%d  ", adc_read('y'));
	}
	*/
	
	Position pos = joy_get_position();
	Position_polar pos_pol = joy_get_position_polar();
	
	printf("X = ");
	printf("x = %d, y = %d, X= %d, Y = %d, Z = %d, amplitude = %d, angle = %d, direction = %d\r\n", adc_read('x'), adc_read('y'), pos.x, pos.y, pos.z, pos_pol.amplitude, pos_pol.angle, joy_get_direction());
	
	/*
	printf("Y = ");
	printf("%d  ", pos.y);
	
	printf("A = ");
	printf("%d  ", pos_pol.amplitude);
	
	printf("theta = ");
	printf("%d  ", pos_pol.angle);
	
	printf("Direction = ");
	printf("%d  ", joy_get_direction());
	
	printf("Middle button= ");
	printf("%d  \r", pos.z);
	*/
	
}
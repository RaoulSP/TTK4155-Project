#include "joy.h"
#include <avr/io.h>
#include "adc.h"
#include "utils.h"
#define F_CPU 4915200
#include <util/delay.h>

int x;
int y;
int xmean;
int ymean;
int xmax = 0;
int xmin = 255;
int ymax = 0;
int ymin = 255;

void JOY_init(){
	clear_bit(DDRB,PB2); //Set joystick button pin to input
	set_bit(PORTB,PB2);
	printf("Calibrating...\n");
	JOY_calibrate();
	printf("Calibrated.");
}

void JOY_calibrate(){
	xmean = ADC_read('x');
	ymean = ADC_read('y');
	printf("%d", test_bit(PINB, PB2));
	while (test_bit(PINB, PB2))
	{
		int x = ADC_read('x');
		int y = ADC_read('y');
		if (x > xmax){
			xmax = x;
			printf("%d \n", xmax);
		}
		if (y > ymax){
			ymax = y;
			printf("%d \n", ymax);
		}
		if (x < xmin){
			xmin = x;
			printf("%d \n", xmin);
		}
		if (y < ymin){
			ymin = y;
			printf("%d \n", ymin);
		}
	}

}

void JOY_button(int button){
	
}

Position JOY_getPosition(){
	x = (ADC_read('x'));
	y = (ADC_read('y'));
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
	Position pos = {x,y};
	return pos;
}
/*
Direction JOY_getDirection(Direction ex){ 
	return ex;
} 
*/
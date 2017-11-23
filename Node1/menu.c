#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include "../lib/joy.h"
#include "../lib/interrupts.h"
#include "menu.h"
#include "oled.h"

//Flags
int joy_moved = 0;
int animation_stepup = 0;
int animation_stepdown = 0;
int transition = 0;
Direction transition_dir = NEUTRAL;
int transition_count = 0;
int transition_step = 1;
int joy_held = 0;

//----------INITIALIZATION--------------
//The name of the menus and their functions
Menu main_menu = {
	.name = "MAIN MENU",
	.num_of_entries = 0,
	.cursor = 0,
	.super_menu = NULL,
	.sub_menus = NULL,
	.draw = draw_menu,
	.offset = 1
};

Menu difficulty = {
	.name = "Difficulty",
	.draw = draw_menu,
	.action = action_start_transition_right,
	.offset = 1
};

Menu options = {
	.name = "Options",
	.draw = draw_menu,
	.num_of_entries = 0,
	.action = action_start_transition_right,
	.offset = 1
};

Menu highscore = {
	.name = "High score",
	.draw = draw_list,
	.action = action_start_transition_right
};

Menu control = {
	.name = "Control",
	.draw = draw_menu,
	.action = action_start_transition_right,
	.offset = 1
};

Menu contro = {
	.name = "Graphics",
	.draw = draw_menu,
	.action = action_start_transition_right
};

Menu toggle = {
	.name = "Hans",
	.draw = draw_menu,
	.action = action_toggle_name
};

Menu play_game= {
	.name = "Play game",
	.draw = draw_menu,
	.action = action_run_game
};


Menu* current_menu = &main_menu;

//The initializer - How the menus are related
void menu_init(){
	add_sub_menu(&main_menu, &play_game);
	
	char* control_entr[3] = {"player1", "player2", "player3"};
	//add_list_entries(&control, control_entr, 3);
	add_sub_menu(&options, &control);
	
	//char* contro_entr[4] = {"low", "medium", "high","ultra"};
	//add_list_entries(&contro, contro_entr, 4);
	add_sub_menu(&options, &contro);
	
	add_sub_menu(&main_menu, &options);
	add_sub_menu(&main_menu, &toggle);
	
	char* diff_entr[3] = {"easy"};
	//add_list_entries(&main_menu, diff_entr, 1);
	//add_sub_menu(&main_menu, &difficulty);
	
	char* high_entr[4] = {"1. herman","2. raoul","3. alle andre","-inf^inf. hans"};
	//add_list_entries(&highscore, high_entr,4);
	add_sub_menu(&main_menu, &highscore);
	
}
//----------NO NEED TO CHANGE THINGS BEYOND THIS POINT----------

//Functions for adding entries and nodes
void add_list_entries(Menu* menu, char* entries[], int num_of_new_entries){

	//Creates an empty menu to be added
	for(int i = 0; i < num_of_new_entries; i++){
		Menu* empty;
		empty = malloc(sizeof(*empty));
		empty->name = entries[i];
		empty->num_of_entries = 0;
		add_sub_menu(menu, empty);
	}
}

void add_sub_menu(Menu* super, Menu* sub){
	super->num_of_entries += 1;		//Num of entries in super are updated
	
	//Pointers to and from the menus are set correctly
	sub->super_menu = super;
	super->sub_menus = realloc(super->sub_menus, super->num_of_entries * sizeof(Menu*));
	super->sub_menus[super->num_of_entries - 1] = malloc(sizeof(Menu));
	super->sub_menus[super->num_of_entries - 1] = sub;
}

//Drawing functions (change to update?)
Menu* draw_menu(Menu* self){
	//Print the menu
	
	oled_print_string(self->name,10,0,8,0);
	for(int i = 0; i < self->num_of_entries; i++){
		oled_print_string(self->sub_menus[i]->name,0,i + self->offset,5,0);
	}
	if(transition == 0){
		oled_invert_rectangle(0,8*(self->cursor + self->offset),64,8*((self->cursor + self->offset) + 1));
	}
	//Where to go next
	Direction dir = joy_get_direction();
	Position pos = joy_get_position();
	//THIS DOES NOT WORK IF THE JOYSTICK IS NOT WORKING
	if(transition == 0){
		switch(dir){
			case UP:
			transition = 1;
			transition_dir = UP;
			break;
			case DOWN:
			transition = 1;
			transition_dir = DOWN;
			break;
			case LEFT:
			if (self->super_menu != NULL){
				self = self->super_menu;
				joy_held = 1;
				transition = 1;
			}
			break;
			case RIGHT:
			self = action_start_transition_right(self);
			break;
			case NEUTRAL:
			if(pos.z  == 1 && self->sub_menus[self->cursor]->action != NULL && joy_held == 0){
				self = self->sub_menus[self->cursor]->action(self);
				joy_held = 1;
				
			}
			else if(pos.z == 0){
				joy_held = 0;
			}
			break;
		}
	}
	else{
		if (transition_dir == UP || transition_dir == DOWN){
			menu_transition(self, transition_dir);
		}
		else if(dir == NEUTRAL){
			joy_held = 0;
			transition = 0;
		}
	}
	return self;
}
Menu* draw_list(Menu* self){
	for(int i = 0; i < self->num_of_entries; i++){
		oled_print_string(self->sub_menus[i]->name,0,i,5,0);
	}
	//Where to go next
	Direction dir = joy_get_direction();
	if((dir == LEFT) & (self->super_menu != NULL)){
		self = self->super_menu;
	}
}

//Action functions
Menu* action_run_game(Menu* self){
	state = in_game;
}

Menu* action_start_transition_right(Menu* self){
	if (self->sub_menus[self->cursor]->num_of_entries != 0){
		self = self->sub_menus[self->cursor];
		joy_held = 1;
		transition = 1;
	}
	return self;
}
Menu* action_toggle_name(Menu* self){	//TEST FOR CHECK BOXES IN OPTIONS
	self->sub_menus[self->cursor]->name= "Herman";
	return self;
}
//void action_animation(){}

void menu_run_display(){
	oled_clear_screen();
	current_menu = current_menu->draw(current_menu);
	oled_refresh();
}
void menu_transition(Menu *self, int dir){
	//If done with transition
	if (transition_count == 8){
		//wrapping
		if(transition_dir == UP){
			self->cursor--;
			if(self->cursor == -1){
				self->cursor = self->num_of_entries - 1;
			}
		}
		else if(transition_dir == DOWN){
			//wrapping
			if(self->cursor == self->num_of_entries - 1){
				self->cursor = 0;
			}
			else{
				self->cursor++;
			}
		}
		transition_count = 0;
		transition_dir = NEUTRAL;
		transition = 0;
		oled_invert_rectangle(self->offset,8*(self->cursor + self->offset),64,8*((self->cursor + self->offset) + 1));//The final box
	}
	//Update the transition
	else if(transition == 1 && (transition_dir == UP || transition_dir == DOWN)){
		int sign = 1;
		if(transition_dir == UP){
			sign = -1;
		}
		int x1 = 0;
		int x2 = 64;
		int y1 = 8*(self->cursor + self->offset) + sign*transition_count;
		int y2 = 8*(self->cursor + 1 + self->offset) + sign*transition_count;
		
		if(y1 < 8*self->offset){
			oled_invert_rectangle(x1, 8*self->offset,x2,y2);
			oled_invert_rectangle(x1,(self->num_of_entries)*8+y1,x2,(self->num_of_entries + self->offset)*8);
		}
		else if(y2 > (self->num_of_entries + self->offset)*8){
			oled_invert_rectangle(x1,y1,x2,(self->num_of_entries + self->offset)*8);
			oled_invert_rectangle(x1,8*self->offset,x2,y2 - (self->num_of_entries)*8);
		}
		else{
			oled_invert_rectangle(x1,y1,x2,y2);
		}
		transition_count++;
	}
}


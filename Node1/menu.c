#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include "../lib/joy.h"
#include "../lib/interrupt_flags.h"
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
	.name = "Main menu",
	.num_of_entries = 0,
	.cursor = 0,
	.super_menu = NULL,
	.sub_menus = NULL,
	//.type = MENU,
	.draw = draw_menu
};

Menu difficulty = {
	.name = "Difficulty",
	//.type = MENU,
	.draw = draw_menu,
	.action = action_start_transition_right
};

Menu options = {
	.name = "options",
	//.type = MENU,
	.draw = draw_menu,
	.action = action_start_transition_right
};

Menu highscore = {
	.name = "High score",
	//.type = LIST,
	.draw = draw_list,
	.action = action_start_transition_right
};

Menu control = {
	.name = "Control",
	//.type = MENU,
	.draw = draw_menu,
	.action = action_start_transition_right
};

Menu contro = {
	.name = "Graphics",
	//.type = MENU,
	.draw = draw_menu,
	.action = action_start_transition_right
};

Menu toggle = {
	.name = "Hans",
	//.type = MENU,
	.draw = draw_menu,
	.action = action_toggle_name
};

Menu play_game= {
	.name = "Play game",
	//.type = MENU,
	.draw = draw_menu,
	.action = action_run_game
};


Menu* current_menu = &main_menu;

//The initializer - How the menus are related
void menu_init(){
	add_sub_menu(&main_menu, &play_game);
	
	char* control_entr[3] = {"Player1", "Player2", "Player3"};
	add_list_entries(&control, control_entr, 3);
	add_sub_menu(&options, &control);
	
	char* contro_entr[4] = {"Low", "Medium", "High","Ultra"};
	add_list_entries(&contro, contro_entr, 4);
	add_sub_menu(&options, &contro);
	
	add_sub_menu(&main_menu, &options);
	add_sub_menu(&main_menu, &toggle);
	
	char* diff_entr[3] = {"Easy", "Medium", "Hard"};
	add_list_entries(&difficulty, diff_entr, 3);
	add_sub_menu(&main_menu, &difficulty);
	
	char* high_entr[4] = {"1. Herman","2. Raoul","3. Alle andre","-inf^inf. Hans"};
	add_list_entries(&highscore, high_entr,4);
	add_sub_menu(&main_menu, &highscore);
	
}
//----------NO NEED TO CHANGE THINGS BEYOND THIS POINT----------

//Functions for adding entries and nodes
void add_list_entries(Menu* menu, char* entries[], int num_of_new_entries){
	
	menu->entries = malloc(num_of_new_entries * sizeof(char*));	//Correct size of double pointer
	
	for(int i = 0; i < num_of_new_entries; i++){
		menu->entries[i] = malloc(strlen(entries[i]) * sizeof(char)); //Allocate space for each string
		menu->entries[i] =  entries[i];								  //Adds each string
		
		//---NEW---
		//Menu* empty;
		//empty = malloc(sizeof(Menu));
		////empty->name = malloc(strlen(entries[i]) * sizeof(char));
		//empty->name = entries[i];
		//add_sub_menu(menu, empty);
		//menu->num_of_entries+=1;
	}
	menu->num_of_entries = num_of_new_entries;
	menu->cursor = 0; //To make sure the cursor is not something else
}

void add_sub_menu(Menu* super, Menu* sub){
	super->num_of_entries += 1;		//Num of entries in super are updated
	
	//Pointers to and from the menus are set correctly
	sub->super_menu = super;
	super->sub_menus = realloc(super->sub_menus, super->num_of_entries * sizeof(Menu*));
	super->sub_menus[super->num_of_entries - 1] = malloc(sizeof(Menu));
	super->sub_menus[super->num_of_entries - 1] = sub;
	
	//Name of entries in the super menu are updated
	super->entries = realloc(super->entries, super->num_of_entries * sizeof(char*));
	super->entries[super->num_of_entries - 1] = malloc(strlen(sub->name) * sizeof(char));
	super->entries[super->num_of_entries - 1] = sub->name;
}

//Drawing functions (change to update?)
void draw_menu(Menu* self){
	//Print the menu
	for(int i = 0; i < self->num_of_entries; i++){
		oled_print_string(self->entries[i],0,i,5,0);
	}
	oled_invert_rectangle(0,8*(self->cursor),64,8*((self->cursor) + 1));
	
	//Where to go next
	Direction dir = joy_get_direction();
	Position pos = joy_get_position();
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
				if (self->sub_menus[self->cursor]->num_of_entries != 0){
					self = self->sub_menus[self->cursor];
					joy_held = 1;
					transition = 1;
				}
				break;
			case NEUTRAL:
				if(pos.z  == 1 && self->sub_menus[self->cursor]->action != NULL){
					self->sub_menus[self->cursor]->action(self);
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
}
void draw_list(Menu* self){

	for(int i = 0; i < self->num_of_entries; i++){
		oled_print_string(self->entries[i],0,i,5,0);
	}
	//Where to go next
	Direction dir = joy_get_direction();
	if((dir == LEFT) & (self->super_menu != NULL)){
		self = self->super_menu;
	}
}

//Action functions
void action_run_game(Menu* self){
	state = in_game;
}
void action_start_transition_right(Menu* self){
	joy_held = 1;
	transition = 1;
}
void action_toggle_name(Menu* self){	//TEST FOR CHECK BOXES IN OPTIONS
	self->entries[self->cursor]= "Herman";
	
}
//void action_animation(){}

void menu_run_display(){
	oled_clear_screen();
	current_menu->draw(current_menu);
	printf(current_menu->name);
	oled_refresh();
}
void menu_transition(Menu *self, Direction dir){
	//If done with transition
	if (transition_count == 8){
		//wrapping
		if(transition_dir == UP){
			if(self->cursor == -1){
				self->cursor = 4;
			}
			else{
				self->cursor--;
			}
		}
		else if(transition_dir == DOWN){
			//wrapping
			if(self->cursor == self->num_of_entries){
				self->cursor = 0;
			}
			else{
				self->cursor++;
			}
		}
		transition_count = 0;
		transition_dir = NEUTRAL;
		transition = 0;
	}
	//Update the transition
	else if(transition == 1 && (transition_dir == UP || transition_dir == DOWN)){
		int sign = 1;
		if(transition_dir == UP){
			sign = -1;
		}
		
		int x1 = 0;
		int x2 = 64;
		int y1 = 8*(self->cursor) + sign*transition_count;
		int y2 = 8*(self->cursor + 1) + sign*transition_count;
		int y11= y1 + sign*transition_step;
		int y22= y2 + sign*transition_step;
		
		if (transition_dir == UP){
			oled_invert_rectangle(x1,y11,x2,y22);
			oled_invert_rectangle(x1,y1,x2,y2);
			
		}
		else{
			oled_invert_rectangle(x1,y1,x2,y2);
			oled_invert_rectangle(x1,y11,x2,y22);
			
		}
		transition_count++;
	}
}


#include <stddef.h>
#include "joy.h"
#include "menu.h"
#include "oled.h"

//Flags
int joy_moved = 0;

//----------INITIALIZATION--------------
//The name of the menus and their functions
Menu main_menu = {
	.name = "Main menu",
	.num_of_entries = 0,
	.cursor = 0,
	.super_menu = NULL,
	.sub_menus = NULL,
	//.type = MENU,
	.draw = &draw_menu
};

Menu difficulty = {
	.name = "Difficulty",
	//.type = MENU,
	.draw = &draw_menu
};

Menu options = {
	.name = "Options",
	//.type = MENU,
	.draw = &draw_menu
};

Menu highscore = {
	.name = "High score",
	//.type = LIST,
	.draw = &draw_list
};

Menu control = {
	.name = "Control",
	//.type = MENU,
	.draw = draw_menu
};

Menu contro = {
	.name = "Graphics",
	//.type = MENU,
	.draw = draw_menu
};

//The initializer - How the menus are related
void menu_init(){
	
	char* control_entr[3] = {"Player1", "Player2", "Player3"};
	add_entries(&control, control_entr, 3);
	add_sub_menu(&options, &control);
	
	char* contro_entr[4] = {"Low", "Medium", "High","Ultra"};
	add_entries(&contro, contro_entr, 4);
	add_sub_menu(&options, &contro);
	
	add_sub_menu(&main_menu, &options);
	
	char* diff_entr[3] = {"Easy", "Medium", "Hard"};
	add_entries(&difficulty, diff_entr, 3);
	add_sub_menu(&main_menu, &difficulty);
	
	char* high_entr[4] = {"1. Hans","2. Raoul","3. Alle andre","-inf. Herman"};
	add_entries(&highscore, high_entr,4);
	add_sub_menu(&main_menu, &highscore);
	
}
//----------NO NEED TO CHANGE THINGS BEYOND THIS----------

//Functions for adding entries and nodes
void add_entries(Menu* menu, char* entries[], int num_of_new_entries){
	
	menu->entries = malloc(num_of_new_entries * sizeof(char*));	//Correct size of double pointer
	
	for(int i = 0; i < num_of_new_entries; i++){
		menu->entries[i] = malloc(strlen(entries[i]) * sizeof(char)); //Allocate space for each string
		menu->entries[i] =  entries[i];								  //Adds each string
		
		//---NEW---
		//Menu* empty;
		//empty = malloc(sizeof(Menu));
		//empty->name = malloc(strlen(entries[i]) * sizeof(char));
		//empty->name = entries[i];
		//add_sub_menu(menu, empty);
		menu->num_of_entries+=1;
	}
	//menu->num_of_entries = num_of_new_entries;
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
Menu* draw_menu(Menu* self){
	
	for(int i = 0; i < self->num_of_entries; i++){
		oled_print_string(self->entries[i],0,i,5,0);
	}
	oled_invert_rectangle(0,8*(self->cursor),64,8*((self->cursor) + 1));
	
	//Where to go next
	Direction dir = joy_get_direction();
	if(1){	//THIS IS NEW
		if((dir == UP) & (self->cursor != 0)){
			for (int i = 0; i < 8; i++,i++){
				oled_invert_rectangle(0,8*(self->cursor) - i - 2,64,8*((self->cursor) + 1) - i - 2);
				oled_invert_rectangle(0,8*(self->cursor) - i,64,8*((self->cursor) + 1) - i);
				oled_refresh();
			}
			self->cursor--;
		}
		else if((dir == DOWN) & (self->cursor != (self->num_of_entries - 1))){
			
			//---NEW--- ANIMATION FOR DOWNWARDS MOVEMENT
			for (int i = 0; i < 8; i++,i++){
				oled_invert_rectangle(0,8*(self->cursor) + i,64,8*((self->cursor) + 1) + i);
				oled_invert_rectangle(0,8*(self->cursor) + i + 2,64,8*((self->cursor) + 1) + i + 2);
				oled_refresh();
			}
			//-------------
			
			self->cursor++;
		}
		else if((dir == RIGHT) & (self->sub_menus[self->cursor]->num_of_entries != 0)){ //THIS IS CHANGED
			self = self->sub_menus[self->cursor];
		}
		else if((dir == LEFT) & (self->super_menu != NULL)){
			self = self->super_menu;
		}
		joy_moved = 1;
	}
	else if (dir == NEUTRAL){ //THIS IS NEW
		joy_moved = 0;
	}
	

	return self;
}
Menu* draw_list(Menu* self){

	for(int i = 0; i < self->num_of_entries; i++){
		oled_print_string(self->entries[i],0,i,5,0);
	}

	//Where to go next
	Direction dir = joy_get_direction();
	if(!joy_moved){//THIS IS NEW
		if((dir == LEFT) & (self->super_menu != NULL)){
			self = self->super_menu;
		}
		joy_moved = 1;		
	}
	else if (dir == NEUTRAL){ //THIS IS NEW
		joy_moved = 0;
	}
	
	return self;
}

//Action functions
//void action_game(){}
//void action_animation(){}
	//
	
Menu* current_menu = &main_menu;

void menu_run_display(){
	oled_clear_screen();
	printf("RUN DISPLAY");
	current_menu = current_menu->draw(current_menu);
	oled_refresh();
}

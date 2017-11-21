
#ifndef MENU_H_
#define MENU_H_

typedef struct menu_t{
	char* name;
	int cursor;

	//Entries are almost the same as sub_menus, but an entry might just be a name.
	//This means that it is necessary to check if there are sub menus if one wants to 
	//do an action or a transition, not whether there are entries or not 
	char** entries;
	int num_of_entries;
	
	struct menu_t* super_menu;
	struct menu_t** sub_menus;
	
	//Functions
	struct menu_t* (*draw)(struct menu_t* self);
	void (*action)(struct menu_t* self);
	
}Menu;

//Setup functions
void menu_init();
void add_list_entries(Menu* menu, char*entries[], int num_of_new_entries);
void add_sub_menu(Menu* super, Menu* sub);

//Drawing functions - to differentiate between different types of menus (change this to update menu?)
Menu* draw_menu(Menu* self);
void draw_list(Menu* self);

//Animations
void menu_transition(Menu *self, Direction dir);

//Actions
void action_start_transition_right(Menu* self);
void action_run_game(Menu* self);
void action_toggle_name(Menu* self);

void menu_run_display();

#endif /* MENU_H_ */
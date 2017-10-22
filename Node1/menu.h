
#ifndef MENU_H_
#define MENU_H_

//Denne kan fjernes
//typedef enum menu_type_t{
	//MENU,
	//LIST,
	//IMAGE,
	//ANIMATION
//}Type;

typedef struct menu_t{
	char* name;
	char** entries;
	int num_of_entries;
	
	int cursor;
	
	//Type type;	//To identify this as a menu or something else THIS CAN BE REMOVED

	struct menu_t* super_menu;
	struct menu_t** sub_menus;
	
	struct menu_t* (*draw)(struct menu_t* self);
	
	//---NEW---
	//void (*action)();
	
}Menu;

//Setup functions
void add_entries(Menu* menu, char*entries[], int num_of_new_entries);
void add_sub_menu(Menu* super, Menu* sub);
void menu_init();

//Drawing functions - to differentiate between different types of menus (change this to update menu?)
Menu* draw_menu(Menu* self);
Menu* draw_list(Menu* self);

//Actions
//void action_game();
//void action_animation();

void menu_run_display();
#endif /* MENU_H_ */
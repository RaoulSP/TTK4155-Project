
#ifndef MENU2_H_
#define MENU2_H_

//Denne kan fjernes
typedef enum menu_type_t{
	MENU,
	LIST,
	IMAGE,
	ANIMATION
}Type;

typedef struct menu_t{
	char* name;
	char** entries;
	int num_of_entries;
	
	int cursor;
	
	Type type;	//To identify this as a menu or something else THIS CAN BE REMOVED

	struct menu_t* super_menu;
	struct menu_t** sub_menus;
	
	struct menu_t* (*draw)(struct menu_t* self);
	
}Menu;

Menu* draw_menu(Menu* self);
Menu* draw_list(Menu* self);

void add_entries(Menu* menu, char*entries[], int num_of_entries);
void add_sub_menu(Menu* super, Menu* sub);
void init_menus();
#endif /* MENU2_H_ */
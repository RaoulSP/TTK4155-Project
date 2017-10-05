////replace with actual menu entries	
//#include "menu.h"
//#include "joy.h"
//#include "oled.h"
//
//Menu current_menu = MAIN_MENU;
//Main_cursor main_cursor = C_DIFFICULTY;
//Difficulty_cursor difficulty_cursor = EASY;
//Highscore_cursor highscore_cursor = HERMAN;
//
//void menu_controller(){
	////printf("%d",current_menu);
	//switch (current_menu)
	//{
		//case MAIN_MENU:
			//oled_clear_screen();
			//oled_print_string("Difficulty",0,0,8,0);
			//oled_print_string("Highscore",0,1,8,0);
			//oled_print_string("Options",0,2,8,0);
			//oled_print_string("Easter Egg",0,3,8,0);
			//
			//Direction dir = joy_get_direction();
			//if( dir == UP & main_cursor != C_DIFFICULTY){
				//main_cursor--;
			//}
			//else if(dir == DOWN & main_cursor != C_EASTER_EGG){
				//main_cursor++;
			//}
			//else if(dir == RIGHT){
				//printf("%d", current_menu);
				//current_menu = main_cursor+1;
			//}
			//
			//oled_invert_rectangle(0,main_cursor*8,127,(main_cursor + 1)*8);
			//
			//break;
		//case DIFFICULTY:
			//oled_clear_screen();
			//oled_print_string("Easy",0,0,8,0);
			//oled_print_string("Medium",0,1,8,0);
			//oled_print_string("Hard",0,2,8,0);
			//oled_print_string("Nightmare",0,3,8,0);
	//
			//if(joy_get_direction() == UP & difficulty_cursor != EASY){
				//difficulty_cursor--;
			//}
			//else if(joy_get_direction() == DOWN & difficulty_cursor != NIGHTMARE){
				//difficulty_cursor++;
			//}
			//else if(joy_get_direction() == LEFT){
				//current_menu = MAIN_MENU;
			//}
			//
			//oled_invert_rectangle(0,difficulty_cursor*8,127,(difficulty_cursor + 1)*8);
			//break;
			//
		//case HIGHSCORE:
			//oled_clear_screen();
			//oled_print_string("1. Herman",0,0,8,0);
			//oled_print_string("2. Raoul",0,1,8,0);
			//oled_print_string("3. Bestemor",0,2,8,0);
			//oled_print_string("4. Hans",0,3,8,0);
			//
			//if(joy_get_direction() == UP & highscore_cursor != HERMAN){
				//highscore_cursor--;
			//}
			//else if(joy_get_direction() == DOWN & highscore_cursor != HANS){
				//highscore_cursor++;
			//}
			//else if(joy_get_direction() == LEFT){
				//current_menu = MAIN_MENU;
			//}
			//
			//oled_invert_rectangle(0,difficulty_cursor*8,127,(difficulty_cursor + 1)*8);
			//break;
			//
		//case OPTIONS:
			//break;
		//
		//case EASTER_EGG:
			//break;
			//
		//default:
			//break;
		//}
	//
	//oled_refresh();
//}

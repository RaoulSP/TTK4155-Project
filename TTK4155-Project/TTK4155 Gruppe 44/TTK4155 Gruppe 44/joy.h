
typedef struct JOY_position_t 
{
	 int x, y
}JOY_position_t;
typedef struct JOY_direction_t 
{

}JOY_direction_t;

void JOY_init();
void JOY_calibrate();
void JOY_button(int button);
JOY_position_t JOY_getPosition();
JOY_direction_t JOY_getDirection();
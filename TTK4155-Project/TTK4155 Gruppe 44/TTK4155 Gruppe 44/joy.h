
typedef struct JOY_position_t 
{
	 int x, y
}Position;
typedef enum JOY_direction_t 
{
	UP,
	DOWN,
	LEFT,
	RIGHT
}Direction;

void JOY_init();
void JOY_calibrate();
void JOY_button(int button);
Position JOY_getPosition( Position ex);
Direction JOY_getDirection(Direction ex);
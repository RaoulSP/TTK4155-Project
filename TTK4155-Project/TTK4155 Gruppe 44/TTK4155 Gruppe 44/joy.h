

typedef struct JOY_position_t
{
	int x, y;
} Position;
typedef enum JOY_direction_t
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	NEUTRAL
} Direction;


#ifndef JOY_H_
#define JOY_H_


void JOY_init();
void JOY_calibrate();
void JOY_button(int button);
Position JOY_getPosition();
//Direction JOY_getDirection(Direction ex);

#endif /* JOY_H_ */
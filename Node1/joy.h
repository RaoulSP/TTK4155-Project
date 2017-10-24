#define set_bit( reg, bit ) (reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))

typedef struct joy_position_t
{
	int x, y, z;
} Position;

typedef struct joy_position_pol_t
{
	int angle, amplitude;
} Position_polar;

typedef enum joy_direction_t
{
	RIGHT,
	UP,
	LEFT,
	DOWN,
	NEUTRAL = -1
} Direction;

#ifndef JOY_H_
#define JOY_H_

void joy_init();
void joy_calibrate();
Position joy_get_position();
Position_polar joy_get_position_polar();
Direction joy_get_direction();

void joy_print(int all);
#endif /* JOY_H_ */
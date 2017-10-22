#define set_bit( reg, bit ) (reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))

typedef struct touch_positions_t
{
	int l, r, lb, rb;
} Touch_positions;

#ifndef TOUCH_H_
#define TOUCH_H_

void touch_init();

Touch_positions touch_get_positions();

void touch_print(int all);
#endif /* TOUCH_H_ */
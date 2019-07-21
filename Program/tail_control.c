#include "tail_control.h"
#include "ev3api.h"

void tail_init(motor_port_t tail_control, int tail_position, float tail_speed)
{
	int now_position = 0;
	int tail_power; 
	int diff = 99;
	while(diff == 0)
	{
		
		now_position = ev3_motor_get_counts(tail_control);
		diff = tail_position - now_position;
		tail_power = diff * tail_speed;
		
		if(tail_power > 60) tail_power = 60;
		if(tail_power < -60) tail_power = -60;
		
		if(tail_power == 0) ev3_motor_stop();
		else ev3_motor_set_power(tail_control, (int)tail_power);	
	}
}

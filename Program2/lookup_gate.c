#include "lookup_gate.h"
#include "ev3api.h"

volatile int sonor;
volatile int sec_flag;

void lookup_gate(motor_port_t left_motor, motor_port_t right_motor)
{	
	//ルックアップゲートを潜り抜けたかどうかの判定
	int lookup_bar_flag = 0;
	
	ev3_speaker_play_tone(NOTE_A6, 100);
	while(1)
	{
		// ev3_speaker_play_tone(NOTE_A6, 100);
		ev3_motor_set_power(left_motor, 30);
		ev3_motor_set_power(right_motor, 30);
		// if(sonor < 10) lookup_bar_flag = 1;
		// if(lookup_bar_flag && sonor > 20)
		// {
			// ev3_speaker_play_tone(NOTE_G6, 100);
			// ev3_motor_stop(left_motor, false);
			// ev3_motor_stop(right_motor, false);
		// }
		if(sec_flag) break;
	}
}
#include "app.h"

void *__dso_handle = 0;

#define RED_STATE 0
#define GREEN_STATE 1
#define ORANGE_STATE 2

unsigned short state;
unsigned int counter_m, counter_s;
static int time_m, time_s;
static int stamp = 1;

//LEDの状態変更
void led_change()
{
	ledcolor_t color;
	switch(state)
	{
		case RED_STATE:
			color = LED_RED;
			break;
		case GREEN_STATE:
			color = LED_GREEN;
			break;
		case ORANGE_STATE:
			color = LED_ORANGE;
			break;
		default:
			color = LED_RED;
			break;
	}
	ev3_led_set_color(color);
}

//状態の変更
void state_change()
{
	switch(state)
	{
		case RED_STATE:
			state = GREEN_STATE;
			break;
		case GREEN_STATE:
			state = ORANGE_STATE;
			break;
		case ORANGE_STATE:
			state = RED_STATE;
			break;
		default:
			state = RED_STATE;
			break;
	}
	led_change();
}

//システムの初期化
void initialize()
{
	state = LED_OFF;
	ev3_speaker_set_volume(1);
	ev3_led_set_color(LED_RED);
}

void create_system()
{
	act_tsk(SUB_TASK);
}

void finalize()
{
	state = LED_OFF;
	//ev3_speaker_stop();
	ev3_led_set_color(LED_OFF);
}

void wait_task()
{
    if(counter_m++ == time_m) {
		stamp *= -1;
        wup_tsk(MAIN_TASK);
    }
	if(counter_s++ == time_s && stamp == 1) {
        wup_tsk(SUB_TASK);
    }
	
	if(counter_m < 11) ev3_speaker_play_tone(NOTE_B5, 500);
	//state_change();
    
	ext_tsk(); //WAIT_TASK is finished
}

void sub_task()
{
	setTime(time_s + 3, 0); //from now to + a few seconds waiting
	//ev3_speaker_play_tone(NOTE_E5, 500);
	timer_reset();
	ext_tsk();
}

void setTime(int wait_time, int flag)
{
	if(flag == 1) time_m = wait_time;
	else time_s = wait_time;

	if(flag == 1) ev3_sta_cyc(TIMER);
	slp_tsk();
	state_change();
	ev3_speaker_play_tone(NOTE_C5, 500);
	if(flag == 1) ev3_stp_cyc(TIMER);
}

void timer_reset()
{
	time_s = 0;
	stamp *= -1;
}

//周期タスク(1sec周期)
void timer_act_1sec(intptr_t exinf)
{
	act_tsk(WAIT_TASK);
}

//メインタスク
void main_task(intptr_t exinf)
{
	//システムの初期化
	initialize();

	create_system();

	setTime(10, 1);

	finalize();

	//タスクの終了
	ext_tsk();
};
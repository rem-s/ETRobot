#include "app.h"

void *__dso_handle = 0;

#define RED_STATE 0
#define GREEN_STATE 1
#define ORANGE_STATE 2

unsigned short state;
unsigned int counter;
static short stamp = 1;

//システムの初期化
void initialize()
{
	state = RED_STATE;
	ev3_led_set_color(LED_RED);	
}

//システム生成
void create_system()
{
	//周期ハンドラの起動
	//ev3_sta_cyc(EV3_CYC_TSK_1SEC);	
}

//システムデリート
void delete_system()
{
	//周期ハンドラ終了
	//ev3_stp_cyc(EV3_CYC_TSK_1SEC);
}

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

void loop()
{
    while(stamp != -1) {
        
    }
    stamp *= -1;
}

void wait_(int time)
{
    if(counter++ == time) {
        stamp *= -1;
        wup_tsk(MAIN_TASK);
    }

    ext_tsk();
}

void make_wait()
{
    ev3_sta_cyc(EV3_CYC_TSK_WAIT);
    
    loop();

    ev3_stp_cyc(EV3_CYC_TSK_WAIT);
}

//周期タスク(1sec周期)
void timer_act_1sec(intptr_t exinf)
{
	act_tsk(CONTROLER);
}

//メインタスク
void main_task(intptr_t unused)
{
	//システムの生成
	create_system();

	//システムの初期化
	initialize();
	
    make_wait();

	//メインタスクのスリープ
	slp_tsk();
	
    ev3_led_set_color(LED_GREEN);

    make_wait();

    slp_tsk();

	//システムの削除
	delete_system();

	//タスクの終了
	ext_tsk();
}

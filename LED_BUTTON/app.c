#include "app.h"

void *__dso_handle = 0;

#define RED_STATE 0
#define GREEN_STATE 1
#define ORANGE_STATE 2

unsigned short state;

//システムの初期化
void initialize()
{
	state = RED_STATE;
	ev3_led_set_color(LED_RED);	
}

//システム生成
void create_system()
{
	
}

//システムデリート
void delete_system()
{
	
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

//ボタン確認
void button_moniter()
{
	if(ev3_button_is_pressed(ENTER_BUTTON))
	{
		state_change();
	}
}

//メインタスク
void main_task(intptr_t unused)
{
	//システムの生成
	create_system();

	//システムの初期化
	initialize();
	
	//無限ループ
	while(1)
	{
		button_moniter();
	}
	
	//システムの削除
	delete_system();

	//タスクの終了
	ext_tsk();
}

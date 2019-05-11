#include "app.h"

void *__dso_handle = 0;

//システム生成
static void create_system()
{
	
}

//システムデリート
static void delete_system()
{
	
}

//システムの初期化
static void initialize()
{
	ev3_speaker_set_volume(100); // volume = 0 means "mute"
	ev3_led_set_color(LED_OFF);
	ev3_lcd_set_font(EV3_FONT_MEDIUM);
}

void on_button()
{
	static int state = 1;
	int32_t duration = SOUND_MANUAL_STOP;

	if(state == 1){
		ev3_speaker_play_tone(NOTE_A4, 1000);
		ev3_led_set_color(LED_RED);
	}
	else {
		ev3_speaker_stop();
		ev3_led_set_color(LED_GREEN);
	}
	state *= -1;
}

int ext_tsk();

void main_task(intptr_t unused)
{
	//必要な変数の宣言
	uint8_t volume = 100;
	uint16_t frequency = NOTE_C4;
	int32_t duration = SOUND_MANUAL_STOP; //ms
	char str[20];

    //システムの生成
	create_system();

	//システムの初期化
	initialize();

	//実装
	sprintf(str, "Battery: %d", ev3_battery_current_mA());
	ev3_lcd_draw_string(str, 0, 10);
	sprintf(str, "Battery: %d", ev3_battery_voltage_mV());
	ev3_lcd_draw_string(str, 0, 30);
	
	ev3_led_set_color(LED_GREEN);
	while(1)
	{
		ev3_button_set_on_clicked(UP_BUTTON, on_button, 0);
	}

	//システムの削除
	delete_system();

	//タスクの終了
	ext_tsk();
};
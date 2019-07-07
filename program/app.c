#include "app.h"

void *__dso_handle = 0;

#define GO 1
#define STOP 0

unsigned short state;

//システムの初期化
void initialize(){
  state = STOP;

  //モータとモータポートの接続
  ev3_motor_config(EV3_PORT_A, LARGE_MOTOR);

  //周期ハンドラの起動
  ev3_sta_cyc(EV3_MOTOR_TSK_1SEC);
}

//システムデリート
void delete_system(){

}

void state_change(){

	switch(state){
		case STOP:
      ev3_motor_stop(EV3_PORT_A, true);
		   state = GO;
       ev3_led_set_color(LED_RED);
			break;
		default:
      ev3_motor_set_power(EV3_PORT_A, 50);
			state = STOP;
       ev3_led_set_color(LED_GREEN);
			break;
    }
}

//サブタスク
void sub_task(intptr_t exinf){

  //状態変更
	state_change();

	//周期タスクの終了
	ext_tsk();
}

//周期タスク(1sec周期)
void timer_act_1sec(intptr_t exinf){
	act_tsk(CONTROLER);
}

//メインタスク
void main_task(intptr_t unused){

	//システムの初期化
	initialize();

	//メインタスクのスリープ
	slp_tsk();

	//システムの削除
	delete_system();

	//タスクの終了
	ext_tsk();
}

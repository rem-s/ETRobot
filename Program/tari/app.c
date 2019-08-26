/*  EV3 Test Program
 *  tari/s1250039
 *  LCDはピクセル指定
 */

#include "ev3api.h"
#include "app.h"
#include "tarilcd.h"
#include <stdio.h>

#define DEBUG

#ifdef DEBUG
#define _debug(x) (x)
#else
#define _debug(x)
#endif



//センサポート定義
static const sensor_port_t
	gyro_sensor= EV3_PORT_1,
	touch_sensor    = EV3_PORT_2,
	sonor_sensor	= EV3_PORT_3,
	color_sensor    = EV3_PORT_4;

//モーターポート定義
static const motor_port_t
	left_motor      = EV3_PORT_B,
	right_motor     = EV3_PORT_A,
	tail_motor      = EV3_PORT_C;

//bluetooth connect変数
static volatile int bluetooth_done = 0;
static volatile int tail_done = 0;
static volatile int touch_flag = 0;
static volatile int sonor_flag = 0;

//テイルモーター制御用情報変数
//static tail_type tail_info = {99, 0, 0, 0.0};

void initialize()
{	
	//Bluetooth接続
	///bt = ev3_serial_open_file(EV3_SERIAL_BT);
	//file = fopen("forward.txt", "w");

	//カラーセンサ初期化
	ev3_sensor_config( color_sensor, COLOR_SENSOR );

	//ジャイロセンサ初期化
	ev3_sensor_config( gyro_sensor, GYRO_SENSOR );

	//測距センサ
	ev3_sensor_config( sonor_sensor, ULTRASONIC_SENSOR );

	//タッチセンサ
	ev3_sensor_config( touch_sensor, TOUCH_SENSOR );

	// 左タイヤモーター出力ポートの設定
	ev3_motor_config( left_motor, LARGE_MOTOR );

	//右タイヤモーター出力ポートの設定
	ev3_motor_config( right_motor, LARGE_MOTOR );

	//尻尾モーター出力ポートの設定
	ev3_motor_config( tail_motor, LARGE_MOTOR );

	//走行モーターエンコーダーリセット
	ev3_motor_reset_counts(left_motor);
	ev3_motor_reset_counts(right_motor);
	ev3_motor_reset_counts(tail_motor);
	//ジャイロセンサーリセット
	ev3_gyro_sensor_reset(gyro_sensor);

	//倒立振子リセット
	//balance_init(); 

	//テイルモーター初期化
	// tail_info.diff = 99;
	// tail_info.speed = 0.8;
	// tail_info.now_angle = ev3_motor_get_counts(tail_motor);;
	// tail_info.target_angle = FIRST_TAIL_POSITION;

	//LCD初期化
	ev3_lcd_set_font(EV3_FONT_MEDIUM);
	init_lcd(1);
}

//システムデリート
void delete_system()
{
	//ファイルクローズ
	//fclose(bt);
	//fclose(file);

	//周期ハンドラの終了
	ev3_stp_cyc(CYC_HANDLER1);
	ev3_stp_cyc(CYC_HANDLER2);

	//モーターストップ
	ev3_motor_stop(right_motor, true);
	ev3_motor_stop(left_motor, true);
	ev3_motor_stop(tail_motor, true);
}

void main_task(intptr_t exinf)
{
	//システムの初期化
	initialize();

	//周期タスクの起動
	ev3_sta_cyc(CYC_HANDLER1);
	ev3_sta_cyc(CYC_HANDLER2);

	//メインタスクのスリープ
	slp_tsk();

	//システムの削除
	delete_system();

	//タスクの終了
	ext_tsk();
}

//周期ハンドラ1
void cyc_handler1(intptr_t exinf)
{
	act_tsk(CYC_TASK1);
}

//周期ハンドラ
void cyc_handler2(intptr_t exinf)
{
	act_tsk(CYC_TASK2);
}

void cyc_task1(intptr_t exinf)
{
	tarilcd_disp(1,"Shinchoku");
	//周期タスクの終了
	ext_tsk();
}

void cyc_task2(intptr_t exinf)
{
	tarilcd_disp(2,"Robocon");
	//周期タスクの終了
	ext_tsk();
}



}
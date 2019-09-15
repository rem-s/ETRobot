#include "app.h"
#include "balancer.h"
#include "line_trace.h"
#include "sensor_process.h"
#include "bluetooth.h"
#include "measure_position.h"
// #include "lookup_gate.h"

#include <stdlib.h>
#include <stdio.h>

void *__dso_handle = 0;

//バックラッシュキャンセルマクロ(固定値)※調整時に変更したほうがよさそう
#define BACKLASH 2

//ファイル入出力
FILE *bt;
FILE *file;
FILE *file2;

//センサポート定義
static const sensor_port_t
    gyro_sensor     = EV3_PORT_1,
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
static tail_type tail_info = {99, 0, 0, 0.0};

//スタートフォワード
//Gyro_offsetは１０～２０などの間にすること
static float Forward = 10;
// static float Gyro_offset = 12;
static float Gyro_offset = 9;

//振り子センサカウント
int color_sensor_value;
int turn = 0;

//自己位置判定用変数
float theta;
float x_position;
float y_position;
float speed;

//倒れる判定変数
static int fall;

//測距センサ
volatile int sonor = 0;

//ルックアップゲートを検知したかどうかの判定
int bar_flag = 0;

volatile int sec_flag = 0;

static float TARGET_GYRO_OFFSET = 2.0;

//ゲートをくぐっている間カウント(100msごとにカウント)
static int bar_count = 0;

//バー検知から三点倒立までのカウント
static int bar_wait = 0;

static int after_bar_count = 0;

//ルックアップゲート内におけるturnの係数
static volatile float beta = 1.0;

//スタートするまでカウント
static int start_count = 0;

volatile float DESIRED_VALUE = 13;

int init_color_sensor_value;

//スタートから一定時間たつまでturnを0にする
volatile int count = 0;

//初期化処理
void initialize()
{
	//Bluetooth接続
	bt = ev3_serial_open_file(EV3_SERIAL_BT);
	file = fopen("forward.txt", "w");
	file2 = fopen("lookup.txt", "w");
	
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
	balance_init(); 
	
	//テイルモーター初期化
	tail_info.diff = 99;
	// tail_info.speed = 0.8;
	tail_info.now_angle = ev3_motor_get_counts(tail_motor);
	// tail_info.target_angle = FIRST_TAIL_POSITION;
	
	TARGET_GYRO_OFFSET = -10.0;
	tail_info.speed = 0.8;
	tail_info.target_angle = LOOKUP_TAIL_POSITION;
	
}

//システムデリート
void delete_system()
{
	//ファイルクローズ
	fclose(bt);
	fclose(file);
	fclose(file2);
	
	//周期ハンドラの終了
	ev3_stp_cyc(CYC_HANDLER1);
	ev3_stp_cyc(CYC_HANDLER2);
	ev3_stp_cyc(CYC_HANDLER4);
	ev3_stp_cyc(CYC_HANDLER5);
	
	//モーターストップ
	ev3_motor_stop(right_motor, true);
	ev3_motor_stop(left_motor, true);
	ev3_motor_stop(tail_motor, true);
}

//周期タスク5(ルックアップゲート内のライントレース)
void cyc_task5(intptr_t exinf)
{
}

//周期タスク4(ルックアップゲート内の処理)
void cyc_task4(intptr_t exinf)
{
	static int approach_count; 
	
	//測距センサの値を取得する
    sonor = ev3_ultrasonic_sensor_get_distance(sonor_sensor);
	
	if(bar_flag == 0)
	{
		//測距センサで取得した距離が20cm以下の場合、
		if(sonor < 20) approach_count++;
		else approach_count = 0;
		
		if(approach_count > 4)
		{
			// bar_flag = 1;
			// ev3_speaker_play_tone(NOTE_B6, 100);
			// TARGET_GYRO_OFFSET = -10.0;
			// tail_info.speed = 0.8;
			// tail_info.target_angle = LOOKUP_TAIL_POSITION;
			// ev3_sta_cyc(CYC_HANDLER5);
		}
    }
	else
	{
		bar_wait++;
		
		//バー検知してからbar_countをカウント(100msごと)
		if(bar_count++ > 10000)
		{
			ev3_speaker_play_tone(NOTE_G6, 100);
			// TARGET_GYRO_OFFSET = 2.0;
			// ev3_motor_set_power(left_motor, 20);
			// ev3_motor_set_power(right_motor, 20);
			// tail_info.speed = 0.2;
			// tail_info.target_angle = START_TAIL_POSITION;
			// if(after_bar_count++ > 15)
			// {
				// ev3_speaker_play_tone(NOTE_C4, 1000);
				// tail_info.speed = 1.5;
				// tail_info.target_angle = START_TAIL_POSITION;
				// tail_info.target_angle = ISRUN_TAIL_POSITION;
				wup_tsk(MAIN_TASK);
			// }
		}
	}
    //周期タスクの終了
    ext_tsk();
}

//周期タスク3
void cyc_task3(intptr_t exinf)
{
	char message[30];
	
	//ev3の設置場所確認用にLCDにカラーセンサ値を表示する
	init_color_sensor_value = ev3_color_sensor_get_reflect(color_sensor);
	sprintf(message, "COLOR_VALUE:%d", init_color_sensor_value);
	ev3_lcd_draw_string(message, 0, 10 );
	
	if(start_count++ > 7000) 
	{
		bar_flag = 1;
		wup_tsk(MAIN_TASK);
	}
}

//周期タスク2
void cyc_task2(intptr_t exinf)
{	
	float tail_power;
	
	tail_info.now_angle = ev3_motor_get_counts(tail_motor);
	tail_info.diff = tail_info.target_angle - tail_info.now_angle;
	tail_power = (float)tail_info.diff * (float)tail_info.speed;

	if(tail_power > 60) tail_power = 60;
	if(tail_power < -60) tail_power = -60;
	
	if(tail_power == 0) ev3_motor_stop(tail_motor, true);
	else ev3_motor_set_power(tail_motor, (int)tail_power);
	
	// if(tail_info.now_angle == START_TAIL_POSITION)
	// {
		// if(tail_info.diff == 0) wup_tsk(MAIN_TASK);
	// }
	
	//周期タスクの終了
	ext_tsk();
}

//周期タスク関数1
void cyc_task1(intptr_t exinf)
{	
	//テイルモーター確認
	// if(tail_info.target_angle == START_TAIL_POSITION) 
	// {
		// if(tail_info.diff == 0)
		// {
			// tail_info.speed = 0.2;
			// tail_info.target_angle = ISRUN_TAIL_POSITION;
		// }
	// }
	
	//タッチセンサONでメインタスクを起動する
	if(touch_flag) wup_tsk(MAIN_TASK);
	
	else if(fall > 100) wup_tsk(MAIN_TASK);
	
	// else if(bar_flag && abs(TARGET_GYRO_OFFSET - Gyro_offset) <= 0.01) wup_tsk(MAIN_TASK);
	
	//タッチセンサOFFで周期タスクの実行中断
	else
	{	
		//タッチセンサ状態取得
		touch_flag = ev3_touch_sensor_is_pressed(touch_sensor);
		
		//カラーセンサの値取得
		float color_sensor_filtered_value;
		float color_sensor_normalize_value;
		color_sensor_value = ev3_color_sensor_get_reflect(color_sensor);
		filtering(color_sensor_value, &color_sensor_filtered_value);
		normalization(color_sensor_filtered_value, &color_sensor_normalize_value);
		line_trace(color_sensor_normalize_value, &turn);
		
		int right_motor_angle;
		int left_motor_angle;
		int gyro_sensor_value;
		int voltage_value;
		
		left_motor_angle = ev3_motor_get_counts(left_motor);
		right_motor_angle = ev3_motor_get_counts(right_motor);
		gyro_sensor_value = ev3_gyro_sensor_get_rate(gyro_sensor);
		voltage_value = ev3_battery_voltage_mV();
		
		//ルックアップゲート内でのライン走行
		float left_motor_turn;
		float right_motor_turn;
		ev3_led_set_color( LED_ORANGE );
		
		if(count++ < 250)
		{
			turn = 0;
		}
		
		if(turn*beta > 0)
		{
			right_motor_turn = 2;
			left_motor_turn = 2 + (turn*beta);
		}
		else if(turn*beta < 0)
		{
			left_motor_turn = 2;
			right_motor_turn = 2 + (-1*turn*beta);
		}
		else
		{
			left_motor_turn = 4;
			right_motor_turn = 4;
			// ev3_speaker_play_tone(NOTE_A4, 10);
		}
		
		// left_motor_turn = 5 - (turn*beta);
		// right_motor_turn = 5 + (turn*beta);
		
		//ファイル書き込み
			fprintf(file, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
					(float)color_sensor_value,
					(float)color_sensor_normalize_value,
					(float)Forward,
					(float)turn,
					(float)gyro_sensor_value,
					(float)left_motor_angle,
					(float)right_motor_angle,
					(float)voltage_value,
					(float)left_motor_turn,
					(float)right_motor_turn,
					(float)Gyro_offset,
					(float)theta,
					(float)x_position,
					(float)y_position,
					(float)speed,
					(float)turn*beta);
		
		//左モーター制御
		if(left_motor_turn == 0) ev3_motor_stop(left_motor, false);
		else ev3_motor_set_power(left_motor, (int)left_motor_turn);
		//右モーター制御
		if(right_motor_turn == 0) ev3_motor_stop(right_motor, false);
		ev3_motor_set_power(right_motor, (int)right_motor_turn);
		
		//終了判定用処理
		if(abs(left_motor_turn) > 30 || abs(right_motor_turn) > 30)
		{
			fall++;
		}
		else
		{
			fall = 0;
		}
		
		//自己位置判定
		measure_position(right_motor_angle, left_motor_angle, &theta, &x_position, &y_position, &speed);
	}
	//周期タスクの終了
	ext_tsk();
}

//backlash cancel
void backlash_cancel(signed char left_motor_pwm, signed char right_motor_pwm, int *right_motor_angle, int *left_motor_angle)
{
	if(left_motor_pwm < 0) *left_motor_angle += BACKLASH;
	else if(left_motor_pwm > 0) *left_motor_angle -= BACKLASH;
	
	if(right_motor_pwm < 0) *right_motor_angle += BACKLASH;
	else if(right_motor_pwm > 0)*right_motor_angle -= BACKLASH;
}

//周期タスク1(4msec周期)
void cyc_handler1(intptr_t exinf)
{
	act_tsk(CYC_TASK1);
}

//周期タスク2(10msec周期)
void cyc_handler2(intptr_t exinf)
{
	act_tsk(CYC_TASK2);
}

//周期タスク3(1msec周期)
void cyc_handler3(intptr_t exinf)
{
	act_tsk(CYC_TASK3);
}

//周期タスク4(100msec周期)
void cyc_handler4(intptr_t exinf)
{
	act_tsk(CYC_TASK4);
}

//周期タスク5(4msec周期)
void cyc_handler5(intptr_t exinf)
{
	act_tsk(CYC_TASK5);
}

//メインタスク
void main_task(intptr_t exinf)
{
	//システムの初期化
	initialize();
	
	ev3_sta_cyc(CYC_HANDLER2);
	ev3_sta_cyc(CYC_HANDLER3);
	ev3_sta_cyc(CYC_HANDLER4);
	
	//メインタスクのスリープ
	slp_tsk();
	
	ev3_stp_cyc(CYC_HANDLER3);
	
	ev3_led_set_color( LED_ORANGE );
	
	//周期ハンドラの起動
	// ev3_sta_cyc(CYC_HANDLER2);
	ev3_sta_cyc(CYC_HANDLER1);
	
	//メインタスクのスリープ
	slp_tsk();
	
	//システムの削除
	delete_system();
	
	ev3_led_set_color( LED_RED );

	//タスクの終了
	ext_tsk();
}
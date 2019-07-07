#include "app.h"
#include "balancer.h"
#include <stdlib.h>
#include <stdio.h>

void *__dso_handle = 0;

//ファイル入出力
static FILE *file;

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

//初期化処理
void initialize()
{
	//ファイルデータ取得
	file = fopen("TASKER.txt", "w");
	fprintf(file, "file open\n");
	
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

    //ジャイロセンサーリセット
    ev3_gyro_sensor_reset(gyro_sensor);
	
	//倒立振子リセット
	balance_init(); 

}

//システムデリート
void delete_system()
{
	//ファイルクローズ
	fclose(file);
	
	//周期ハンドラの終了
	ev3_stp_cyc(CYC_HANDLER1);
	
	//モーターストップ
	ev3_motor_stop(right_motor, true);
	ev3_motor_stop(left_motor, true);
	ev3_motor_stop(tail_motor, true);
}

//周期タスク関数
void cyc_task1(intptr_t exinf)
{
	//タッチセンサ状態
	static int touch_flag = 0;
	
	//タッチセンサONでメインタスクを起動する
	if(touch_flag) wup_tsk(MAIN_TASK);
	
	//タッチセンサOFFで周期タスクの実行中断
	else
	{	
		//タッチセンサ状態取得
		touch_flag = ev3_touch_sensor_is_pressed(touch_sensor);
		
		//全身と回転の角度の変数
		int forward = 50;
		int turn = 0;
		
		//倒立振り子変数
		int right_motor_angle;
		int left_motor_angle;
		int gyro_sensor_value;
		int voltage_value;
		signed char right_motor_pwm;
		signed char left_motor_pwm;
		left_motor_angle = ev3_motor_get_counts(left_motor);
		right_motor_angle = ev3_motor_get_counts(right_motor);
		gyro_sensor_value = ev3_gyro_sensor_get_rate(gyro_sensor);
		voltage_value = ev3_battery_voltage_mV();
		
		fprintf(file, "%lf\t : %lf\t : %lf\t : %lf\t : %lf\t : %lf\t\n", 
		(float)forward, (float)turn, (float)gyro_sensor_value,
		(float)left_motor_angle, (float)right_motor_angle, (float)voltage_value);
		
		balance_control(
			(float)forward,
			(float)turn,
			(float)gyro_sensor_value,
			(float)3,
			(float)left_motor_angle,
			(float)right_motor_angle,
			(float)voltage_value,
			(signed char*)&left_motor_pwm,
			(signed char*)&right_motor_pwm);
		
		//左モーター制御
		if(left_motor_pwm == 0) ev3_motor_stop(left_motor, true);  
		else ev3_motor_set_power(left_motor, (int)left_motor_pwm);
		
		//右モーター制御
		if(right_motor_pwm == 0) ev3_motor_stop(right_motor, true);  
		else ev3_motor_set_power(right_motor, (int)right_motor_pwm);
	}
	
	//周期タスクの終了
	ext_tsk();
}

//周期タスク1(400msec周期)
void cyc_handler1(intptr_t exinf)
{
	act_tsk(CYC_TASK1);
}

//メインタスク
void main_task(intptr_t exinf)
{
	//システムの初期化
	initialize();
	
	//周期ハンドラの起動
	ev3_sta_cyc(CYC_HANDLER1);
	
	//メインタスクのスリープ
	slp_tsk();
	
	//システムの削除
	delete_system();

	//タスクの終了
	ext_tsk();
}

#include "app.h"
#include "balancer.h"
#include "line_trace.h"
#include "sensor_process.h"
#include "bluetooth.h"

#include <stdlib.h>
#include <stdio.h>

void *__dso_handle = 0;

//バックラッシュキャンセルマクロ(固定値)※調整時に変更したほうがよさそう
#define BACKLASH 4
#define BLACK 20
#define TAIL_INIT_POSITION 80

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
	
//bluetooth connect変数
static volatile int bluetooth_cnct = 0;

//タッチセンサ状態
static volatile int touch_flag = 0;

//ev3が前に転倒した際にプログラムを終了するためのフラグ
static volatile int sonor_flag = 0;

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
	ev3_motor_reset_counts(tail_motor);

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
    //ev3_stp_cyc(CYC_HANDLER4);
	
	//モーターストップ
	ev3_motor_stop(right_motor, true);
	ev3_motor_stop(left_motor, true);
	ev3_motor_stop(tail_motor, true);
}

//周期タスク関数
void cyc_task1(intptr_t exinf)
{
	//タッチセンサ状態
	//static int touch_flag = 0;
    
    int sonor;
	
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
		
		//カラーセンサの値取得
		int color_sensor_value;
		float color_sensor_filtered_value;
		float color_sensor_normalize_value;
		color_sensor_value = ev3_color_sensor_get_reflect(color_sensor);
		filtering(color_sensor_value, &color_sensor_filtered_value);
		normalization(color_sensor_filtered_value, &color_sensor_normalize_value);

		//Line trace
		line_trace(color_sensor_normalize_value, &turn);
		
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
		
		//ファイル出力
		fprintf(file, "%lf\t : %lf\t : %lf\t : %lf\t : %lf\t : %lf\t : %lf\t : %lf\t : %lf\t\n", 
		(float)forward, (float)turn, (float)gyro_sensor_value,
		(float)left_motor_angle, (float)right_motor_angle, (float)voltage_value, (float)color_sensor_value, (float)color_sensor_filtered_value, (float)color_sensor_normalize_value);
		
		//バックラッシュキャンセル
		backlash_cancel(left_motor_pwm, right_motor_pwm, &left_motor_angle, &right_motor_angle);
		
		//倒立振り子API
		balance_control(
			(float)forward,
			(float)turn,
			(float)gyro_sensor_value,
			(float)0,
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
        
        //測距センサの値を取得する
        // sonor = ev3_ultrasonic_sensor_get_distance(sonor_sensor);
        
        //5cm未満の値を取得している間、フラグに加算する
        // if(sonor > 0 && sonor < 5) sonor_flag++;
        //フラグをリセットする
        // else sonor_flag = 0;
    
        //取得した値が5cm未満の状態が50回(20秒間)続いたらメインタスクを起動する
        // if(sonor_flag >= 50) wup_tsk(MAIN_TASK);
        
        //周期ハンドラの起動
        ev3_sta_cyc(CYC_HANDLER4);
        
        //task1のスリープ
        slp_tsk();
        
        
	}
	
	//周期タスクの終了
	ext_tsk();
}

void cyc_task4(intptr_t exinf)
{
    int sonor;
    
    char message[30];
    char message2[30];
    
    //測距センサの値を取得する
    sonor = ev3_ultrasonic_sensor_get_distance(sonor_sensor);
    
    sprintf(message, "ULTRASONIC:%d          ", sonor);
    sprintf(message2, "SONOR_FLAG:%d          ", sonor_flag);
    
    //5cm未満の値を取得している間、フラグに加算する
    if(sonor != 0 && sonor < 5) sonor_flag++;
    //フラグをリセットする
    else sonor_flag = 0;
    
    //取得した値が5cm未満の状態が20回続いたらメインタスクを起動する
    if(sonor_flag >= 20)
    {
        //wup_tsk(MAIN_TASK);
        
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

//周期タスク4(100msec周期)
void cyc_handler4(intptr_t exinf)
{
    act_tsk(CYC_TASK4);
}

//メインタスク
void main_task(intptr_t exinf)
{
	//システムの初期化
	initialize();
	
	//Bluetooth通信
	//bluetooth_w();
	
	//周期ハンドラの起動
	ev3_sta_cyc(CYC_HANDLER1);
    //ev3_sta_cyc(CYC_HANDLER4);
	
	//メインタスクのスリープ
	slp_tsk();
	
	//システムの削除
	delete_system();

	//タスクの終了
	ext_tsk();
}

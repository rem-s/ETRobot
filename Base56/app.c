#include "app.h"
#include "balancer.h"
#include "line_trace.h"
#include "sensor_process.h"
#include "bluetooth.h"
#include "measure_position.h"
#include "lookup_gate.h"

#include <stdlib.h>
#include <stdio.h>

void *__dso_handle = 0;

//バックラッシュキャンセルマクロ(固定値)※調整時に変更したほうがよさそう
#define BACKLASH 2

//ファイル入出力
FILE *bt;
FILE *file;

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
static float Gyro_offset = 12;

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

//初期化処理
void initialize()
{
	//Bluetooth接続
	bt = ev3_serial_open_file(EV3_SERIAL_BT);//bluetoothの設定
	file = fopen("forward.txt", "w");

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
	tail_info.speed = 0.8;
	tail_info.now_angle = ev3_motor_get_counts(tail_motor);;
	tail_info.target_angle = FIRST_TAIL_POSITION;

}

//システムデリート
void delete_system()
{
	//ファイルクローズ
	fclose(bt);
	fclose(file);

	//周期ハンドラの終了
	ev3_stp_cyc(CYC_HANDLER1);
	ev3_stp_cyc(CYC_HANDLER2);
	ev3_stp_cyc(CYC_HANDLER4);

	//モーターストップ
	ev3_motor_stop(right_motor, true);
	ev3_motor_stop(left_motor, true);
	ev3_motor_stop(tail_motor, true);
}

//周期タスク5(シーソー)
void cyc_task5(intptr_t exinf){
  ev3_motor_stop(right_motor, true);
  ev3_motor_stop(left_motor, true);

  //周期タスクの終了
  ext_tsk();

}
//周期タスク4
void cyc_task4(intptr_t exinf)
{
    int sonor;

    //測距センサの値を取得する
    sonor = ev3_ultrasonic_sensor_get_distance(sonor_sensor);

    //周期タスクの終了
    ext_tsk();
}

//周期タスク3
void cyc_task3(intptr_t exinf)
{
	if(bluetooth_done == 0)//bluetooth接続の判定
	{
		if(bluetooth_w())
		{
			bluetooth_done = 1;
			wup_tsk(MAIN_TASK);
		}
	}
	//周期タスクの終了
	ext_tsk();
}

//周期タスク2
void cyc_task2(intptr_t exinf)//tailモータの制御
{
	float tail_power;

	tail_info.now_angle = ev3_motor_get_counts(tail_motor);
	tail_info.diff = tail_info.target_angle - tail_info.now_angle;
	tail_power = (float)tail_info.diff * (float)tail_info.speed;

	if(tail_power > 60) tail_power = 60;//上限
	if(tail_power < -60) tail_power = -60;//下限

	if(tail_power == 0) ev3_motor_stop(tail_motor, true);
	else ev3_motor_set_power(tail_motor, (int)tail_power);//ここでモータが動く

	if(tail_info.now_angle == START_TAIL_POSITION)
	{
		if(tail_info.diff == 0) wup_tsk(MAIN_TASK);
	}

	//周期タスクの終了
	ext_tsk();
}

//周期タスク関数1
void cyc_task1(intptr_t exinf)
{
	//テイルモーター確認
	if(tail_info.target_angle == START_TAIL_POSITION)
	{
		if(tail_info.diff == 0)
		{
			tail_info.speed = 0.2;
			tail_info.target_angle = ISRUN_TAIL_POSITION;
		}
	}

	//タッチセンサONでメインタスクを起動する
	if(touch_flag) wup_tsk(MAIN_TASK);

	else if(fall > 100) wup_tsk(MAIN_TASK);

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
		normalization(color_sensor_filtered_value, &color_sensor_normalize_value);//正規化（やらない）
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

		//バックラッシュキャンセル
		backlash_cancel(left_motor_pwm, right_motor_pwm, &left_motor_angle, &right_motor_angle);

		//フォワード
		Forward += KFORWARD_START * (TARGET_FORWARD - Forward);
		Gyro_offset += KGYRO_OFFSET * (TARGET_GYRO_OFFSET - Gyro_offset);
		if(Forward < TARGET_FORWARD-20)turn = 0;

		//倒立振り子API
		balance_control(
			(float)Forward,
			(float)-1 * turn,
			(float)gyro_sensor_value,
			(float)Gyro_offset,
			(float)left_motor_angle,
			(float)right_motor_angle,
			(float)voltage_value,
			(signed char*)&left_motor_pwm,
			(signed char*)&right_motor_pwm);

		//ファイル書き込み
		fprintf(file, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",(float)color_sensor_value,(float)color_sensor_normalize_value,(float)Forward,(float)turn,(float)gyro_sensor_value,(float)left_motor_angle,(float)right_motor_angle,(float)voltage_value,(float)left_motor_pwm,(float)right_motor_pwm,(float)Gyro_offset,(float)theta,(float)x_position,(float)y_position,(float)speed);

		//左モーター制御
		if(left_motor_pwm == 0) ev3_motor_stop(left_motor, false);
		else ev3_motor_set_power(left_motor, (int)left_motor_pwm);

		//右モーター制御
		if(right_motor_pwm == 0) ev3_motor_stop(right_motor, false);
		else ev3_motor_set_power(right_motor, (int)right_motor_pwm);

		//終了判定用処理
		if(abs(left_motor_pwm) > 90 || abs(right_motor) > 90)
		{
			fall++;
		}
		else
		{
			fall = 0;
		}

		//自己位置判定
		measure_position(right_motor_angle, left_motor_angle, &theta, &x_position, &y_position, &speed);

    //追加した部分
    if(gyro_sensor_value > 100){
      ev3_speaker_play_tone(NOTE_D6, 1000);

      //テイルモーター初期化状態に戻す
      tail_info.diff = 99;
      tail_info.speed = 0.8;
      tail_info.now_angle = ev3_motor_get_counts(tail_motor);;
      tail_info.target_angle = FIRST_TAIL_POSITION;
    }
      wup_tsk(MAIN_TASK);
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
	act_tsk(CYC_TASK2);//CYC_TASK2を実行
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

//周期タスク5(1msec周期)
void cyc_handler5(intptr_t exinf)
{
  act_tsk(CYC_TASK5);
}

//メインタスク
void main_task(intptr_t exinf)
{
	//システムの初期化
	initialize();

	//Tail制御用タスクの起動
	ev3_sta_cyc(CYC_HANDLER2);
	ev3_sta_cyc(CYC_HANDLER3);

	//メインタスクのスリープ
	slp_tsk();

	//bluetooth用タスクの停止
	ev3_stp_cyc(CYC_HANDLER3);

	//テイル起き上がり
	tail_info.speed = 1.5;
	tail_info.target_angle = START_TAIL_POSITION;

	slp_tsk();

	//周期ハンドラの起動
	ev3_sta_cyc(CYC_HANDLER1);
	ev3_sta_cyc(CYC_HANDLER4);

	//メインタスクのスリープ
	slp_tsk();

  //シーソー用タスクの起動
  ev3_sta_cyc(CYC_HANDLER5);

  //メインタスクのスリープ
  slp_tsk();

  //シーソー用タスクの停止
  ev3_stp_cyc(CYC_HANDLER5);

	//システムの削除
	delete_system();

	//タスクの終了
	ext_tsk();
}

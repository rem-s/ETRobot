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
	//balance_init(); 

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
	static int a = 0;
	//タッチセンサ状態
	static int touch_flag = 0;
	
	//タッチセンサONでメインタスクを起動する
	if(touch_flag) wup_tsk(MAIN_TASK);
	
	//タッチセンサOFFで周期タスクの実行中断
	else
	{
	ev3_lcd_set_font(EV3_FONT_MEDIUM);
	ev3_lcd_draw_string ("Chocolate\n",10,0);


	ev3_speaker_play_tone(NOTE_C4,10);
	ev3_speaker_play_tone(NOTE_E4,10);
	ev3_speaker_play_tone(NOTE_G4,10);
	
		ev3_speaker_play_tone(NOTE_C4,10);
	ev3_speaker_play_tone(NOTE_E4,10);
	ev3_speaker_play_tone(NOTE_G4,10);
		ev3_speaker_play_tone(NOTE_C4,10);
	ev3_speaker_play_tone(NOTE_E4,10);
	ev3_speaker_play_tone(NOTE_G4,10);
		ev3_speaker_play_tone(NOTE_C4,10);
	ev3_speaker_play_tone(NOTE_E4,10);
	ev3_speaker_play_tone(NOTE_G4,10);
		ev3_speaker_play_tone(NOTE_C4,10);
	ev3_speaker_play_tone(NOTE_E4,10);
	ev3_speaker_play_tone(NOTE_G4,10);
		ev3_speaker_play_tone(NOTE_E4,10);
	ev3_speaker_play_tone(NOTE_G4,10);
		ev3_speaker_play_tone(NOTE_C4,10);
	ev3_speaker_play_tone(NOTE_E4,10);
	ev3_speaker_play_tone(NOTE_G4,10);
	ev3_lcd_draw_string ("Shinchoku",100,0);
	a++;
	if(ev3_button_is_pressed (ENTER_BUTTON))exit(8);
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

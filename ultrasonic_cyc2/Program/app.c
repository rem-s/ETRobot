#include "app.h"

void *__dso_handle = 0;

unsigned short state;

// センサー、モーターの接続を定義します
static const sensor_port_t
    gyro_sensor        = EV3_PORT_1,
    touch_sensor       = EV3_PORT_2,
    ultrasonic_sensor  = EV3_PORT_3,
    color_sensor       = EV3_PORT_4;

//static const motor_port_t
//    right_motor    = EV3_PORT_A,
//    left_motor     = EV3_PORT_B,
//    tail_motor     = EV3_PORT_C;

// 測距センサ値
static int dist = 20;

// 周期ハンドラがメインタスクから呼び出されたか、サブタスクから呼び出されたのかの場合分け
static int flag = 0;

// 初期化
void initialize()
{
    ev3_led_set_color( LED_GREEN );
    // センサーポートを定義する
    ev3_sensor_config( color_sensor, COLOR_SENSOR );
    ev3_sensor_config( gyro_sensor, GYRO_SENSOR );
    ev3_sensor_config( ultrasonic_sensor, ULTRASONIC_SENSOR );
    ev3_sensor_config( touch_sensor, TOUCH_SENSOR );
}

// サブタスク
void sub_task(intptr_t exinf)
{
    ev3_led_set_color( LED_RED );
    // 距離の値を変更する
    dist = 10;
    
    // 周期ハンドラがサブタスクから呼ばれたことのフラグ
    flag = 1;
    
    // 周期ハンドラの起動
    ev3_sta_cyc( EV3_CYC_TSK_1SEC );
    
    // サブタスクのスリープ
    slp_tsk();
    
    ev3_led_set_color( LED_RED );
    
    // 周期ハンドラの終了
    ev3_stp_cyc( EV3_CYC_TSK_1SEC );
    
    // メインタスクを起床する
    wup_tsk( MAIN_TASK );
    
    // サブタスクの終了
    ext_tsk();
}

// 距離測定
void us_task( intptr_t exinf ) {
    char message[30];
    char message2[30];
    int ultrasonic;
    
    ev3_led_set_color( LED_GREEN );
    
    ultrasonic = ev3_ultrasonic_sensor_get_distance( ultrasonic_sensor );
    
    if(ultrasonic != 0) {
        sprintf(message, "ULTRASONIC:%d          ", ultrasonic);
        sprintf(message2, "FLAG:%d          ", flag);
        // 取得した距離を表示する
        ev3_lcd_draw_string( message, 0, 10 );
        // メイン->周期タスクから呼ばれたときは0を表示
        // サブ->周期タスクから呼ばれたときは1を表示する
        ev3_lcd_draw_string( message2, 0, 20 );
        
        if( ultrasonic < dist ) {
            if(flag == 0) {
                wup_tsk( MAIN_TASK );
            } else {
                wup_tsk( SUB_TASK );
            }
        }
    }
    // タスクの終了
    ext_tsk();
}

// 周期タスク(1sec周期)
void timer_act_1sec( intptr_t exinf )
{
    act_tsk( US_TASK );
}

// メインタスク
void main_task( intptr_t exinf )
{
    // システムの初期化
    initialize();
    
    // 周期ハンドラの起動
    ev3_sta_cyc( EV3_CYC_TSK_1SEC );
    
    // メインタスクのスリープ
    slp_tsk();
    
    ev3_led_set_color( LED_ORANGE );
    
    // 周期ハンドラの終了
    ev3_stp_cyc( EV3_CYC_TSK_1SEC );
    
    // サブタスクの起動
    act_tsk( SUB_TASK );
    
    // メインタスクのスリープ
    slp_tsk();
    
    //ev3_led_set_color( LED_ORANGE );
    
    // メインタスクの終了
    ext_tsk();
}

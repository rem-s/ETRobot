#include "app.h"

void *__dso_handle = 0;

#define RED_STATE 0
#define GREEN_STATE 1
#define ORANGE_STATE 2

unsigned short state;

// センサー、モーターの接続を定義します
static const sensor_port_t
    gyro_sensor     = EV3_PORT_1,
    touch_sensor    = EV3_PORT_2,
    ultrasonic_sensor    = EV3_PORT_3,
    color_sensor    = EV3_PORT_4;

//static const motor_port_t
//    right_motor    = EV3_PORT_A,
//    left_motor     = EV3_PORT_B,
//    tail_motor     = EV3_PORT_C;

// 測距センサ値
double dist = 10.0;

// 周期ハンドラがメインタスクから呼び出されたか、サブタスクから呼び出されたのかの場合分け
int flag = 1;

// 初期化
void initialize()
{
    // センサーポートを定義する
    ev3_sensor_config( color_sensor, COLOR_SENSOR );
    ev3_sensor_config( gyro_sensor, GYRO_SENSOR );
    ev3_sensor_config( ultrasonic_sensor, ULTRASONIC_SENSOR );
    ev3_sensor_config( touch_sensor, TOUCH_SENSOR );
}

// サブタスク
void sub_task()
{
    // 距離の値を変更する
    dist = 5.0;
    
    // 周期ハンドラがサブタスクから呼ばれたことのフラグ
    flag = 0;
    
    // 周期ハンドラの起動
    ev3_sta_cyc( EV3_CYC_TSK_1SEC );
    
    // サブタスクのスリープ
    slp_tsk();
    
    // メインタスクを起床する
    wup_tsk( MAIN_TASK );
    
    // サブタスクの終了
    ext_tsk();
}

// 距離測定
double get_distance() {
    char message[30];
    int ultrasonic = 1000;
    
    ultrasonic = ev3_ultrasonic_sensor_get_distance( ultrasonic_sensor );
    if( ultrasonic == 0 ) {
        ev3_led_set_color( LED_RED );
    } else {
        ev3_led_set_color( LED_GREEN );
    }
    sprintf(message, "ULTRASONIC:%d          ", ultrasonic);
    ev3_lcd_draw_string( message, 0, 10 );
    
    return ultrasonic;
}

// 周期タスク(1sec周期)
void timer_act_1sec( intptr_t exinf )
{
    double tmp_dist = 100.0;
    // 距離の取得
    while(tmp_dist >= dist) {
        tmp_dist = get_distance();
    }
    
    if(flag) {
        wup_tsk( MAIN_TASK );
    } else {
        wup_tsk( SUB_TASK );
    }
    
    //周期タスクの終了
    ext_tsk();
}

// メインタスク
void main_task( intptr_t unused )
{
    // システムの初期化
    initialize();
    
    // 周期ハンドラの起動
    ev3_sta_cyc( EV3_CYC_TSK_1SEC );
    
    // メインタスクのスリープ
    slp_tsk();
    
    // サブタスクの起動
    act_tsk( SUB_TASK );
    
    // メインタスクのスリープ
    slp_tsk();
    
    // メインタスクの終了
    ext_tsk();
}

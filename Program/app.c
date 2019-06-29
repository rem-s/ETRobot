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

//システムの初期化
void initialize()
{
    state = RED_STATE;
    ev3_led_set_color( LED_RED );
    
    // 周期ハンドラの起動
    ev3_sta_cyc( EV3_CYC_TSK_1SEC );
}

//システム生成
void create_system()
{
    //ev3_led_set_color( LED_ORANGE );
    ev3_sensor_config( color_sensor, COLOR_SENSOR );
    ev3_sensor_config( gyro_sensor, GYRO_SENSOR );
    ev3_sensor_config( ultrasonic_sensor, ULTRASONIC_SENSOR );
    ev3_sensor_config( touch_sensor, TOUCH_SENSOR );
}

//システムデリート
void delete_system()
{
    
}

//LEDの状態変更
void led_change()
{
    ledcolor_t color;
    switch( state )
    {
        case RED_STATE:
            color = LED_RED;
            break;
        case GREEN_STATE:
            color = LED_GREEN;
            break;
        case ORANGE_STATE:
            color = LED_ORANGE;
            break;
        default:
            color = LED_RED;
            break;
    }

    ev3_led_set_color( color );
}

//状態の変更
void state_change()
{
    switch(state)
    {
        case RED_STATE:
            state = GREEN_STATE;
            break;
        case GREEN_STATE:
            state = ORANGE_STATE;
            break;
        case ORANGE_STATE:
            state = RED_STATE;
            break;
        default:
            state = RED_STATE;
            break;
    }
    led_change();
}

// 距離測定
void get_distance() {
    char message[30];
    int ultrasonic = 1000;
    
    //while(1) {
    ultrasonic = ev3_ultrasonic_sensor_get_distance( ultrasonic_sensor );
    if( ultrasonic == 0 ) {
        ev3_led_set_color( LED_RED );
    } else {
        ev3_led_set_color( LED_GREEN );
    }
    sprintf(message, "ULTRASONIC:%d          ", ultrasonic);
    ev3_lcd_draw_string( message, 0, 10 );
    //}
}

//サブタスク
void sub_task()
{
    //状態変更
    //state_change();
    
    // 距離の取得
    get_distance();
    
    //周期タスクの終了
    ext_tsk();
}

//周期タスク(1sec周期)
void timer_act_1sec( intptr_t exinf )
{
    // タスクの起動
    act_tsk( SUB_TASK );
}

//メインタスク
void main_task( intptr_t unused )
{
    //システムの生成
    create_system();

    //システムの初期化
    initialize();
    
    //メインタスクのスリープ
    slp_tsk();
    
    //システムの削除
    delete_system();

    //タスクの終了
    ext_tsk();
}

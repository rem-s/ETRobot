#include "app.h"

void *__dso_handle = 0;

using namespace std;

//システム生成
static void create_system()
{

}

//システムデリート
static void delete_system()
{

}

//システムの初期化
static void initialize()
{
	
}

//モーターポートの設定
//static const motor_port_t
//	L_motor1 = EV3_PORT_A,
	//L_motor2 = EV3_PORT_D;

//メインタスク
void main_task(intptr_t unused)
{
	//システムの生成
	create_system();

	//システムの初期化
	initialize();

	//モーターとモーターポートの接続
	ev3_motor_config(EV3_PORT_A, LARGE_MOTOR);
	ev3_motor_config(EV3_PORT_D, LARGE_MOTOR);
	
	//回転角のリセット
	ev3_motor_reset_counts(EV3_PORT_A);
	ev3_motor_reset_counts(EV3_PORT_D);

	//右に転回
	while (1) {
		ev3_motor_steer(EV3_PORT_D, EV3_PORT_A, 50, 25);
	}

	ev3_motor_stop(EV3_PORT_A, true);
	ev3_motor_stop(EV3_PORT_D, true);

	//システムの削除
	delete_system();

	//タスクの終了
	ext_tsk();
}

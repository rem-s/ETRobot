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
	ev3_led_set_color(LED_ORANGE);	
}

//メインタスク
void main_task(intptr_t unused)
{
	//システムの生成
	create_system();

	//システムの初期化
	initialize();
	
	//無限ループ
	while(1);
	
	//システムの削除
	delete_system();

	//タスクの終了
	ext_tsk();
}
